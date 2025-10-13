import paho.mqtt.client as mqtt
import json
import random
import time

# Configuración WiFi / MQTT
ssid = "Livebox6-6587"     
password = "72kKLstTueD2"  
mqtt_server = "localhost"                   # Dirección del servidor MQTT
mqtt_port = 1883                            # Puerto del servidor
mqtt_topic = "sensors/inside/temperature"   # Topic al que se envía la información

# Crear cliente MQTT
client = mqtt.Client("Python_Temperature_Publisher")

def connect_mqtt():
    print("Conectando al servidor MQTT...")
    client.connect(mqtt_server, mqtt_port)
    print("Conectado al servidor MQTT")

def publish_temperature():
    temperature = random.randint(-1, 8)  # Valor aleatorio entre -1 y 8
    if temperature == -1:
        print("Error al leer el sensor")
        message = {"temperature": temperature}
    else:
        print(f"Temperatura: {temperature}")
        message = {"temperature": temperature}

    client.publish(mqtt_topic, json.dumps(message))

if __name__ == "__main__":
    connect_mqtt()
    while True:
        publish_temperature()
        time.sleep(5)  # Espera 5 segundos antes de la siguiente publicación
