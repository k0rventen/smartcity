import base64
import datetime
import json
import os

import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

INFLUX_DB = os.environ["INFLUX_DB"]
MQTT_ENDPOINT = os.environ["MQTT_ENDPOINT"]
MQTT_PORT = int(os.environ["MQTT_PORT"])
MQTT_USER = os.environ["MQTT_USER"]
MQTT_PASS = os.environ["MQTT_PASS"]
MQTT_TOPIC = os.environ["MQTT_TOPIC"]


def parsePaquet(paquet):
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    paq = json.loads(paquet.payload)
    data = paq["data"]
    eui = paq["devEUI"]
    parsedData = format(int.from_bytes(base64.b64decode(data), 'big'), 'x').zfill(14)
    print(current_time,eui, parsedData)

    if "720" in eui:  # metrics
        json_body = [
            {
                "measurement": "metrics",
                "fields": {
                    "lights": int(parsedData[0]),
                    "flood": (1 if int(parsedData[3]) == 0 else 0),
                    "light": int(parsedData[10:12]),
                    "noise": int(parsedData[8:10]),
                    "temp": int(parsedData[5:7])
                }
            }
        ]
    elif "721" in eui:  # parking
        json_body = [
            {
                "measurement": "parking",
                "fields": {
                    "parking1": int(parsedData[0]),
                    "parking2": int(parsedData[2]),
                    "parking3": int(parsedData[4]),
                    "parking4": int(parsedData[6]),
                    "parking5": int(parsedData[8]),
                    "parking6": int(parsedData[10])
                }
            }
        ]
    elif "722" in eui:  # garbage
        json_body = [
            {
                "measurement": "garbage",
                "fields": {
                    "trash1": int(parsedData[0]),
                    "trash2": int(parsedData[2]),
                    "trash3": int(parsedData[4])
                }
            }
        ]
    influx_client = InfluxDBClient('influxdb', 8086, database=INFLUX_DB)
    influx_client.write_points(json_body)
    del influx_client


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    parsePaquet(msg)

if __name__ == "__main__":
    client=mqtt.Client()
    client.username_pw_set(MQTT_USER,MQTT_PASS)
    client.on_message=on_message
    client.on_connect=on_connect

    print("connecting to broker")
    client.tls_set()
    client.connect(MQTT_ENDPOINT, MQTT_PORT)
    print("Up and running..")
    client.loop_forever()
