// Amigus Labs http://www.amiguslabs.org
//
// Sistema de monitorizado de Telemetría regata Solar Marine Instruments
//
// Created  Ardunio Day 2020 • 21/3/2020

#include "xbm.h"             // Sketch tab header for xbm images
#include <M5Stack.h>        // Hardware-specific library
#include <WiFi.h>
#include <HTTPClient.h>
#include "ListLib.h"

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char password[] = SECRET_PASS;

List<float> listVol;
List<float> listVel;
List<float> listAmp;

int totalList = 150; // longitud del array de valores

const char* serverNameVol = "http://192.168.4.1/voltaje";
const char* serverNameAmp = "http://192.168.4.1/amp";
const char* serverNameVel = "http://192.168.4.1/velocity";

String Vol;
String Amp;
String Vel;

float VolF;
float VolAnterior;
float AmpF;
float VelF;

unsigned long previousMillis = 0;
const long interval = 100;

unsigned long previousMillis1 = 0;

int tipografia = 7;
float aleatorio = 0;
String texto = "inicio";
#define GFXFF 1
#define FF24 &FreeSansBold24pt7b

String floatToString( float, int = 8, int = 2, boolean = true);

void setup() {

  M5.begin();               // Initialise the display

  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill

  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.print("Conectando   ");
  Serial.println("Hola");

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {

    unsigned long currentMillis = millis();
    unsigned long currentMillis1 = millis();
    if (currentMillis - previousMillis >= 1000) {
      M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill

      M5.Lcd.setTextFont(4);
      M5.Lcd.setCursor(10, 100);
      M5.Lcd.print("Conectando   ");
      Serial.print(".");

      previousMillis = currentMillis;
    }

    if (currentMillis1 - previousMillis1 >= 500) {
      previousMillis1 = currentMillis1;
      M5.Lcd.print(".");
    }
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  M5.Lcd.fillScreen(TFT_BLACK); // Black screen fill
  M5.Lcd.drawXBitmap(55, 0, logo, logo_W, logo_H, TFT_MAGENTA);
  M5.Lcd.drawXBitmap(6, 4, logo_labs, logo_labs_W, logo_labs_H, TFT_BLUE);
  M5.Lcd.drawXBitmap(4, 35, logo_amigus, logo_amigus_W, logo_amigus_H, TFT_ORANGE);
  M5.Lcd.fillRect(165, 185, 150, 50, BLACK);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.print("Valor:   ");


  // Crear lista

  for (int i = 0; i <= totalList; i++)
  {
    listVol.Add(0);
    listVel.Add(0);
    listAmp.Add(0);
  }
}


void loop() {





  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED ) {
      Vol = httpGETRequest(serverNameVol);
      Amp = httpGETRequest(serverNameAmp);
      Vel = httpGETRequest(serverNameVel);


      Serial.println("Vol: " + Vol + "v Amp: " + Amp + "A Vel: " + Vel + "%");

      VolF = Vol.toFloat();
      AmpF = Amp.toFloat();
      VelF = Vel.toFloat();
      Serial.print(VolF);
      Serial.print("v Amp: ");
      Serial.print(AmpF);
      Serial.print("A Vel: ");
      Serial.print(VelF);
      Serial.println("%");

      M5.Lcd.setTextColor(TFT_BLACK, TFT_MAGENTA);
      M5.Lcd.setFreeFont(FF24);
      M5.Lcd.drawString(Vol, 165, 50, GFXFF);
      //Serial.println(Vol);
      M5.Lcd.drawString(Amp, 165, 120, GFXFF);
      //  Serial.println(Vol);
      M5.Lcd.setTextFont(6);
      M5.Lcd.setTextColor(TFT_WHITE, TFT_MAGENTA);
      M5.Lcd.drawString(Vel, 0, 200, GFXFF);
      //  Serial.println(Vol + "V " + Amp + "A " + Vel + "%");
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }


  if (VolF != VolAnterior) {
    VolAnterior = VolF;

    listVol.RemoveFirst();
    listAmp.RemoveFirst();
    listVel.RemoveFirst();

    listVol.Add(VolF);
    listAmp.Add(AmpF);
    listVel.Add(VelF);

    M5.Lcd.fillRect(165, 185, 150, 50, BLACK);

    for (int i = 0; i < totalList; i++)
    {
      int punto_x;
      int punto_y_Vol ;
      int punto_y_Amp ;
      int punto_y_Vel ;


      punto_x = 320 - totalList - 5 + i;
      punto_y_Vol = 240 - 5 - listVol[i] * 5;
      punto_y_Amp = 240 - 5 - listAmp[i] * 5;
      punto_y_Vel = 240 - 5 - listVel[i] * 0.5;
      M5.Lcd.drawPixel(punto_x, punto_y_Vol, WHITE);
      M5.Lcd.drawPixel(punto_x, punto_y_Amp, RED);
      M5.Lcd.drawPixel(punto_x, punto_y_Vel, BLUE);

      Serial.print(punto_y_Vol);
      Serial.print(",");
    }
    Serial.println(totalList);


  }
  //delay(100);
}


String floatToString( float n, int l, int d, boolean z) {
  char c[l + 1];
  String s;

  dtostrf(n, l, d, c);
  s = String(c);

  if (z) {
    s.replace(" ", "0");
  }

  return s;
}


String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
