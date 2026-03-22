#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "secrets.h"

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int inductiveSensorPin = 2;
const int servoPin = 6;
#define btn 12

// WiFi and MQTT
const char* ssid         = WIFI_SSID;
const char* password     = WIFI_PASSWORD;
const char* mqtt_server  = "broker.hivemq.com";
const int   mqtt_port    = 1883;
const char* mqtt_Client  = MQTT_CLIENT;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char* lineNotifyToken = LINE_TOKEN;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
int value = 0;
char msg[100];
String DataString;

Servo servo;
long duration;
int distance;
int inductiveValue;
int objectCount = 0;
int non = 0, metal = 0;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/operator");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(inductiveSensorPin, INPUT);
  pinMode(btn, INPUT);

  servo.attach(servoPin);
  servo.write(90);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    // Ultrasonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    // Reset button
    if (digitalRead(btn) == 0) {
      objectCount = 0;
      non = 0;
      metal = 0;
    }

    // Object detected
    if (distance < 15) {
      Serial.println("***********Object detected***********");
      delay(1000);

      inductiveValue = digitalRead(inductiveSensorPin);
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.print(" Sensor: ");
      Serial.println(inductiveValue);

      if (inductiveValue == 0) {
        Serial.println("Metal detected");
        servo.write(0);
        delay(2000);
        servo.write(90);
        delay(2000);
        metal++;
      } else {
        Serial.println("Non-metal detected");
        servo.write(180);
        delay(2000);
        servo.write(90);
        delay(2000);
        non++;
      }

      objectCount++;

      DataString = "{\"data\":{\"amount\":" + (String)objectCount +
                   ",\"non_metal\":" + (String)non +
                   ",\"metal\":" + (String)metal + "}}";
      DataString.toCharArray(msg, 100);

      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("trash/object", msg);

      delay(2000);
      servo.write(90);
    }
    delay(1000);
  }
}