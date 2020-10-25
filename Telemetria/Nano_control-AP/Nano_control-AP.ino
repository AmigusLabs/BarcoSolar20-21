/*
  WiFi Web Server LED Blink

  A simple web server that lets you blink an LED via the web.
  This sketch will create a new access point (with no password).
  It will then launch a new server and print out the IP address
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 13.

  If the IP address of your board is yourAddress:
    http://yourAddress/H turns the LED on
    http://yourAddress/L turns it off

  created 25 Nov 2012
  by Tom Igoe
  adapted to WiFi AP by Adafruit
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"


#define RECEPTOR_CH3_PIN A1 // Canal 3 de velocidad
#define RECEPTOR_CH6_PIN A2 // Canal 6 de Auto/Manual
#define SALIDA_ESC_PIN 10

unsigned long timeout_pwm_micros = 50000;

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

unsigned long tiempo;
float Vol;
float Amp;
int Vel;


float voltage_SuperCap = 0;


void setup() {
  //Initialize serial and wait for port to open:

  analogReference(AR_INTERNAL1V65);


  pinMode(RECEPTOR_CH3_PIN, INPUT);
  pinMode(RECEPTOR_CH6_PIN, INPUT);


    
  Serial.begin(9600);
  /*  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }*/

  Serial.println("Access Point Web Server");

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address of will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}










void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
  tiempo = millis();



// demo datos
  Vol = 5 + sin(tiempo / PI / 1000 + 500) * 5.0;
  int Vol1 = Vol * 100;
  Amp = 5 + sin(tiempo / PI / 500 + 750) * 5.0;
  int Amp1 = Amp * 100;
  Vel = 50 + sin(tiempo / PI / 2000) * 50.0;
  int Vel1 = Vel;




  // Voltaje SuperCap

  float voltage_SuperCap = leerVoltaje();


 



  Serial.println(Vel);

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:

          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":

        if (currentLine.endsWith("GET /datos")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          if (Vol1 < 100) {
            client.print("0");
          }
          if (Vol1 < 10) {
            client.print("0");
          }
          client.print(Vol1);
          client.print(":");

          if (Amp1 < 100) {
            client.print("0");
          }
          if (Amp1 < 10) {
            client.print("0");
          }
          client.print(Amp1);
          client.print(":");
          
          if (Vel1 < 100) {
            client.print("0");
          }
          if (Vel1 < 10) {
            client.print("0");
          }
          client.print(Vel1);
          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;            // GET /H turns the LED on
        }


        if (currentLine.endsWith("GET /voltaje")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:

          client.print(Vol1);

          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;            // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /amp")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(Amp1);

          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;              // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /velocity")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print(Vel1);

          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;              // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println();
    Serial.println("client disconnected");
  }


}








bool switchActivado()
{
  unsigned long value1 = pulseIn(RECEPTOR_CH6_PIN, HIGH, timeout_pwm_micros);
  unsigned long value2 = pulseIn(RECEPTOR_CH6_PIN, HIGH, timeout_pwm_micros);
  if ((value1 > 1600) && (value2 > 1600))
  {
    return false;
  }
  return true;
}





void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
