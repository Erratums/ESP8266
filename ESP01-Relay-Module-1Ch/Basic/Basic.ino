    
/*
 * ESP01S-Relay-Module-1Ch Basic
 * Developed by Erratum Solutions
 * Contact us @ support@erratums.com
 * https://erratums.com
 * Date created: 26-Jun-2019
 */
 
#include <ESP8266WebServer.h>
#include "WebPage.h"

const char cWiFiName[] = "ES-1CH-Relay";
const char cWiFiPass[] = "trytest123";
const byte cRelayPin = 0;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  delay(100);
  InitAccessPoint();
  WiFi.setAutoConnect(true);

  delay(100);
  Serial.println(F("\nDeveloped by Erratum Solutions\nESP01S-Relay-Single Channel-Module"));

  pinMode(cRelayPin, OUTPUT);
  digitalWrite(cRelayPin, HIGH);
}

void loop() {
  server.handleClient();
}

void InitAccessPoint() {
  Serial.println();
  Serial.println(F("Configuring access point..."));

  WiFi.softAP(cWiFiName, cWiFiPass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("AP IP address: "));
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on(F("/turnon"), handleTurnOn);
  server.on(F("/turnoff"), handleTurnOff);
  server.on(F("/toggle"), handleToggleState);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println(F("HTTP server started"));
}

void handleRoot() {
  server.send(200, "text/html", cWebPageServer);
}

void handleTurnOn() {
  digitalWrite(cRelayPin, LOW);  
  Serial.println(F("Relay turned On"));  
  
  handleRoot();
}

void handleTurnOff() {
  digitalWrite(cRelayPin, HIGH);
  Serial.println(F("Relay turned Off"));  

  handleRoot();
}

void handleToggleState() {
  if (digitalRead(cRelayPin) == LOW) {
    handleTurnOff();
  } else {
    handleTurnOn();
  }
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
