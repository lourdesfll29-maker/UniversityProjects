# Type help("robodk.robolink") or help("robodk.robomath") for more information
# Press F5 to run the script
# Documentation: https://robodk.com/doc/en/RoboDK-API.html
# Reference:     https://robodk.com/doc/en/PythonAPI/robodk.html
# Note: It is not required to keep a copy of this file, your Python script is saved with your RDK project


# ---------------------------------------------------------------------------- #
# IMPORTACIONES NECESARIAS

from json.__init__ import loads as json_loads
import paho.mqtt.client as mqtt

from robodk import robolink    # RoboDK API
from robodk import robomath    # Robot toolbox

from robodk.robolink import ITEM_TYPE_OBJECT
from robodk.robomath import TxyzRxyz_2_Pose

RDK = robolink.Robolink()
item_01 = RDK.Item('Item 1', ITEM_TYPE_OBJECT)
robot = RDK.Item('UR3e')
tool = RDK.Item('HerramientaDual_borr')

# Establecer herramienta de la herramienta industria:
robot.setTool(tool)

# ---------------------------------------------------------------------------- #
# PARÁMETROS Y TOPICS PARA LA CONEXIÓN CON MQTT

BROKER   = 'broker.emqx.io'
PORT     =  1883
USERNAME = 'emqx'
PASSWORD = 'public'

POSITION_TOPIC = "robodk/position"


# ---------------------------------------------------------------------------- #
# FUNCIONES DESARROLLADAS

def on_message(mqttc, obj, msg):
    """
    Función que analiza el topic recibido y realiza la acción oportuna.
    """

    # El primer paso es parsear msg:
    msg_dict = json_loads((msg.payload).decode('UTF-8'))

    if msg.topic == POSITION_TOPIC:
        new_pose = TxyzRxyz_2_Pose(
            [msg_dict["x"] * 10.0,
	     msg_dict["y"] * 10.0,
	     msg_dict["z"] * 10.0,
	     0.0, 0.0, 0.0])
	    
        item_01.setPose(new_pose)
        
        pre_pick_pose = TxyzRxyz_2_Pose(
            [msg_dict["x"] * 10.0,
	     msg_dict["y"] * 10.0,
	     msg_dict["z"] * 10.0 + 10,
	     0.0, 0.0, 0.0])
        
        pick_pose = TxyzRxyz_2_Pose(
            [msg_dict["x"] * 10.0,
	     msg_dict["y"] * 10.0,
	     msg_dict["z"] * 10.0 + 3,
	     0.0, 0.0, 0.0])
        
        for i in range(1, msg_dict["quantity"] + 1):
            robot.setPoseFrame(RDK.Item('World Frame'))
            robot.MoveJ(pre_pick_pose), True)
            robot.MoveL(pick_pose), True)
            robot.MoveL(pre_pick_pose), True)
            
            robot.setPoseFrame(RDK.Item('Place Frame'))
            robot.MoveJ(RDK.Item('Pre Place Target'), True)
            robot.MoveL(RDK.Item('Place Target'), True)
            robot.MoveL(RDK.Item('Pre Place Target'), True)

# ---------------------------------------------------------------------------- #
# CREAR CLIENTE Y ESTABLECER CONEXIÓN MQTT (BUCLE)

client = mqtt.Client()
client.on_message = on_message

client.username_pw_set(username = USERNAME, password = PASSWORD)
client.connect(BROKER, PORT, 60)

client.subscribe(POSITION_TOPIC, 0)

client.loop_forever()

### end of file ###
