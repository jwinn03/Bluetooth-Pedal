#include <Arduino.h>
#include <BleKeyboard.h>

#define ENABLE_SERIAL_DEBUG

// Enable NIMBLE mode to save power?
// Probably not necessary, this is already Bluetooth LE though NIMBLE may reduce memory overhead

BleKeyboard bleKeyboard("ESP32 BLE Pedal", "Me", 100);

const int fwdButtonPin = 4; // GPIO pin for the fwd page button
const int bckButtonPin = 2; // GPIO pin for the back page button - optional
bool fwdButtonState = false; // Tracks button state
bool bckButtonState = false; 
bool fwdLastButtonState = false;
bool bckLastButtonState = false;

void setup() {
    #ifdef ENABLE_SERIAL_DEBUG
    Serial.begin(115200);
    #endif
    pinMode(fwdButtonPin, INPUT_PULLUP);
    pinMode(bckButtonPin, INPUT_PULLUP);
    bleKeyboard.begin();
}

void loop() {
    if (bleKeyboard.isConnected()) {
        fwdButtonState = digitalRead(fwdButtonPin); // Read the button state
        bckButtonState = digitalRead(bckButtonPin); 

        // Check if button is pressed (LOW due to pull-up) and handle state change for "fwd" button
        if (fwdButtonState == LOW && fwdLastButtonState == HIGH) {
            #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("Next button pressed");
            #endif
            bleKeyboard.write(KEY_RIGHT_ARROW);
            //bleKeyboard.press(KEY_RIGHT_ARROW)
            //bleKeyboard.release(KEY_RIGHT_ARROW);
            delay(50);              // Debounce delay
        }
        fwdLastButtonState = fwdButtonState; // Update last button state

        // Check if button is pressed (LOW due to pull-up) and handle state change for "bck" button
        if (bckButtonState == LOW && bckLastButtonState == HIGH) {
            #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("Prev button pressed");
            #endif
            //bleKeyboard.print("H"); // Send the character 'H'
            bleKeyboard.write(KEY_LEFT_ARROW);
            //bleKeyboard.press(KEY_LEFT_ARROW)
            //bleKeyboard.release(KEY_LEFT_ARROW);
            delay(50);              // Debounce delay
        }
        bckLastButtonState = bckButtonState; // Update last button state

    }
    delay(10);
}
