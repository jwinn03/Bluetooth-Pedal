#include <Arduino.h>

// Define the GPIO pin for the built-in LED.
// On many ESP32 boards, this is GPIO 2.
#define LED_PIN 13

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Set the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Hello from ESP32!");
}

void loop() {
  // Turn the LED on
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED is ON");
  delay(1000); // Wait for 1 second

  // Turn the LED off
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED is OFF");
  delay(1000); // Wait for 1 second
}