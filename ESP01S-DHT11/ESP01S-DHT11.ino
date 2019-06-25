/*
 * Smart Plug.h
 * Developed by Erratum Solutions
 * Contact us @ support@erratums.com
 * https://erratums.com
 * Date created: 22-Jun-2019
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN  2
 
ESP8266WebServer server(80);

DHT dht(DHTPIN, DHTTYPE, 11); 
 
float humidity, temp_c; // Values read from sensor
String webString = ""; // String to display
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0; // will store last temp was read
const long interval = 2000; // interval at which to read sensor
 
void handle_root() {
  server.send(200, "text/html", F("<HTML><HEAD></HEAD><BODY><CENTER><br/><H1>Hello from the weather esp8266, read from /temp or /humidity</H1></CENTER></BODY></HTML>"));
  delay(100);
}
 
void setup() {
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200); // Serial connection from ESP-01 via 3.3v console cable
  dht.begin(); // initialize temperature sensor
 
  // Connect to WiFi network
  WiFi.softAP("ESSP-DHT", "");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("AP IP address: "));
  Serial.println(myIP);
     
  server.on("/", handle_root);

  server.on("/temp", [](){ 
    getTempHumid();      
    webString = "Temperature: " + String((int) temp_c) + " C";
    server.send(200, "text/html", "<HTML><HEAD></HEAD><BODY><CENTER><br/><H1>" + webString + "</H1></CENTER></BODY></HTML>");
  });
 
  server.on("/humidity", [](){
    getTempHumid();         
    webString = "Humidity: " + String((int) humidity) + "%";
    server.send(200, "text/html", "<HTML><HEAD></HEAD><BODY><CENTER><br/><H1>" + webString + "</H1></CENTER></BODY></HTML>");
  });
  
  server.begin();
  Serial.println(F("HTTP server started"));
}
 
void loop() {
  server.handleClient();
} 
 
void getTempHumid() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();         
    temp_c = dht.readTemperature(false /* Celsius */);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  }
}
