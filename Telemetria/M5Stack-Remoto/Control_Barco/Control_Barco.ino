/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Import required libraries
#include <SPI.h>
#include <WiFiNINA.h>
//#include "arduino_secrets.h" 
#include "ESPAsyncWebServer.h"


// Set your access point network credentials
const char* ssid = "AmigusLabs";
const char* password = "Amigus42";


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readVol() {
  return String(25) ;
  //return String(1.8 * bme.readTemperature() + 32);
}

String readAmp() {
  return String(1);
}

String readVel() {
  return String(90);
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/voltaje", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readVol().c_str());
  });
  server.on("/amp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readAmp().c_str());
  });
  server.on("/velocity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readVel().c_str());
  });
  
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  
  
  // Start server
  server.begin();
}
 
void loop(){
  
}
