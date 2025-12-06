#include <Arduino.h>
#include <BleKeyboard.h>
// Enable NIMBLE mode to save power?
// Probably not neccessary, this is already Bluetooth LE though NIMBLE may reduce memory overhead

BleKeyboard bleKeyboard("ESP32 BLE Pedal", "Me", 100);

const int nextButtonPin = 4; // GPIO pin for the next page button
const int prevButtonPin = 2; // GPIO pin for the previous page button - optional
bool nextButtonState = false; // Tracks button state
bool prevButtonState = false; 
bool nextLastButtonState = false;
bool prevLastButtonState = false;

void setup() {
    //Serial.begin(115200);
    pinMode(nextButtonPin, INPUT_PULLUP);
    pinMode(prevButtonPin, INPUT_PULLUP);
    bleKeyboard.begin();
}

void loop() {
    if (bleKeyboard.isConnected()) {
        nextButtonState = digitalRead(nextButtonPin); // Read the button state
        prevButtonState = digitalRead(prevButtonPin); // Read the button state

        // Check if button is pressed (LOW due to pull-up) and handle state change for "next" button
        if (nextButtonState == LOW && nextLastButtonState == HIGH) {
            //Serial.println("Button pressed, typing 'H'");
            //bleKeyboard.print("H"); // Send the character 'H'
            bleKeyboard.write(KEY_RIGHT_ARROW);
            //bleKeyboard.press(KEY_RIGHT_ARROW)
            //bleKeyboard.release(KEY_RIGHT_ARROW);
            delay(50);              // Debounce delay
        }
        nextLastButtonState = nextButtonState; // Update last button state

        // Check if button is pressed (LOW due to pull-up) and handle state change for "prev" button
        if (prevButtonState == LOW && prevLastButtonState == HIGH) {
            //Serial.println("Button pressed, typing 'H'");
            //bleKeyboard.print("H"); // Send the character 'H'
            bleKeyboard.write(KEY_RIGHT_ARROW);
            //bleKeyboard.press(KEY_RIGHT_ARROW)
            //bleKeyboard.release(KEY_RIGHT_ARROW);
            delay(50);              // Debounce delay
        }
        prevLastButtonState = prevButtonState; // Update last button state

    }
    delay(10);
}
