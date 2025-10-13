/*
  BDISOLDIER_EOB.asl

  Autors:
    - Belda Martinez, Marcos
    - Espert Cornejo, Angela
    - Frances Llimera, Lourdes
*/

// FLAG SEARCH AND PATROL

+flag(F): team(200) // Indicates the flag position (F)
  <-
  .create_control_points(F, 50 , 4, C); // Create control points around the flag (returns list C with 3 points)
  +control_points(C);                   // Add a belief with the control points
  .length(C, L);                        // Get the number of control points
  +total_control_points(L);             // Save total number of control points
  +patrolling;                          // Enable patrol mode
  +patrol_point(0);                     // Start patrolling from point 0 (cycle through the points)
  .print("Got control points").

+target_reached(T): patrolling  // Triggered when a patrol point is reached
  <-
  ?patrol_point(P);      // Get current patrol point
  -+patrol_point(P + 1); // Move to next patrol point
  -target_reached(T).    // Clear reached target belief

+patrol_point(P): patrolling & total_control_points(T) & P < T // Get next patrol point
  <-
  ?control_points(C); // Get list of control points
  .nth(P, C, A);      // Get point A at index P
  .goto(A).           // Go to that point

+patrol_point(P): patrolling & total_control_points(T) & P == T  // Restart patrol from beginning
  <-
  -patrol_point(P);
  +patrol_point(0).

// ENEMY DETECTION AND RESPONSE

+enemies_in_fov(ID,Type,Angle,Distance,Health,Position): patrolling
  <-
  -patrolling;            // Disable patrol mode
  +fighting;              // Enable fighting mode
  .print("Enemy detected: ", ID, Type, Angle, Distance, Health, Position);
  .get_service("sniper"); // Request sniper support
  .get_backups;           // Request backup units
  .wait(1000);            // Wait 1 second for reinforcements to prepare
  .goto(Position);        // Move to enemy's position
  .print("Soldier going to enemy position: ", Position);
  .shoot(3, Position);    // Fire 3 shots at the last seen position
  -fighting.              // End fighting mode

+enemies_in_fov(ID,Type,Angle,Distance,Health,Position): not patrolling
  <-
  +fighting;
  .shoot(3, Position);  // Fire at the enemy's position
  -fighting.

// SNIPER AND BACKUP NOTIFICATIONS

+sniper(S)
  <-
  .print("Getting sniper");
  ?enemies_in_fov(_,_,_,_,_,Pos);           // Get enemy position
  .send(S, tell, enemyDetected(Pos));  // Notify sniper
  .wait(300).

+myBackups(B)
  <-
  .print("Getting backups");
  ?enemies_in_fov(_,_,_,_,_,Pos);     // Get enemy position
  .send(B, tell, enemyDetected(Pos)); // Notify backups
  .wait(300).

// BACKUP MANAGEMENT

+enemyDetected(Position): not fighting
  <-
  .print("Enemy detected by soldier at: ", Position);
  -patrolling;      // Disable patrol mode
  .goto(Position).  // Move to enemy location

+enemyDetected(Position): fighting
  <-
  .print("Already fighting, cannot help!"). // Already in combat

+target_reached(T): fighting
  <-
  .print("Reached enemy at: ", T);
  -target_reached(T);
  -fighting;          // End fighting mode
  +patrolling;        // Resume patrol
  +patrol_point(0);  // Restart patrol from point 0
  .print("Soldier returning to patrol after combat").

// HEALTH MANAGEMENT

+health(H): H < 50
  <-
  .checkHealth(H, Res); // Evaluate whether medical attention is needed
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
  .print("Soldier needs healing!");
  ?position(Pos);                     // Get current position
  .send(M, tell, cureProposal(Pos)).  // Send request for healing

// AMMO MANAGEMENT

+ammo(A): A < 50
  <-
  .checkAmmo(A, Res); // Evaluate whether ammo is needed
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
  .print("Soldier needs ammo!");
  ?position(Pos);                     // Get current position
  .send(F, tell, ammoProposal(Pos)).  // Send request for resupply

// PACKS MANAGEMENT

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1002 & healthNeeded & not goingForPack // Health pack
  <- 
  -patrolling;
  .goto(Position);  // Move to health pack
  +goingForPack.

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1001 & ammoNeeded & not goingForPack // Ammo pack
  <- 
  -patrolling;
  .goto(Position);  // Move to ammo pack
  +goingForPack.

+target_reached(T): goingForPack
  <-
  -target_reached(T);
  -goingForPack;
  -healthNeeded;
  -ammoNeeded;
  .print("Reached pack at: ", T);
  +patrolling;  // Resume patrol mode
  +patrol_point(0);
  -target_reached(T).