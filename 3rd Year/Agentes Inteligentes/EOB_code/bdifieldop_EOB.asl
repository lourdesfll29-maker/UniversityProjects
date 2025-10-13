/*
  BDIFIELDOP_EOB.asl

  Autors:
    - Belda Martinez, Marcos
    - Espert Cornejo, Angela
    - Frances Llimera, Lourdes
*/

// FLAG SEARCH AND PATROL

+flag(F): team(200)  // Indicates the position of the flag (F)
  <-
  .create_control_points(F, 30, 4, C);  // Create control points around the flag (list C with 3 points)
  +control_points(C);                   // Add belief with control points
  .length(C, L);                        // Get the number of control points
  +total_control_points(L);
  +patrolling;                          // Enable patrol mode
  +patrol_point(0);                     // Start patrol at point 0 (cycled)
  .print("Got control points").

+target_reached(T): patrolling 
  <-
  ?patrol_point(P);      // Get current patrol point
  -+patrol_point(P + 1); // Move to the next patrol point
  -target_reached(T).

+patrol_point(P): patrolling & total_control_points(T) & P < T 
  <-
  ?control_points(C); // Retrieve list of control points
  .nth(P,C,A);        // Get point A at index P
  .goto(A).           // Move to the control point

+patrol_point(P): patrolling & total_control_points(T) & P == T
  <-
  -patrol_point(P);
  +patrol_point(0).  // Restart patrol from the beginning

// ENEMY DETECTION AND RESPONSE

+enemies_in_fov(ID,Type,Angle,Distance,Health,Position)
  <-
  .shoot(3, Position);    // Shoot 3 times at enemy's last known position
  -patrolling;            // Stop patrolling
  .print("Enemy detected: ", ID, Type, Angle, Distance, Health, Position);
  .get_service("sniper"); // Request sniper support
  .get_backups;           // Request reinforcements
  .wait(1000);            // Wait 1 second for support to get ready
  +retreating.            // Activate retreating mode

+retreating: not helping  // Retreat behavior
  <- 
  ?flag(F); // Get the flag position
  .goto(F); // Move back to the flag/base
  .print("FieldOp retreating to flag position: ", F).

+target_reached(T): retreating
  <-
  .print("Reached flag");
  +patrolling;  // Resume patrolling
  -retreating;
  +patrol_point(0);
  -target_reached(T).

// SNIPER AND BACKUP NOTIFICATIONS

+sniper(S)
  <-
  .print("Getting sniper");
  ?enemies_in_fov(_,_,_,_,_,Pos);     // Get enemy position
  .send(S, tell, enemyDetected(Pos)); // Notify sniper
  .wait(300).

+myBackups(B)
  <-
  .print("Getting backups");
  ?enemies_in_fov(_,_,_,_,_,Pos);     // Get enemy position
  .send(B, tell, enemyDetected(Pos)); // Notify backups
  .wait(300).

// RELOAD MANAGEMENT

+ammoProposal(Pos)[source(A)]: not helping  // Help requests
  <-
  .print("Helping agent: ", A, "to position: ", Pos);
  -patrolling;  // Stop patrolling
  +helping;
  .goto(Pos). // Move to the agent who needs ammo

+ammoProposal(Pos)[source(A)]: helping
  <-
  .print("Already helping someone! Cannot help agent: ", A, "to position: ", Pos).

+target_reached(T): helping
  <-
  .print("AMMOPACK to agent:", A);
  .reload;      // Refill the agent's ammo
  -helping;
  +patrolling;  // Resume patrol mode
  +patrol_point(0);
  -target_reached(T).

// HEALTH MANAGEMENT

+health(H): H < 50
  <-
  .checkHealth(H, Res); // Check whether health is low
  +checkingHealth(Res).

+checkingHealth(Res): Res == 1
  <-
  .print("Health is low, need a medic!");
  +healthNeeded;
  .get_medics.

+checkingHealth(Res): Res == 0
  <-
  .print("Health is ok, no medic needed!").

+myMedics(M): healthNeeded
  <-
  .print("FieldOp needs healing!");
  ?position(Pos);
  .send(M, tell, cureProposal(Pos)). // Request healing at current position

// AMMO MANAGEMENT

+ammo(A): A < 50
  <-
  .checkAmmo(A, Res); // Check whether ammo is low
  +checkingAmmo(Res).

+checkingAmmo(Res): Res == 1
  <-
  .print("Ammo is low, need a field op!");
  +ammoNeeded;
  .get_fieldops.

+checkingAmmo(Res): Res == 0
  <-
  .print("Ammo is ok, no field op needed!").

+myFieldOps(F): ammoNeeded
  <-
  .print("FieldOp needs ammo!");
  ?position(Pos);
  .send(F, tell, ammoProposal(Pos)).  // Request ammo resupply

// PACKS MANAGEMENT

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1002 & healthNeeded & not goingForPack // AMMO pack
  <- 
  -patrolling;
  .goto(Position);  // Move to ammo pack
  +goingForPack.

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1001 & ammoNeeded & not goingForPack // MEDIC pack
  <- 
  -patrolling;
  .goto(Position);  // Move to medic pack
  +goingForPack.

+target_reached(T): goingForPack
  <-
  -target_reached(T);
  -goingForPack;
  +patrolling;  // Resume patrolling
  +patrol_point(0);
  -target_reached(T).