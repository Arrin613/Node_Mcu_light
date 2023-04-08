#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// GPIO 5 D1
#define LED = 4;
const int ledPin = 5;
const int ldrPin = A0;

// WiFi
const char *ssid = "Arin"; // Enter your WiFi name
const char *password = "password";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp8266/led";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
      pinMode(4, OUTPUT);
  pinMode(ledPin, OUTPUT);

pinMode(ldrPin, INPUT);
    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());
        Serial.println("Connecting to public emqx mqtt broker.....");
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
    client.publish(topic, "hello emqx");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    int ldrStatus = analogRead(ldrPin);
    if (message == "2" && ldrStatus <=300)
    { 
      if(ldrStatus <=300)
      digitalWrite(ledPin, HIGH);// LED on
      else
      {
        digitalWrite(ledPin, LOW);
      }
    }
   else
   {
     if (message == "0")
     { 
      digitalWrite(4, LOW); 
     }   // LED on
    else  if (message == "1") { digitalWrite(4, HIGH); } // LED off
     else
      {
        digitalWrite(ledPin, LOW);
      }
   
   }
   
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    client.loop();
    int ldrStatus = analogRead(ldrPin);
      delay(5000);
    if (ldrStatus >=300) {
digitalWrite(ledPin,LOW);
}
}
