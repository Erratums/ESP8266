    
/*
 * ESP01S-Relay-Module-1Ch MQTT.ino
 * Developed by Erratum Solutions
 * Contact us @ support@erratums.com
 * https://erratums.com
 * Date created: 27-Jun-2019
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char cMyWiFiSSIDName[] = "ESoft-Test";
const char cMyWiFiPassword[] = "trytest1";

// Enter a unique topic name to which this device subscribe 
const char cMQTTSubTopic[] = "ESUniqueTopic/Sub"; // Change this to a unique code

// This can only be used for test. 
// Provided by https://beebotte.com
const PROGMEM char cMQTTServerAddr[] = "mqtt.beebotte.com";
const PROGMEM char cMQTTChannelName[] = "erratums_gitrepo";
const PROGMEM char cMQTTToken[] = "iamtkn_2TpqgXUQmLzo31T7";
const int cMQTTServerPort = 1883;

const byte cRelayPin = 0;

WiFiClient mqttWifi;
PubSubClient mqttClient(mqttWifi);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  delay(100);
  // Set a call back function for MQTT. When a message is received, this function will be triggered 
  mqttClient.setCallback(mqttCallback);
  
  delay(100);
  Serial.println(F("\nDeveloped by Erratum Solutions\nESP01S-Relay-Single Channel-Module [MQTT Test]\n"));

  pinMode(cRelayPin, OUTPUT);
  digitalWrite(cRelayPin, HIGH);

  // Connect esp module to your wifi router 
  connectToMyWiFi();

  // Update MQTT Server and Port 
  updateMQTTDetails();
}

void loop() {
  // Continously check MQTT connection and verify it's successful
  mqttReconnect();

  // This function internally check with the MQTT server for updation to the subscribed topic
  mqttClient.loop();  
}

void connectToMyWiFi() {
  Serial.println(F("Connecing to my WiFi router for Internet Access"));
  WiFi.begin(cMyWiFiSSIDName, cMyWiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.println(F("MQTT message received"));  
  for (int iCntr = 0; iCntr < length; iCntr++) {
    char chr = (char) payload[iCntr];
    if (iCntr == 0) {
      // As a test project, we are just checking the 1st char is 'T'
      if (chr == 'T') {
        digitalWrite(cRelayPin, LOW);
        Serial.println(F("\nRelay turned On\n"));
      } else {
        digitalWrite(cRelayPin, HIGH);
        Serial.println(F("\nRelay turned Off\n"));  
      }
    }
    Serial.print(chr);
  }
}

void updateMQTTDetails() {
  mqttClient.setServer(cMQTTServerAddr, cMQTTServerPort);
  Serial.println(F("MQTT details updated."));
}

void mqttReconnect() {
  if (mqttClient.connected()) {
    return;
  }

  Serial.print(F("Attempting MQTT connection..."));
  // Generate a random client ID. You can change this for better security
  String sClientID = "__ESMQTT_ClntID_" + String(random(0xffff), HEX);

  // for now simply use above generated Client ID as Client ID and user name. 
  if (mqttClient.connect(sClientID.c_str(), cMQTTToken, cMQTTToken)) {
    mqttClient.subscribe(cMQTTSubTopic);
    Serial.println(F("connected"));
  } else {
    Serial.print(F("failed, rc="));
    Serial.print(mqttClient.state());
    Serial.println(F(" try again in 3 seconds"));
    delay(3000);
  }
}
