import os
import sys
import json 
import time 
import random
from typing import Union 
from datetime import datetime
from collections import OrderedDict
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

# Configuration
CONFIG_DIR = '../config'
PORT = 8883
DEVICE_NAME = 'sample-device'
HOST = os.environ.get("AWS_IOT_ENDPOINT")
ROOT_CA_PATH = os.path.join(CONFIG_DIR,'root-CA.crt')
DEVICE_PRIVATE_KEY_PATH = os.path.join(CONFIG_DIR,'sample-device','ff1b4a6e6e-private.pem.key')
DEVICE_CERT_PATH = os.path.join(CONFIG_DIR,'sample-device','ff1b4a6e6e-certificate.pem.crt')

# Topics 
PUB_TOPIC='sample/device/out/data'
SUB_TOPIC='sample/device/in/data'

QOS = 1

mqtt_client = AWSIoTMQTTClient(DEVICE_NAME) # For certificate authentication
mqtt_client.configureEndpoint(HOST, PORT)  # For TLS Mutual authentication
mqtt_client.configureCredentials(ROOT_CA_PATH, DEVICE_PRIVATE_KEY_PATH, DEVICE_CERT_PATH)
mqtt_client.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
mqtt_client.configureDrainingFrequency(2)  # Draining: 2 Hz
mqtt_client.configureConnectDisconnectTimeout(10)  # 10 sec
mqtt_client.configureMQTTOperationTimeout(5)  # 5 sec

# Handle connection
try:
    mqtt_client.connect()
    print(f'MQTT Connection established at {HOST}')
except Exception as e:
    print(e)
    sys.exit(1)

def on_message_callback(client, userdata, message):
    """ Callback to handle received message """
    print(f"Message received: Topic - {message.topic}, Message - {message.payload.decode()}")

# Subscribe the client to the subscribe topic with a QoS of 1
mqtt_client.subscribe(SUB_TOPIC, QOS, on_message_callback)

def generate_fake_payload(device_name: str) -> OrderedDict:
    """ Function to generate fake payload """
    payload = OrderedDict()
    payload["device_id"]=device_name 
    payload["battery_soc"]=random.randint(0,100)
    payload["battery_soh"]=random.randint(0,100)
    payload["device_status"]=random.choice(["offline","online","inverter_not_detected"])
    payload["generated_energy"]=round(random.uniform(100,5000),2)
    payload["peak_power_generated"]=round(random.uniform(100,5000),2)
    payload["timestamp"]=str(datetime.now())
    return payload 

def publish_message(topic: str, payload: Union[str,dict,OrderedDict]):
    try:
        if isinstance(payload,dict) or isinstance(payload,OrderedDict):
            mqtt_client.publish(topic, json.dumps(payload), QOS)
        else:
            mqtt_client.publish(topic, str(payload), QOS)
    except Exception as e:
        print(e)
        sys.exit(1)

if __name__=="__main__":
    try:
        while True:
            payload=generate_fake_payload('sample-board-1')
            print("[PUBLISHING PAYLOAD]")
            print(payload)
            publish_message(PUB_TOPIC, payload)
            time.sleep(20)  # Publish every 20 seconds interval
    except KeyboardInterrupt:
        print("[EXITING]....")
        sys.exit(0)