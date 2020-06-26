/*
  photogram server
*/

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <AccelStepper.h>

#define STEP_PIN 5
#define DIR_PIN 4

AsyncWebServer server(80);
DNSServer dns;
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

unsigned long lastUpdate = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.println("");

  // initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);

  // Setup WIFIManager
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("photogram_SETUP", "photogram");

  // Setup MDNS
  if (MDNS.begin("photogram")) {
    Serial.println("MDNS responder started");
  }

  // Setup web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.onNotFound(handleNotFound);
  server.begin();

  // Setup stepper
  stepper.setMaxSpeed(2000);
  stepper.setSpeed(2000);
  stepper.setAcceleration(2000);

  Serial.println("HTTP server started");
}

// the loop function runs over and over again forever
void loop() {
  MDNS.update();
  stepper.run();
}

void doMove(long deg, long dir) {
  int fullRevolutionSteps = 3200;
  int steps = (deg / (float)360) * fullRevolutionSteps;

  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);    // turn the LED off by making the voltage LOW
    delayMicroseconds(1000);
  }
}
void moveOneStep() {
  digitalWrite(STEP_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(10);
  digitalWrite(STEP_PIN, LOW);    // turn the LED off by making the voltage LOW
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "I'm alive!! \n\n" + String(random(1000)));
}

void handleMove(AsyncWebServerRequest *request) {
  long degParam = 0;
  if (request->hasParam("deg")) {
    AsyncWebParameter* p = request->getParam("deg");
    degParam = String(p->value().c_str()).toInt();
    if (degParam == 0 || degParam > 720 || degParam < -720) {
      request->send(400, "text/plain", "invalid deg param, got: \"" + String(p->value().c_str()) + "\"[" + degParam + "]");
      return;
    }
  }

  int steps = (degParam / (float)360) * 3200;
  stepper.move(steps);
  
  String message = "success \ndeg: " + String(degParam);
  request->send("text/plain", message.length(), [message](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
    if(stepper.isRunning()) {
      return 0;
    }
    memcpy(buffer, message.c_str(), message.length());
    return message.length();
  });
}

void handleNotFound(AsyncWebServerRequest *request) {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: \n";
  int params = request->params();
  for (int i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    message += String(sprintf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str()));
  }
  request->send(404, "text/plain", message);
}
