// 12 Mar 2014
// this works with ComArduino.py and ComArduinoA4e.rb
// this version uses a start marker 254 and an end marker of 255
//  it uses 253 as a special byte to be able to reproduce 253, 254 and 255
// it also sends data to the PC using the same system
//   if the number of bytes is 0 the PC will assume a debug string and just print it to the screen


#include <Packetizer.h>

//================

#define R_PIN 11
#define G_PIN 6
#define B_PIN 3

byte outputPins[3] = {R_PIN, G_PIN, B_PIN};


boolean inProgress = false;
boolean startFound = false;
boolean allReceived = false;

//================

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // for debugging
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
  Serial.begin(115200);

  // register callback called if packet has come
  Packetizer::subscribe(Serial,
                        [&](const uint8_t* data, const size_t size)
  {
    processData(data, size);
  }
                       );

  blinkLED(20); // just so we know it's alive
}

//================

void loop() {
  Packetizer::parse();
}

//============================

void processData(uint8_t * data, size_t len) {
  if (len == 1 && data[0] == 255) {
    Packetizer::send(Serial, data, len);
    blinkLED(5);
    return;
  }

  Packetizer::send(Serial, data, len);
}

//============================
void blinkLED(byte numBlinks) {
  for (byte n = 0; n < numBlinks; n ++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
  }
}
