import json
import logging
from enum import Enum
import paho.mqtt.client as mqtt

logging.basicConfig(level=logging.INFO)

class CodigoColores(Enum):
    id_rojo = "0"
    id_negro = "1"
    id_gris = "2"
    id_blanco = "3"
    id_verde = "4"
    id_naranja = "5"

String_to_codigo = {
    CodigoColores.id_rojo: 0,
    CodigoColores.id_negro: 1,
    CodigoColores.id_verde: 2,
    CodigoColores.id_gris: 3,
    CodigoColores.id_blanco: 4,
    CodigoColores.id_naranja: 5
}

class Solicitud:
    def __init__(self,tipo_chasis: int, color_chasis: CodigoColores):
        self.usado = False
        self.tipo_chasis = tipo_chasis
        self.color_chasis = String_to_codigo[color_chasis]

    def __str__(self):
        return f"Solicitud(tipo_chasis={self.tipo_chasis}, color_chasis={self.color_chasis})"

class Comunicaciones:
    def __init__(self):
        self.Robot1 = None
        self.Robot2 = None
        self.cambio = False
        
    # def __init__(self, broker_address: str, topic: str, username: str = None, password: str = None):
    #     self.broker_address = broker_address
    #     self.broker_port = 1883
    #     self.topic = topic
    #     self.client = mqtt.Client()
    #     if username and password:
    #         self.client.username_pw_set(username, password)
    #     self.Robot1 = Solicitud(0, CodigoColores.id_rojo)
    #     self.Robot1.usado = True
    #     self.Robot2 = Solicitud(0, CodigoColores.id_rojo)
    #     self.Robot2.usado = True
    #     self.cambio = False
    #     self.client.on_message = self.on_message
    
    def conectar(self):
        try:
            self.client.connect(self.broker_address, self.broker_port, 60)
            self.client.subscribe(self.topic)
            logging.info("Conectado al broker MQTT en %s:%d y suscrito al tema '%s'", 
                         self.broker_address, self.broker_port, self.topic)
            self.client.loop_start()
        except Exception as e:
            logging.error("No se pudo conectar al broker MQTT: %s", e)

    def on_message(self, client, userdata, msg):
        try:
            payload = msg.payload.decode('utf-8')
            data = json.loads(payload)

            id_device = data.get("id_device")
            if id_device == "1":                
                return
            id_estacion = data.get("id_estacion")
            tipo_chasis = data.get("tipo_chasis")
            id_color_raw = data.get("id_color")
            try:
                id_color = CodigoColores(id_color_raw)
            except ValueError as e:
                logging.error("El valor de id_color no es válido: %s", id_color_raw)
                return

            if None in (id_device, id_estacion, tipo_chasis, id_color):
                logging.warning("Mensaje JSON incompleto: %s", data)
                return

            solicitud = Solicitud(tipo_chasis=tipo_chasis, color_chasis=id_color)
            if id_estacion == 1:
                self.Robot1 = solicitud
            elif id_estacion == 2:
                self.Robot2 = solicitud
            else:
                logging.warning("ID de estación desconocido: %s", id_estacion)  
            logging.info("Solicitud recibida: %s para el robot: %d", solicitud, id_estacion)
            self.cambio = True

        except json.JSONDecodeError as e:
            logging.error("Error al decodificar el JSON: %s", e)
        except Exception as e:
            logging.error("Error al procesar el mensaje: %s", e)

    def enviar_estado_robots(self,robot1: bool, robot2: bool):
        mensaje = {
            "id_device": "1",
            "Robot1": robot1,
            "Robot2": robot2
        }
        try:
            self.client.publish(self.topic, json.dumps(mensaje))
            #logging.info("Mensaje enviado al tema '%s': %s", self.topic, mensaje)
        except Exception as e:
            logging.error("Error al enviar el mensaje: %s", e)
