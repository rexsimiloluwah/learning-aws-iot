#include <DHT.h>  // DHT11 Sensor Library
#include <DHT_U.h>
#include <ESP8266WiFi.h> // ESP8266WiFi Library for enabling connection to WiFi network
#include <PubSubClient.h> // PubSubClient for enabling MQTT connections, and pub/sub to topics on the MQTT broker
#include <Adafruit_Sensor.h> // AdaFruit Sensor Library
#include <ArduinoJson.h>  // Used this for generating the JSON payloads and efficient JSON serialization
#include <WiFiClientSecure.h> // for secure TLS connection with AWS IoT certificates and private key

#include "secrets.h" // Header file containing configuration credentials
#include "libb64/cdecode.h" // Base64 decoding algorithm

#define DHTTYPE DHT11
#define DHTPIN 4
#define BAUD_RATE 115200
#define AWS_THING_NAME "esp8266-mcu"

// Pin mapping
const uint8_t dht_pin = 4;
const uint8_t green_led_pin = 5; // Indicator
const uint8_t ldr_pin = A0; // LDR Pin
const uint8_t relay_pin = 12; // Led pin controlled via AWS IoT
unsigned long last_publish = 0;
unsigned long publish_interval = 20000; // 20s

// WiFi connection credentials 
const char* ssid = "Algorhythm";
const char* password = "theblackdove";
/*
 * Function prototypes
 */
int b64decode(char* b64Text, uint8_t* output);
void msgReceived(char* topic, byte* payload, unsigned int length);
void pubSubConnectCheck(void);
void setCurrentTime();

WiFiClientSecure wiFiClient;
PubSubClient pubSubClient(aws_iot_endpoint, 8883, msgReceived, wiFiClient);
DHT_Unified dht(DHTPIN, DHTTYPE);

BearSSL::X509List client_crt(aws_device_cert);
BearSSL::PrivateKey client_key(aws_cert_private_key);
BearSSL::X509List rootCert(aws_cert_ca);

/*
 * @desc: Function for connecting to the WiFi network
 */
void connectToWiFi(void){
  // Connect to WiFi in station mode
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connection successfully established.");
  Serial.print("Connected on IP Address: ");
  Serial.println(WiFi.localIP());
  // Reconnect the ESP8266 automatically, when connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

/*
 * @desc: Function for establishing connection between the pubSubClient and the AWS IoT core
 */
void setupAWSIoTConnection(void){
  // Configuring WiFiClientSecure to use the AWS IoT credentials 
  setCurrentTime();
  wiFiClient.setClientRSACert(&client_crt, &client_key);
  wiFiClient.setTrustAnchors(&rootCert);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(green_led_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  // Set the initial state of the Green led pin to HIGH (+3.3V)
  digitalWrite(green_led_pin, HIGH);
  // Set initial state of the Red led pin to LOW (0V)
  digitalWrite(relay_pin, HIGH); //reverse logic
  connectToWiFi();
  setupAWSIoTConnection();

  // Initialize the DHT11 sensor 
  dht.begin();
  Serial.println("[PROCESS]: Fetching DHT sensor details");
  sensor_t sensor;
  // Print temperature sensor details 
  dht.temperature().getSensor(&sensor);
  Serial.println("Temperature sensor details: ");
  Serial.print("Sensor name: - ");
  Serial.println(sensor.name);
  Serial.print("Sensor Maximum value: ");
  Serial.println(sensor.max_value);
  Serial.print("Sensor Minimum value: ");
  Serial.println(sensor.min_value);
  Serial.print("Sensor resolution: ");
  Serial.println(sensor.resolution);

  // Print humidity sensor details 
  dht.humidity().getSensor(&sensor);
  Serial.println("Humidity sensor details: ");
  Serial.print("Sensor name: - ");
  Serial.println(sensor.name);
  Serial.print("Sensor Maximum value: ");
  Serial.println(sensor.max_value);
  Serial.print("Sensor Minimum value: ");
  Serial.println(sensor.min_value);
  Serial.print("Sensor resolution: ");
  Serial.println(sensor.resolution);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Reconnect the pubSubClient if connection is lost
  pubSubConnectCheck();
  sensors_event_t event;
  float temperature;
  float humidity;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)){
    Serial.println("Error reading temperature");
  }else{
    Serial.print("Temperature: ");
    Serial.print((float)event.temperature);
    Serial.println("°C");
  }
  temperature = (float)event.temperature;

  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)){
    Serial.println("Error reading humidity");  
  }else{
    Serial.print("Humidity: ");
    Serial.print((float)event.relative_humidity);
    Serial.println("%");
  }
  humidity = (float)event.relative_humidity;

  // LDR 
  int sensor_value = analogRead(ldr_pin);
  // adc conversion
  float ldr_voltage = ((float)sensor_value * 5) /1023.0;

  // This buffer holds the serialized result for version 5 (https://arduinojson.org/v5/doc/serialization/)
  char output[128];
  // The goal here is to publish the sensor readings to the MQTT topic every {publish_interval} seconds
  if (millis() - last_publish > publish_interval) {
    StaticJsonBuffer<256> jsonBuffer;  // Use DynamicJsonBuffer to store in heap instead
    JsonObject& payload = jsonBuffer.createObject();
    payload["deviceName"] = "esp8266-mcu";
    payload["temperature"] = temperature;
    payload["humidity"] = humidity;
    payload["ldrVoltage"] = ldr_voltage;
    payload.printTo(output);
    pubSubClient.publish("esp8266-mcu/sensors/data", output);
    Serial.print("Published: ");
    payload.prettyPrintTo(Serial);
    last_publish = millis();
  }
  
  digitalWrite(green_led_pin, HIGH);
  delay(1000);
  digitalWrite(green_led_pin, LOW);
  delay(1000);
}

/*
 * @desc: Function for reconnecting the pubSubClient if connection is lost
 */
void pubSubConnectCheck(void){
  if(!pubSubClient.connected()){
    Serial.print("PubSubClient connecting to: ");
    Serial.print(aws_iot_endpoint);
    while(!pubSubClient.connected()){
      //delay(1000);
      Serial.print(".");
      pubSubClient.connect("esp8266-mcu");
    }
     // After connection is successful
    Serial.println("Successful established PUB/SUB connection with AWS IoT MQTT Broker.");
    // Subscribe to Shadow Update topic
    pubSubClient.subscribe("esp8266-mcu/in/data");
    pubSubClient.subscribe("$aws/things/esp8266-mcu/shadow/update/delta");
  }
  pubSubClient.loop();
}

/*
 * @desc: Base64 decoding algorithm for getting the size of the certificate and private key
 * @reference: Using the cdecode.h header file --> https://github.com/esp8266/Arduino/blob/master/cores/esp8266/libb64/cdecode.h
 */
int b64decode(char* b64Text, uint8_t* output){
  base64_decodestate s;
  base64_init_decodestate(&s);
  int count = base64_decode_block(b64Text, strlen(b64Text), (char*)output, &s);
  return count;
}

/*
 * @desc: Callback function when a message is received from the MQTT broker
 */
void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on ");
  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if(strcmp(topic,"$aws/things/esp8266-mcu/shadow/update/delta")==0){
    Serial.println("Device Shadow Document has been updated");
    // Deserialization of the received JSON payload: https://arduinojson.org/v5/doc/deserialization/
    const int capacity = JSON_OBJECT_SIZE(10);
    StaticJsonBuffer<capacity> jsonParser;
    JsonObject& shadow_doc = jsonParser.parseObject((char *)payload);
    if(shadow_doc.success()){
      const char* lamp_state = shadow_doc["state"]["lamp"];
      Serial.println(lamp_state);
      if(strcmp(lamp_state,"ON")==0){
        digitalWrite(relay_pin, LOW);
        Serial.println("LAMP state changed to ON");
      }else{
        digitalWrite(relay_pin, HIGH);
        Serial.println("LAMP state changed to OFF");
      }
    }else{
      Serial.println("Error parsing JSON payload");
    }
  }
  Serial.println();
}

/*
 *@desc: Set current time else the certificates will be flagged as expired
 *@reference: https://aws.amazon.com/blogs/iot/using-device-time-to-validate-aws-iot-server-certificates/
 */
void setCurrentTime(void){
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
