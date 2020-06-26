// 12 Mar 2014
// this works with ComArduino.py and ComArduinoA4e.rb
// this version uses a start marker 254 and an end marker of 255
//  it uses 253 as a special byte to be able to reproduce 253, 254 and 255
// it also sends data to the PC using the same system
//   if the number of bytes is 0 the PC will assume a debug string and just print it to the screen


#include <Packetizer.h>

//================

#define DIR_PIN 7
#define STEP_PIN 8

//================
bool dirHigh;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // for debugging
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);

  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(STEP_PIN, HIGH);


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

  // Toggle the DIR pin to change direction.
  if (dirHigh)
  {
    dirHigh = false;
    digitalWrite(DIR_PIN, LOW);
  }
  else
  {
    dirHigh = true;
    digitalWrite(DIR_PIN, HIGH);
  }

  // Step the motor 50 times before changing direction again.
  for (int i = 0; i < 12000; i++)
  {
    // Trigger the motor to take one step.
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(100000000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(100000000);
    
  }

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
