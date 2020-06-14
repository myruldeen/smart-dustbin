// Include the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>

#define TRIGGER_PIN 5
#define ECHO_PIN 4
#define MAX_DISTANCE 500


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//Change the credientials below, so your ESP8266connects to your router
const char* ssid = "your wifi ssid";
const char* password = "your wifi password";

// Initialize LED pins
int red = 16;
int yellow = 14;
int blue = 13;
int green = 12;

//Change the variable to yur Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "your RPI's IP address";

// Intializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);
long now = millis();
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
} 

//This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
} 

//This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
// Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Ultrasonic")) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 

//The setup function sets your ESP GPIOs to Outputs, starts the serial communication at 
//a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(blue, OUTPUT);
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop()) {
    client.connect("ESP8266lights");
  }
    
  now = millis();
  // Pulishes new temperature and humidity every 10 seonds
  if (now - lastMeasure > 10000) {
    lastMeasure = now;
    float d = sonar.ping_cm();
    float dd;
    if (d<=3) {
      dd=100;
    } 
    if (d>3 && d<=5) {
      dd=75;
    } 
    if (d>5 && d<=10) {
      dd=50;
    } 
    if (d>10) {
      dd=0;
    } 
    static char dis[7];
    dtostrf(dd, 6, 2, dis);
    client.publish("/esp8266/ultra",dis);
    Serial.print("Percent: ");
    Serial.print(dd);
    Serial.println("% filled");
    if (d<=3) {
      digitalWrite(red,HIGH);
    } else {
      digitalWrite(red,LOW);
    }
    if (d>3 && d<=5) {
      digitalWrite(yellow,HIGH);
    } else {
      digitalWrite(yellow,LOW);
    } 
    if (d>5 && d<=10) {
      digitalWrite(blue,HIGH);
    } else {
      digitalWrite(blue,LOW);
    }
    if (d>10) {
      digitalWrite(green,HIGH);
    } else {
      digitalWrite(green,LOW);
    }
  }
}
