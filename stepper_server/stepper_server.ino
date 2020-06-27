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
#include <AsyncElegantOTA.h>

#define STEP_PIN 5
#define DIR_PIN 4

String indexHTML = "<html lang=\"en\"> <head> <meta charset=\"utf-8\"> <title>photogram.local</title> <meta name=\"description\" content=\"photogram.local web interface\"> </head> <body> <div style=\"padding: 20px; display: flex; justify-content: center; align-items: center; flex-direction: column; font-family: Helvetica, Arial, sans-serif;\"> <h1> photogram.local </h1> <table> <tr> <td> <button onClick=\"window.move(-2 * getDeg())\"> &lt;&lt; </button> <button onClick=\"window.move(-1 * getDeg())\"> &lt; </button> </td> <td> <form onSubmit=\"return false;\" style=\"margin: 0;\"> <input id=\"deg\" type=\"text\" value=\"15\" size=\"3\" style=\"text-align:center;\" /> </form> </td> <td> <button onClick=\"window.move(getDeg())\"> &gt; </button> <button onClick=\"window.move(2 * getDeg())\"> &gt;&gt; </button> </td> </tr> </table> </div> <script> window.getDeg = function() { return parseInt(document.querySelector(\'#deg\').value, 10); }; window.move = function(deg) { fetch(\'/move?deg=\' + window.encodeURIComponent(deg)); }; </script> </body> </html> ";
AsyncWebServer server(80);
DNSServer dns;
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", indexHTML);
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

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Serial.println("");

  // initialize pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW);

  // Setup WIFIManager
  WiFi.hostname("photogram-local");
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("photogram_SETUP", "photogram");

  // Setup MDNS
  if (MDNS.begin("photogram")) {
    MDNS.addService("http", "tcp", 80);
  }

  // Setup web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  
  // Start elegant OTA
  AsyncElegantOTA.begin(&server);
  
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
  AsyncElegantOTA.loop();
  MDNS.update();
  stepper.run();
}
