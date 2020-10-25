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
#include "displayM5.h"

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char password[] = SECRET_PASS;

List<float> listVol;
List<float> listVel;
List<float> listAmp;

int totalList = 150; // longitud del array de valores para gráfica

const char* serverNameDatos = "http://192.168.4.1/datos";
const char* serverNameVol = "http://192.168.4.1/voltaje";
const char* serverNameAmp = "http://192.168.4.1/amp";
const char* serverNameVel = "http://192.168.4.1/velocity";

String Vol;
String Amp;
String Vel;
String Datos;

float VolF;
float VolAnterior;
float AmpF;
int VelF;

int pantalla = 1;

unsigned long previousMillis = 0;
const long interval = 200;

unsigned long previousMillis1 = 0;

int tipografia = 7;
float aleatorio = 0;
String texto = "inicio";
#define GFXFF 1
#define FF24 &FreeSansBold24pt7b
#define FF12 &FreeSansBold12pt7b

String floatToString( float, int = 8, int = 2, boolean = false);

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


  pantalla1_inicio(); //=== Dibuja pantalla de interface


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
      //  Vol = httpGETRequest(serverNameVol);
      // Amp = httpGETRequest(serverNameAmp);
      //  Vel = httpGETRequest(serverNameVel);

      Datos = httpGETRequest(serverNameDatos);

      int n = sscanf(Datos.c_str(), "%3s:%3s:%3s", &Vol, &Amp, &Vel);

      VolF = Vol.toFloat() / 10.0;
      AmpF = Amp.toFloat() / 10.0;
      VelF = Vel.toFloat() * 1.0;




      Serial.print("Vol: ");
      Serial.print(String(VolF));
      Serial.print("v Amp: ");
      Serial.print(String(AmpF));
      Serial.print("A Vel: ");
      Serial.print(String(VelF));
      Serial.println("%");

      Serial.println(Datos);





      //=========== actualizar array si hay nueva lectura ===================
      if (VolF != VolAnterior) {
        VolAnterior = VolF;

        listVol.RemoveFirst();
        listAmp.RemoveFirst();
        listVel.RemoveFirst();

        listVol.Add(VolF);
        listAmp.Add(AmpF);
        listVel.Add(VelF);


        VolF = mapfloat(VolF, 0.0, 100.0, 0.0, 25.0);
        AmpF = mapfloat(AmpF, 0.0, 100.0, 0.0, 6.0);
        int posY;

        if (pantalla == 1) {
          //======= Dibujar numeros =================
          M5.Lcd.setTextColor(TFT_BLACK, TFT_MAGENTA);
          M5.Lcd.setFreeFont(FF24);
          M5.Lcd.fillRect(165, 50, 155, 100, TFT_MAGENTA);
          if (VolF >= 10) {
            posY = 165;
          } else {
            posY = 191;
          }
          M5.Lcd.drawString( String(VolF).substring(0, String(VolF).length() - 1), posY, 50, GFXFF);
          if (AmpF >= 10) {
            posY = 165;
          } else {
            posY = 191;
          }
          M5.Lcd.drawString(String(AmpF).substring(0, String(AmpF).length() - 1), posY, 90, GFXFF);
          M5.Lcd.fillRect(270, 10, 320, 30, TFT_MAGENTA);
          M5.Lcd.setFreeFont(FF12);
          M5.Lcd.drawString(String(VelF) + "% ", 270, 10, GFXFF);



          //=========== dibujar grafica ===================
          M5.Lcd.fillRect(164, 184, 152, 52, BLACK);

          for (int i = 0; i < totalList; i++)
          {
            int punto_x = 320 - totalList - 5 + i;
            int punto_y_Vol = 240 - 5 - listVol[i] * 0.5;
            int punto_y_Vol1 = 240 - 5 - listVol[i - 1] * 0.5;
            int punto_y_Amp = 240 - 5 - listAmp[i] * 0.5;
            int punto_y_Amp1 = 240 - 5 - listAmp[i - 1] * 0.5;
            int punto_y_Vel = 240 - 5 - listVel[i] * 0.5;
            M5.Lcd.drawLine(punto_x, punto_y_Vel, punto_x, 235, 0x7bef);
            M5.Lcd.drawLine(punto_x - 1, punto_y_Amp, punto_x, punto_y_Amp1, RED);
            M5.Lcd.drawLine(punto_x - 1, punto_y_Vol, punto_x, punto_y_Vol1, BLUE);

          }
        }

        previousMillis = currentMillis;
      }

    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }



  if (M5.BtnA.wasPressed()) {
    M5.powerOFF();
  }

  if (M5.BtnC.wasPressed()) {
    if (pantalla == 1) {
      pantalla = 0;
      pantalla2_inicio();
    } else {
      pantalla = 1;
      pantalla1_inicio();
    }

  }

  M5.update();

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




float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
