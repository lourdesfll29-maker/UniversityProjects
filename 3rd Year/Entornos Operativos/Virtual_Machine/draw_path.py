# Mueve la tortuga de forma constante variando las velocidades.

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import random

class NodoDibujaCamino(Node):
    def __init__(self):
        super().__init__("drawing_path")
        self.cmd_vel_pub = self.create_publisher(Twist, "/turtle1/cmd_vel", 10)
        self.get_logger().info("Inicialización del movimiento de la tortuga.")
        self.create_timer(1.0, self.enviar_velocidad)

    def enviar_velocidad(self):
        msg = Twist()
        msg.linear.x = random.uniform(0.0, 5.0)
        msg.angular.z = random.uniform(-3.0, 3.0)
        self.cmd_vel_pub.publish(msg)
        self.get_logger().info("Velocidad modificada.")

def main(args=None):
    rclpy.init(args=args)
    nodo_pub = NodoDibujaCamino()
    rclpy.spin(nodo_pub)
    nodo_pub.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()