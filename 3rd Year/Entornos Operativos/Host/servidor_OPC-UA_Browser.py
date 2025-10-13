# Aplicación Servidor OPC-UA
# Se suscribe a los topics del bróker Mosquitto
# Cada vez que llega un dato nuevo, lo actualiza en los nodos correspondientes OPC-UA

import threading
import time
from asyncua.sync import Server
import json
from datetime import datetime
import paho.mqtt.client as mqtt

BROKER_ADDRESS = "localhost"  
PORT = 1883       
ENDPOINT= "opc.tcp://localhost:4840/freeopcua/server/"            

POSE_TOPIC = "turtle/pose"
VEL_TOPIC = "turtle/velocity"

pose_counter = 0
velocity_counter = 0

class PoseSubscriber(threading.Thread):
    def __init__(self):
        super().__init__()
        self.topic = POSE_TOPIC
        self.client = mqtt.Client(f"pose_subscriber")
        self.client.on_message = self.on_message
        
    def run(self):
        try:
            self.client.connect(BROKER_ADDRESS, PORT)
            self.client.subscribe(self.topic)
            print(f"Pose Subscriber subscribed to topic: {self.topic}")
            self.client.loop_forever()
        except Exception as e:
            print(f"Error connecting to MQTT broker: {e}")
        
    def on_message(self, client, userdata, msg):
        # Lectura del mensaje recibido
        message = json.loads(msg.payload.decode())
        
        position = message["position"] #[x, y]
        orientation = message["orientation"]
        
        print(f"Received from {self.topic}: {message}")    
        
        # Obtención de los últimos valores
        last_position = turtle_position.get_value()
        last_orientation = turtle_orientation.get_value()
        
        # Actualización en los nodos de OPC-UA 
        turtle_position.write_value(position)
        turtle_orientation.write_value(orientation)
        turtle_last_position.write_value(last_position)
        turtle_last_orientation.write_value(last_orientation)

        print(f"Object 'Pose' updated with 'Position': {position}, Orientation: {orientation}]")
        
        # Actualización del contador
        global pose_counter
        pose_counter += 1
        
        turtle_pose_count.write_value(pose_counter)
        
class VelocitySubscriber(threading.Thread):
    def __init__(self):
        super().__init__()
        self.topic = VEL_TOPIC
        self.client = mqtt.Client(f"velocity_subscriber")
        self.client.on_message = self.on_message
        
    def run(self):
        try:
            # Conectar al broker MQTT y suscribirse al tema correspondiente
            self.client.connect(BROKER_ADDRESS, PORT)
            self.client.subscribe(self.topic)
            print(f"Velocity Subscriber subscribed to topic: {self.topic}")
            self.client.loop_forever()
        except Exception as e:
            print(f"Error connecting to MQTT broker: {e}")
        
    def on_message(self, client, userdata, msg):
        # Lectura del mensaje recibido
        message = json.loads(msg.payload.decode())
        
        linear_vel = message["linear_velocity"] 
        angular_vel = message["angular_velocity"]
        
        print(f"Received from {self.topic}: {message}")    
        
        # Obtención de los últimos valores
        last_linear = turtle_linear_vel.get_value()
        last_angular = turtle_angular_vel.get_value()
          
        # Actualización en los nodos de OPC-UA
        turtle_linear_vel.write_value(linear_vel)    
        turtle_angular_vel.write_value(angular_vel) 
        turtle_last_linear.write_value(last_linear)
        turtle_last_angular.write_value(last_angular)
            
        print(f"Object 'Velocity' updated with 'Linear': {linear_vel}, 'Angular': {angular_vel}")
        
        # Actualización del contador
        global velocity_counter
        velocity_counter += 1
        
        turtle_velocity_count.write_value(velocity_counter)
        

if __name__ == "__main__":

    # Configuración del servidor
    server = Server() 
    # Configuración del endpoint del servidor OPC-UA
    server.set_endpoint(ENDPOINT)

    # Configuración del namespace
    uri = "http://localhost/mynamespace"
    idx = server.register_namespace(uri)
    print(f"Espacio de nombres registrado: {uri} con índice: {idx}")

    # Creación de los nodos objeto y variables
    # Posición y orientación 
    myobj1 = server.nodes.objects.add_object(idx, "Pose")
    turtle_position = myobj1.add_variable(idx, "Position", [0.0, 0.0])
    turtle_orientation = myobj1.add_variable(idx, "Orientation", 0.0)    
    turtle_position.set_writable()
    turtle_orientation.set_writable()
    # Control de estado
    turtle_pose_state = myobj1.add_object(idx, "State")
    turtle_pose_count = turtle_pose_state.add_variable(idx, "Pose Counter", 0)
    turtle_last_position = turtle_pose_state.add_variable(idx, "Last Position", [0.0, 0.0])
    turtle_last_orientation = turtle_pose_state.add_variable(idx, "Last Orientation", 0.0)    
    turtle_pose_count.set_writable()
    turtle_last_position.set_writable()
    turtle_last_orientation.set_writable()
    # Velocidad    
    myobj2 = server.nodes.objects.add_object(idx, "Velocity")
    turtle_linear_vel = myobj2.add_variable(idx, "Linear", 0.0)
    turtle_angular_vel = myobj2.add_variable(idx, "Angular", 0.0)
    turtle_linear_vel.set_writable()
    turtle_angular_vel.set_writable()
    # Control de estado
    turtle_velocity_state = myobj2.add_object(idx, "State")
    turtle_velocity_count = turtle_velocity_state.add_variable(idx, "Velocity Counter", 0)
    turtle_last_linear = turtle_velocity_state.add_variable(idx, "Last Linear", 0.0)
    turtle_last_angular = turtle_velocity_state.add_variable(idx, "Last Angular", 0.0) 
    turtle_velocity_count.set_writable()
    turtle_last_linear.set_writable()
    turtle_velocity_count.set_writable()

    server.start()
    print("Servidor OPC-UA iniciado y en ejecución.")
    
    pose_subscriber = PoseSubscriber()
    vel_subscriber = VelocitySubscriber()
    
    pose_subscriber.start()
    vel_subscriber.start()