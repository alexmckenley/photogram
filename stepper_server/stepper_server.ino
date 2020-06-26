/*
  photogram server
*/

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>

#define STEP_PIN 5
#define DIR_PIN 4

ESP8266WebServer server(80);

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.println("");

  // initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);

  // Setup WIFIManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("photogram_SETUP", "photogram");

  // Setup MDNS
  if (MDNS.begin("photogram")) {
    Serial.println("MDNS responder started");
  }

  // Setup web server
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();
  MDNS.update();
}

void doMove(long deg, long dir) {
  for (int i = 0; i < 500; i++) {
    digitalWrite(STEP_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);    // turn the LED off by making the voltage LOW
    delayMicroseconds(1000);
  }
}

void handleRoot() {
  doMove(200, 200);

  server.send(200, "text/plain", "I'm alive!! \n\n" + String(random(1000)));
}

void handleMove() {
  long degParam = 0;
  long dirParam = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "deg") {
      degParam = String(server.arg(i)).toInt();
      if (degParam == 0) {
        server.send(400, "text/plain", "invalid deg param, got: \"" + server.arg(i) + "\"");
        return;
      }
    }
    if (server.argName(i) == "dir") {
      dirParam = String(server.arg(i)).toInt();
      if (dirParam == 0) {
        server.send(400, "text/plain", "invalid dir param, got: \"" + server.arg(i) + "\"");
        return;
      }
    }
  }
  if (degParam == 0 || dirParam == 0) {
    String message = "Invalid input\n\n";
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
    server.send(400, "text/plain", message);
    return;
  }
  doMove(degParam, dirParam);
  server.send(200, "text/plain", "success");
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
