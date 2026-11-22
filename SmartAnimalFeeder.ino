#include <Servo.h>
#include "FeederController.h"

// Arduino Uno reconstruction: timed servo gate, button and Serial control.
const uint8_t SERVO_PIN = 9;
const uint8_t BUTTON_PIN = 2;
const uint8_t LED_PIN = LED_BUILTIN;
const uint8_t CLOSED_ANGLE = 0;
const uint8_t OPEN_ANGLE = 70;
const uint32_t FEED_INTERVAL_MS = 6UL * 60UL * 60UL * 1000UL;
const uint32_t GATE_OPEN_MS = 700UL;
const uint32_t COOLDOWN_MS = 60UL * 1000UL;
const uint32_t DEBOUNCE_MS = 40UL;

Servo gate;
FeederController feeder(FEED_INTERVAL_MS, GATE_OPEN_MS, COOLDOWN_MS);
int lastReading = HIGH;
int stableButton = HIGH;
uint32_t changedAt = 0;
bool previousOpen = false;

void printStatus() {
  Serial.print(F("Gate: "));
  Serial.print(feeder.isOpen() ? F("OPEN") : F("CLOSED"));
  Serial.print(F(" | feeds since restart: "));
  Serial.println(feeder.feedCount());
}

void requestManualFeed(uint32_t now) {
  if (feeder.requestFeed(now)) Serial.println(F("Manual feed accepted."));
  else Serial.println(F("Feed blocked: gate active or cooldown."));
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  gate.write(CLOSED_ANGLE);
  gate.attach(SERVO_PIN);
  feeder.begin(millis());
  Serial.println(F("Smart feeder ready. f = feed, s = status."));
  Serial.println(F("Automatic feed in 6 hours. No feed on startup."));
}

void loop() {
  const uint32_t now = millis();
  const int reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastReading = reading;
    changedAt = now;
  }
  if (uint32_t(now - changedAt) >= DEBOUNCE_MS && reading != stableButton) {
    stableButton = reading;
    if (stableButton == LOW) requestManualFeed(now);
  }

  // Bound work per loop so a continuous serial stream cannot delay gate closure.
  if (Serial.available() > 0) {
    const char command = Serial.read();
    if (command == 'f' || command == 'F') requestManualFeed(now);
    else if (command == 's' || command == 'S') printStatus();
  }

  feeder.update(now);
  const bool open = feeder.isOpen();
  if (open != previousOpen) {
    gate.write(open ? OPEN_ANGLE : CLOSED_ANGLE);
    digitalWrite(LED_PIN, open ? HIGH : LOW);
    Serial.println(open ? F("Gate opened.") : F("Gate closed."));
    previousOpen = open;
  }
}
