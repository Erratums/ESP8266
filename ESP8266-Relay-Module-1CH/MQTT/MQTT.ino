    
/*
 * ESP01S-Relay-Module-1Ch MQTT Sample
 * Developed by Erratum Solutions
 * Contact us @ support@erratums.com
 * https://erratums.com
 * Date created: 27-Jun-2019
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include "WebPage.h"
#include "CoreUtils.h"

const char cWiFiName[] = "ES-1CH-Relay";
const char cWiFiPass[] = "trytest123";

// Enter a unique topic name to which this device subscribe 
const char cMQTTSubTopic[] = "ESUniqueTopic/Sub"; // Change this to a unique code

// This can only be used for test. 
// Provided by https://beebotte.com
const char cMQTTServerAddr[] = "mqtt.beebotte.com";
const char cMQTTChannelName[] = "erratums_gitrepo";
const char cMQTTToken[] = "iamtkn_2TpqgXUQmLzo31T7";
const int cMQTTServerPort = 1883;

const byte cRelayPin = 4;
const byte cRelayStateLow = LOW;
const byte cRelayStateHigh = HIGH;

SoftwareSerial resetSerial(0, 12);
EDelay dlyWiFiConn(3000);

WiFiClient mqttWifi;
ESP8266WebServer server(80);
PubSubClient mqttClient(mqttWifi);

void setup() {
  Serial.begin(9600);
  resetSerial.begin(9600);
  while (!Serial);
  while (!resetSerial);

  delay(100);
  EEPROM.begin(512);
  delay(500);
  // Try to reset before nitialization { Ajmal }
  WaitForReset();
  InitAccessPoint();
  WiFi.setAutoConnect(true);
  
  // Set a call back function for MQTT. When a message is received, this function will be triggered 
  mqttClient.setCallback(mqttCallback);
  
  delay(100);
  Serial.println(F("\nDeveloped by Erratum Solutions\nESP01S-Relay-Single Channel-Module [MQTT Test]\n"));

  pinMode(cRelayPin, OUTPUT);
  digitalWrite(cRelayPin, cRelayStateLow);

  // Connect esp module to your wifi router 
  connectToMyWiFi();

  // Update MQTT Server and Port 
  updateMQTTDetails();
}

void loop() {
  // call handleClient, so that if there is any request to server it will handle
  server.handleClient();

  // Continously check MQTT connection and verify it's successful
  mqttReconnect();

  // This function internally check with the MQTT server for updation to the subscribed topic
  mqttClient.loop();
}

void WaitForReset() {
  // Do resetting if Serial Receive "_ES_RESET" { Ajmal }
  delay(1000);
  String sData = "";
  int iResetCntr = 30; // 30x50 => 3000. Wait for 3 secs { Ajmal }
  Serial.println(F("_ES_RESET"));
  while (true) {
    delay(50);
    iResetCntr--;
    Serial.print(F("Count: ")); 
    Serial.println(iResetCntr);
    
    if (iResetCntr < 0) break;
    if (resetSerial.available() > 0) {
      char sChar = (char) resetSerial.read();
      sData = sData + sChar;
      if (sData.equals(F("_ES_RESET"))) {
        writeToMem(0, "F");
        Serial.println(F("Device Configuration Reset Successfully"));
        break;
      }
    }
  }  
}

void InitAccessPoint() {
  Serial.println();
  Serial.println(F("Configuring access point..."));

  // Set WiFi Hotspot only if not exist { Ajmal }
  if (!readFromMem(1).equals("T")) {
    writeToMem(0, "T");
    WiFi.softAP(cWiFiName, cWiFiPass);
    Serial.println(F("Default Hotspot Settings Applied"));
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("AP IP address: "));
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on(F("/saveSett"), handleSave);
  server.on(F("/turnon"), handleTurnOn);
  server.on(F("/turnoff"), handleTurnOff);
  server.on(F("/saveTopic"), handleSaveTopic);
  server.on(F("/saveHotspot"), handleSaveHotspot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("HTTP server started"));
}

void connectToMyWiFi() {
  Serial.println(F("Connecing to my WiFi router for Internet Access"));
  while (true) {
    // Check WiFi connection only once in 500 milli sec { Ajmal }
    if (dlyWiFiConn.canContinue()) {
      if (WiFi.status() == WL_CONNECTED) break;
      Serial.print('.');
    }
    
    // call handleClient, so that if there is any request to server it will handle
    server.handleClient();
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
        digitalWrite(cRelayPin, cRelayStateHigh);
        Serial.println(F("\nRelay turned On\n"));
      } else {
        digitalWrite(cRelayPin, cRelayStateLow);
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
    mqttClient.subscribe(getMQTTTopicName().c_str());
    Serial.println(F("connected"));
  } else {
    Serial.print(F("failed, rc="));
    Serial.print(mqttClient.state());
    Serial.println(F(" try again in 3 seconds"));
    delay(3000);
  }
}

String getMQTTTopicName() {
  String sTopicName = readFromMem(2);
  sTopicName.trim();
  if (!sTopicName.equals(""))
    return sTopicName;

  return cMQTTSubTopic;
}

void handleRoot() {  
  server.send(200, "text/html", cWebPageServer);
}

void handleTurnOn() {
  digitalWrite(cRelayPin, cRelayStateHigh);  
  Serial.println(F("Relay turned On"));  
  
  handleRoot();
}

void handleTurnOff() {
  digitalWrite(cRelayPin, cRelayStateLow);
  Serial.println(F("Relay turned Off"));  

  handleRoot();
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleSave() {
  boolean bWiFiSaved = false;  
  String sTemp = server.arg(F("ssid"));
  sTemp.trim();
  if (sTemp.length() != 0) {
    char userName[sTemp.length() + 1];
    sTemp.toCharArray(userName, sTemp.length() + 1);

    sTemp = server.arg(F("psw"));
    sTemp.trim();
    char pass[sTemp.length() + 1];
    sTemp.toCharArray(pass, sTemp.length() + 1);

    WiFi.begin(userName, pass);
    server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>Wi-Fi Settings Saved</H1></CENTER></BODY></HTML>"));
    Serial.println(F("Wi-Fi credentials updated."));
    connectToMyWiFi();
    bWiFiSaved = true;
  }

  if (!bWiFiSaved) {
    server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>Wi-Fi Settings Not Changed</H1></CENTER></BODY></HTML>"));
    Serial.println(F("Wi-Fi credentials updated."));
  }
}

void handleSaveHotspot() {
  boolean bHotspotSaved = false;  
  String sTemp = server.arg(F("hp_ssid"));
  sTemp.trim();
  if (sTemp.length() != 0) {
    char userName[sTemp.length() + 1];
    sTemp.toCharArray(userName, sTemp.length() + 1);

    sTemp = server.arg(F("hp_psw"));
    sTemp.trim();
    if (sTemp.length() > 7) {
      char pass[sTemp.length() + 1];
      sTemp.toCharArray(pass, sTemp.length() + 1);
      bHotspotSaved = WiFi.softAP(userName, pass);      
    } else {
      server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>Hotspot password should be atleast 8 characters</H1></CENTER></BODY></HTML>"));
      Serial.println(F("Hotspot password should be atleast 8 characters"));
      return;
    }
  }
  
  if (!bHotspotSaved) {
    server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>Hotspot Settings Not Changed</H1></CENTER></BODY></HTML>"));
    Serial.println(F("Hotspot credentials updated."));
  } else {
    server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>Hotspot Settings Saved</H1></CENTER></BODY></HTML>"));
    Serial.println(F("Hotspot credentials updated."));
  }
}

void handleSaveTopic() {
  String sTemp = server.arg(F("mqttTopic"));
  sTemp.trim();
  if (sTemp.length() != 0) {
    if (writeToMem(2, sTemp) > 0) {
      mqttClient.disconnect();
      server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><H1>MQTT Settings Saved</H1></CENTER></BODY></HTML>"));
    }
  }
}
