#include <WiFi.h>
#include <PubSubClient.h>
#include "Constants.h"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

// setup for wifi and mqtt
void setup_connections() {
  Serial.printf("connecting to %s", wifi_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }
  Serial.println("");
  Serial.println("connected.");
  Serial.print("ip address: ");
  Serial.println(WiFi.localIP());
  pinMode(movement_pin, OUTPUT);
  mqtt_client.setServer(mqtt_server_address, mqtt_server_port);
}

// connects to mqtt
void reconnect_mqtt() {
  int counter = 0;
  while (!mqtt_client.connected()) {
    if (counter==5) ESP.restart();
    counter+=1;
    Serial.printf("connecting to mqtt server at %s", mqtt_server_address);
    Serial.println("");

    if (mqtt_client.connect(mqtt_client_name)) Serial.println("connected.");
    else {
      Serial.print("failed, rc = ");
      Serial.println(mqtt_client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("");
  setup_connections();
}

void loop() {
  if (!mqtt_client.connected()) reconnect_mqtt();

  Serial.println(digitalRead(movement_pin));
  mqtt_client.publish(movement_mqtt_topic, String(digitalRead(movement_pin)).c_str(), false);
  
  delay(3000);
}
