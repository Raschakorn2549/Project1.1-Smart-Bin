#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int inductiveSensorPin = 2;
const int servoPin = 6;  // Servo to move object (metal/non-metal)

// WiFi and MQTT Configuration
const char* ssid = "Laptop"; // Change to your WiFi SSID
const char* password = "12345678"; // Change to your WiFi password
const char* mqtt_server = "broker.hivemq.com"; // Your MQTT broker
const int mqtt_port = 1883;
const char* mqtt_Client = "trash";  // Change to your ClientID
const char* mqtt_username = "";  // Change to your username
const char* mqtt_password = ""; // Change to your password

// Line Notify Configuration
const char* lineNotifyToken = "kWKq3zMJ1YfqJdaAzZ3SZSewUDfagyZFQOMkx4a1fzm"; // Change to your LINE Notify token

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
int objectCount = 0; // Variable to count objects
int non=0 ,metal = 0;

#define btn 12

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
  if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) { //เชื่อมต่อกับ MQTT BROKER
    Serial.println("connected");
    client.subscribe("@msg/operator");
  }
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //เชื่อมต่อกับ WIFI
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //เชื่อมต่อกับ WIFI สำเร็จ แสดง IP
  client.setServer(mqtt_server, mqtt_port); //กำหนด MQTT BROKER, PORT ที่ใช้
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(inductiveSensorPin, INPUT);
  pinMode(btn, INPUT);
  servo.attach(servoPin);
  servo.write(90);  // Initial position (neutral)

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  
  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
}


void loop() {
  // Ultrasonic sensor - detecting object
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 2000) { //จับเวลาส่งข้อมูลทุก ๆ 2 วินาที
    lastMsg = now;
    ++value;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculate distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  if(digitalRead(btn)==0)
  {
    objectCount = 0;
    non = 0;
    metal = 0;
  }

  // Check if object is detected within 15 cm
  if (distance < 15) {
    Serial.println("***********Object detected***********");
    delay(1000);

    // Inductive sensor - detecting metal
    inductiveValue = digitalRead(inductiveSensorPin);
    Serial.print("Distance :");
    Serial.print(distance);
    Serial.print(" Sensor :");
    Serial.println(inductiveValue);

    if (inductiveValue == 0) {  // Assuming metal is detected
      Serial.println("Metal detected");
      servo.write(0); // Move servo for metal
      delay(2000);
      servo.write(90);
      delay(2000);
      metal++;
    } else {
      Serial.println("Non-metal detected");
      servo.write(180); // Move servo for non-metal
      delay(2000);
      servo.write(90);
      delay(2000);
      non++;
    }

    objectCount++; // Increment the object count

    // Send LINE Notify if objectCount reaches threshold (e.g., 10)
    
    DataString = "{\"data\":{\"amount\":"+(String)objectCount+",\"non_metal\":"+(String)non+",\"metal\":"+(String)metal+"}}"; 
    
    DataString.toCharArray(msg, 100);
    
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("trash/object", msg); // อย่าลืมแก้ไข TOPIC ที่จะทำการ PUBLISH ไปยัง MQTT BROKER
  

    delay(2000);
    servo.write(90);  // Return servo to neutral position
  }
  delay(1000); // Delay before next check
}
}
