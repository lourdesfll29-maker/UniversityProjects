/*
  BDIMEDIC_EOB.asl

  Autors:
    - Belda Martinez, Marcos
    - Espert Cornejo, Angela
    - Frances Llimera, Lourdes
*/

// FLAG SEARCH AND PATROL

+flag(F): team(200) // Indicates the flag position (F)
  <-
  .create_control_points(F, 10, 4, C);  // Create control points around the flag (list C with 3 points)
  +control_points(C);                   // New belief with control points
  .length(C, L);
  +total_control_points(L);
  +patrolling;                          // Patrol mode activated
  +patrol_point(0);                     // Start patrol at point 0 and cycle through points
  .print("Got control points").

+target_reached(T): patrolling  
  <-
  ?patrol_point(P);
  -+patrol_point(P + 1);  // Move to next point
  -target_reached(T).

+patrol_point(P): patrolling & total_control_points(T) & P < T 
  <-
  ?control_points(C);
  .nth(P, C, A);  // Get point A at index P
  .goto(A).

+patrol_point(P): patrolling & total_control_points(T) & P == T
  <-
  -patrol_point(P);
  +patrol_point(0). // Restart patrol from the beginning

// ENEMY DETECTION AND RESPONSE

+enemies_in_fov(ID,Type,Angle,Distance,Health,Position)
  <-
  .shoot(3, Position);    // Shoot 3 times at enemy's last known position
  -patrolling;            // Stop patrol
  .print("Enemy detected: ", ID, Type, Angle, Distance, Health, Position);
  .get_service("sniper"); // Request sniper
  .get_backups;           // Request reinforcements
  .wait(1000);            // Wait 1 second for sniper and backup readiness
  +retreating.            // Activate retreat mode

+retreating: not helping  // Retreat behavior
  <- 
  ?flag(F);
  .goto(F); // Return to base/flag
  .print("Medic retreating to flag position: ", F).

+target_reached(T): retreating
  <-
  .print("Reached flag");
  +patrolling;  // Resume patrol
  -retreating;
  +patrol_point(0);
  -target_reached(T).

// SNIPER AND BACKUP NOTIFICATIONS

+sniper(S)
  <-
  .print("Getting sniper");
  ?enemies_in_fov(_,_,_,_,_,Pos);
  .send(S, tell, enemyDetected(Pos)); // Notify sniper
  .wait(300).

+myBackups(B)
  <-
  .print("Getting backups");
  ?enemies_in_fov(_,_,_,_,_,Pos);
  .send(B, tell, enemyDetected(Pos)); // Notify backups
  .wait(300).

// CURE MANAGEMENT

+cureProposal(Pos)[source(A)]: not helping  // Incoming request for help
  <-
  .print("Helping agent: ", A, " to position: ", Pos);
  -patrolling;
  +helping;
  .goto(Pos). // Move to the position to assist

+cureProposal(Pos)[source(A)]: helping
  <-
  .print("Already helping someone! Cannot help agent: ", A, " at position: ", Pos).

+target_reached(T): helping
  <-
  .print("MEDPACK to agent:", A);
  .cure;  // Apply healing
  -helping;
  +patrolling;
  +patrol_point(0);
  -target_reached(T).

// HEALTH MANAGEMENT

+health(H): H < 50
  <-
  .checkHealth(H, Res);
  +checkingHealth(Res).

+checkingHealth(Res): Res == 1
  <-
  .print("Health is low, need a medic!");
  +healthNeeded;
  .get_medics.

+checkingHealth(Res): Res == 0
  <-
  .print("Health is okay, no medic needed!").

+myMedics(M): healthNeeded
  <-
  .print("Medic needs healing!");
  ?position(Pos);
  .send(M, tell, cureProposal(Pos)).  // Request healing from another medic

// AMMO MANAGEMENT

+ammo(A): A < 50
  <-
  .checkAmmo(A, Res);
  +checkingAmmo(Res).

+checkingAmmo(Res): Res == 1
  <-
  .print("Ammo is low, need a field op!");
  +ammoNeeded;
  .get_fieldops.

+checkingAmmo(Res): Res == 0
  <-
  .print("Ammo is okay, no field op needed!").

+myFieldOps(F): ammoNeeded
  <-
  .print("Medic needs ammo!");
  ?position(Pos);
  .send(F, tell, ammoProposal(Pos)).  // Request ammo from FieldOp

// PACKS MANAGEMENT

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1002 & healthNeeded & not goingForPack // AMMO pack
  <- 
  -patrolling;
  .goto(Position);
  +goingForPack.

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1001 & ammoNeeded & not goingForPack // MEDIC pack
  <- 
  -patrolling;
  .goto(Position);
  +goingForPack.

+target_reached(T): goingForPack
  <-
  -target_reached(T);
  -goingForPack;
  +patrolling;
  +patrol_point(0);
  -target_reached(T).