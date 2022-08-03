#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "iowifi.h"
#include "WakeOnLan.h"
#include <WiFiUdp.h>
const char* mqtt_server = "bemfa.com";
const int mqtt_server_port = 9501;
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP UDP;
WakeOnLan WOL(UDP);
void turnOnLed();
void turnOffLed();

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String Mqtt_Buff = "";
  for (int i = 0; i < length; i++) {
    Mqtt_Buff += (char)payload[i];
  }
  Serial.print(Mqtt_Buff);
  Serial.println();

  if (Mqtt_Buff == "on") {
    turnOnLed();

  } else if (Mqtt_Buff == "off") {
    turnOffLed();
  }
  Mqtt_Buff = "";
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID_MQTT)) {
      Serial.println("connected");

      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_server_port);
  client.setCallback(callback);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}


void turnOnLed() {
  Serial.println("turn on PC");
  WOL.sendMagicPacket(MACAddress);
}

void turnOffLed() {
    Serial.println("turn off PC");
}
