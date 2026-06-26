import paho.mqtt.client as mqtt
import csv
import json
import requests
from datetime import datetime

BROKER="10.80.203.245"
TOPIC="room/iaq"
CSV_FILE="iaq_data.csv"
GS_Script="https://script.google.com/macros/s/AKfycbyBwNG6rZtTfxNTWIrQML2Hac54VdNf75U-s0RJgQf-vgvuTY5vVSyEKcNg0W1XBU6cvg/exec"
try:
    open(CSV_FILE,"x")
except:
    pass
    

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    # Subscribe to both temperature and VOC topics
    client.subscribe(TOPIC)


def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload.decode())
        timestamp=datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        with open(CSV_FILE,"a",newline="")as f:
            writer=csv.writer(f)
            writer.writerow([
            timestamp,
            data.get("temp"),
            data.get("hum"),
            data.get("co2"),
            data.get("voc"),
            data.get("pm1"),
            data.get("pm25"),
            data.get("pm10")
            ])
        data_time={
        "timestamp":timestamp,
        "temp":data.get("temp"),
        "hum":data.get("hum"),
        "co2":data.get("co2"),
        "voc":data.get("voc"),
        "pm1":data.get("pm1"),
        "pm25":data.get("pm25"),
        "pm10":data.get("pm10")
        }
        response=requests.post(GS_Script,json=data_time,timeout=10)
        print("GS:",response.status_code,response.text)
        print("Logged Locally")
        
    except Exception as e:
        print(f"Error decoding message: {e}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message


client.connect(BROKER, 1883, 60)

# Loop for both MQTT and Blynk
while True:
    client.loop(timeout=0.1)

