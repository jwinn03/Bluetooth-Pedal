#include <Arduino.h>
#include <BleKeyboard.h>
#include <driver/rtc_io.h>

#define ENABLE_SERIAL_DEBUG
#define SLEEP_TIMEOUT_MS (5 * 60 * 1000) // 5 minutes

// Enable NIMBLE mode to save power?
// Probably not necessary, this is already Bluetooth LE though NIMBLE may reduce memory overhead

BleKeyboard bleKeyboard("ESP32 BLE Pedal", "Me", 100);

const int fwdButtonPin = 4; // GPIO pin for the fwd page button
const int bckButtonPin = 2; // GPIO pin for the back page button - optional
bool fwdButtonState = false; // Tracks button state
bool bckButtonState = false; 
bool fwdLastButtonState = HIGH;
bool bckLastButtonState = HIGH;

unsigned long lastActivityTime = 0;

void setup() {
    #ifdef ENABLE_SERIAL_DEBUG
    Serial.begin(115200);
    #endif
    pinMode(fwdButtonPin, INPUT_PULLUP);
    pinMode(bckButtonPin, INPUT_PULLUP);
    bleKeyboard.begin();

    lastActivityTime = millis();
}

void loop() {
    fwdButtonState = digitalRead(fwdButtonPin); // Read the button state
    bckButtonState = digitalRead(bckButtonPin); 
    bool activityDetected = false;

    // Check if button is pressed (LOW due to pull-up) and handle state change for "fwd" button
    if (fwdButtonState == LOW && fwdLastButtonState == HIGH) {
        activityDetected = true;
        if (bleKeyboard.isConnected()) {
            #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("Next button pressed");
            #endif
            bleKeyboard.write(KEY_RIGHT_ARROW);
            delay(50);              // Debounce delay
        }
    }
    fwdLastButtonState = fwdButtonState; // Update last button state

    // Check if button is pressed (LOW due to pull-up) and handle state change for "bck" button
    if (bckButtonState == LOW && bckLastButtonState == HIGH) {
        activityDetected = true;
        if (bleKeyboard.isConnected()) {
            #ifdef ENABLE_SERIAL_DEBUG
            Serial.println("Prev button pressed");
            #endif
            bleKeyboard.write(KEY_LEFT_ARROW);
            delay(50);              // Debounce delay
        }
    }
    bckLastButtonState = bckButtonState; // Update last button state

    if (activityDetected) {
        lastActivityTime = millis();
    }

    // Check for inactivity
    if (millis() - lastActivityTime > SLEEP_TIMEOUT_MS) {
        #ifdef ENABLE_SERIAL_DEBUG
        Serial.println("Inactivity timeout. Going to sleep...");
        Serial.flush();
        #endif

        bleKeyboard.end(); // Stop BLE

        // Configure internal pullups for sleep
        // We must keep RTC_PERIPH on to sustain the internal pullups during sleep
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

        rtc_gpio_pullup_en((gpio_num_t)fwdButtonPin);
        rtc_gpio_pulldown_dis((gpio_num_t)fwdButtonPin);
        rtc_gpio_pullup_en((gpio_num_t)bckButtonPin);
        rtc_gpio_pulldown_dis((gpio_num_t)bckButtonPin);

        // Configure wakeup sources:
        // ext0 for fwdButtonPin (Pin 4) - Wake on LOW (0)
        esp_sleep_enable_ext0_wakeup((gpio_num_t)fwdButtonPin, 0);

        // ext1 for bckButtonPin (Pin 2) - Wake on LOW
        // ext1 normally supports ALL_LOW or ANY_HIGH.
        // By using a mask with ONLY Pin 2, ALL_LOW effectively acts as "Pin 2 is Low"
        esp_sleep_enable_ext1_wakeup(1ULL << bckButtonPin, ESP_EXT1_WAKEUP_ALL_LOW);

        esp_deep_sleep_start();
    }

    delay(10);
}
