#include <BleKeyboard.h>
// Enable NIMBLE mode to save power?
// Probably not neccessary, this is already Bluetooth LE though NIMBLE may reduce memory overhead

BleKeyboard bleKeyboard("ESP32 BLE Pedal", "Me", 100);

const int buttonPin = 4; // GPIO pin for the button
bool buttonState = false; // Tracks button state
bool lastButtonState = false;

void setup() {
    //Serial.begin(115200);
    pinMode(buttonPin, INPUT_PULLUP);
    bleKeyboard.begin();
}

void loop() {
    if (bleKeyboard.isConnected()) {
        buttonState = digitalRead(buttonPin); // Read the button state

        // Check if button is pressed (LOW due to pull-up) and handle state change
        if (buttonState == LOW && lastButtonState == HIGH) {
            //Serial.println("Button pressed, typing 'H'");
            //bleKeyboard.print("H"); // Send the character 'H'
            bleKeyboard.write(KEY_RIGHT_ARROW);
            //bleKeyboard.press(KEY_RIGHT_ARROW)
            //bleKeyboard.release(KEY_RIGHT_ARROW);
            delay(50);              // Debounce delay
        }

        lastButtonState = buttonState; // Update last button state
    }
    delay(10);
}
