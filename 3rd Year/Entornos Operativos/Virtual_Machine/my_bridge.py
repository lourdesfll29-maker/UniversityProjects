# Aplicación Puente
# Se suscribe al topic /turtle1/pose para obtener la posición (x, y) y la orientación (theta)
# Se suscribe al topic /turtle1/cmd_vel para obtener la velocidad lineal y angular
# Publica estos datos en un mensaje JSON por MQTT 
# -> mensaje JSON vector de 2 componentes y 1 dato (posición, orientación)
# -> mensaje JSON de 2 datos (velocidad lineal, angular)

import rclpy
from rclpy.node import Node

from turtlesim.msg import Pose
from geometry_msgs.msg import Twist

import paho.mqtt.client as mqtt
import json

import time

BROKER_ADRESS = "192.168.0.13" # Varía según la IP del host
PORT = 1883

POSE_TOPIC = "turtle/pose"
VEL_TOPIC = "turtle/velocity"

class TurtleMQTTPublisher(Node):

    def __init__(self):
        super().__init__('turtle_mqtt_publisher')

        # Subscripciones a los tópicos
        # Posición y Orientación
        self.pose_subs = self.create_subscription(
            Pose,
            '/turtle1/pose',
            self.pose_callback,
            10)
        self.pose_subs  # prevent unused variable warning
        # Velocidad lineal y angular
        self.cmd_vel_subs = self.create_subscription(
            Twist,
            '/turtle1/cmd_vel',
            self.cmd_vel_callback,
            10)
        self.cmd_vel_subs # prevent unused variable warning

        # Configuración del cliente MQTT
        self.mqtt_client = mqtt.Client("TurtleMQTTPublisher")
        self.mqtt_client.connect(BROKER_ADRESS, PORT)

        # Tiempos de publicación e intérvalos
        self.last_pose_time = 0.0
        self.last_vel_time = 0.0
        self.pose_interval = 1.0
        self.vel_interval = 0.5

    def pose_callback(self, msg):
        current_time = time.time()
        if current_time - self.last_pose_time >= self.pose_interval:
            self.last_pose_time = current_time

            # Mensaje JSON para posición y orientación
            pose_data = {
                "position": [msg.x, msg.y],
                "orientation": msg.theta
            }
            pose_json = json.dumps(pose_data)

            # Publicación por MQTT del mensaje
            self.mqtt_client.publish(POSE_TOPIC, pose_json)
            self.get_logger().info(f"Turtle pose: {pose_json}")

    def cmd_vel_callback(self, msg):
        current_time = time.time()
        if current_time - self.last_vel_time >= self.vel_interval:
            self.last_vel_time = current_time
            
            # Mensaje JSON para velocidad
            velocity_data = {
                "linear_velocity": msg.linear.x,
                "angular_velocity": msg.angular.z
            }
            velocity_json = json.dumps(velocity_data)

            # Publicación por MQTT del mensaje
            self.mqtt_client.publish(VEL_TOPIC, velocity_json)
            self.get_logger().info(f"Turtle velocity: {velocity_json}")

def main(args=None):
    rclpy.init(args=args)

    turtle_mqtt_publisher = TurtleMQTTPublisher()

    # Manejo de interrupción mediante "ctrl+C" en terminal
    try: 
        rclpy.spin(turtle_mqtt_publisher)
    except KeyboardInterrupt:
        pass

    # Destruir el nodo y detener MQTT
    turtle_mqtt_publisher.destroy_node()
    turtle_mqtt_publisher.mqtt_client.disconnect()  # Cerrar conexión MQTT    
    rclpy.shutdown()

if __name__ == '__main__':
    main()