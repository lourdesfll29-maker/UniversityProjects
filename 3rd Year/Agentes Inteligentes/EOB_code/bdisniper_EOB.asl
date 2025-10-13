/*
  BDISNIPER_EOB.asl

  Autors:
    - Belda Martinez, Marcos
    - Espert Cornejo, Angela
    - Frances Llimera, Lourdes
*/

// BASE AND WAITING STATE

!start.

+!start
  <-
  .register_service("sniper");  // Register as a sniper service
  ?base(B);
  .goto(B).                     // Move to base position

+target_reached(T): not goingForPack
  <-
  .print("Reached base");
  +waiting; // Enter waiting mode
  -target_reached(T).

+waiting
  <-
  .print("Waiting for orders").

// ENEMY DETECTION AND RESPONSE

+enemyDetected(Position)
  <-
  -waiting;             // Exit waiting mode
  .print("Sniper shooting at: ", Position);
  .look_at(Position);   // Aim at enemy position
  .wait(500);           // Wait half a second
  .shoot(5, Position).  // Fire 5 shots

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
  .print("Sniper needs healing!");
  ?position(Pos);
  .send(M, tell, cureProposal(Pos)).  // Request help from Medic

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
  .print("Sniper needs ammo!");
  ?position(Pos);
  .send(F, tell, ammoProposal(Pos)).  // Request ammo from FieldOp

// PACKS MANAGEMENT

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1002 & healthNeeded & not goingForPack // HEALTH PACK
  <- 
  -waiting;
  .goto(Position);
  +goingForPack.

+packs_in_fov(ID,Type,Angle,Distance,Health,Position): Type == 1001 & ammoNeeded & not goingForPack // AMMO PACK
  <- 
  -waiting;
  .goto(Position);
  +goingForPack.

+target_reached(T): goingForPack
  <-
  -target_reached(T);
  -goingForPack;
  ?base(B);
  .goto(B); // Return to base after picking up pack
  +waiting;
  -target_reached(T).