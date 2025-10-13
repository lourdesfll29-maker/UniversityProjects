'''
  BDIMEDIC_EOB.asl

  Autors:
    - Belda Martinez, Marcos
    - Espert Cornejo, Angela
    - Frances Llimera, Lourdes
'''

# Import necessary libraries for agent behavior and BDI logic
import random
import math
from pygomas.bditroop import BDITroop
from pygomas.bdisoldier import BDISoldier
from pygomas.bdifieldop import BDIFieldOp
from pygomas.bdimedic import BDIMedic
import agentspeak
from agentspeak.stdlib import actions as asp_action
from pygomas.ontology import DESTINATION

# Define the custom behavior for the Medic agent
class BDIMedicBehavior(BDIMedic):
      def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            # Track previous health and ammo to detect changes
            self.last_health = 100
            self.health_needed = 0
            self.last_ammo = 100
            self.ammo_needed = 0

      def add_custom_actions(self, actions):
            # Register custom functions with the agent
            super().add_custom_actions(actions)

            # Custom function to check agent's health status
            @actions.add_function(".checkHealth", (int,))
            def _checkHealth(x):
                  current_health = x
                  # If health has decreased, mark as needing help
                  if current_health < self.last_health:
                        self.health_needed = 1
                  else:
                        self.health_needed = 0
                  self.last_health = current_health
                  return self.health_needed

            # Custom function to check agent's ammo status
            @actions.add_function(".checkAmmo", (int,))
            def _checkAmmo(x):
                  current_ammo = x
                  # If ammo has decreased, mark as needing resupply
                  if current_ammo < self.last_ammo:
                        self.ammo_needed = 1
                  else:
                        self.ammo_needed = 0
                  self.last_ammo = current_ammo
                  return self.ammo_needed

# Define custom behavior for the FieldOp agent
class BDIFieldOpBehavior(BDIFieldOp):
      def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.last_health = 100
            self.health_needed = 0
            self.last_ammo = 100
            self.ammo_needed = 0

      def add_custom_actions(self, actions):
            super().add_custom_actions(actions)

            @actions.add_function(".checkHealth", (int,))
            def _checkHealth(x):
                  current_health = x
                  if current_health < self.last_health:
                        self.health_needed = 1
                  else:
                        self.health_needed = 0
                  self.last_health = current_health
                  return self.health_needed

            @actions.add_function(".checkAmmo", (int,))
            def _checkAmmo(x):
                  current_ammo = x
                  if current_ammo < self.last_ammo:
                        self.ammo_needed = 1
                  else:
                        self.ammo_needed = 0
                  self.last_ammo = current_ammo
                  return self.ammo_needed

# Define custom behavior for the generic Soldier agent
class BDISoldierBehavior(BDISoldier):
      def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.last_health = 100
            self.health_needed = 0
            self.last_ammo = 100
            self.ammo_needed = 0
  
      def add_custom_actions(self, actions):
            super().add_custom_actions(actions)

            @actions.add_function(".checkHealth", (int,))
            def _checkHealth(x):
                  current_health = x
                  if current_health < self.last_health:
                        self.health_needed = 1
                  else:
                        self.health_needed = 0
                  self.last_health = current_health
                  return self.health_needed

            @actions.add_function(".checkAmmo", (int,))
            def _checkAmmo(x):
                  current_ammo = x
                  if current_ammo < self.last_ammo:
                        self.ammo_needed = 1
                  else:
                        self.ammo_needed = 0
                  self.last_ammo = current_ammo
                  return self.ammo_needed       

# Define custom behavior for the Sniper agent (inherits from Soldier)
class BDISniperBehavior(BDISoldier):
      def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.last_health = 100
            self.health_needed = 0
            self.last_ammo = 100
            self.ammo_needed = 0
  
      def add_custom_actions(self, actions):
            super().add_custom_actions(actions)

            @actions.add_function(".checkHealth", (int,))
            def _checkHealth(x):
                  current_health = x
                  if current_health < self.last_health:
                        self.health_needed = 1
                  else:
                        self.health_needed = 0
                  self.last_health = current_health
                  return self.health_needed
            
            @actions.add_function(".checkAmmo", (int,))
            def _checkAmmo(x):
                  current_ammo = x
                  if current_ammo < self.last_ammo:
                        self.ammo_needed = 1
                  else:
                        self.ammo_needed = 0
                  self.last_ammo = current_ammo
                  return self.ammo_needed    