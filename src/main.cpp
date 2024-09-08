#include <Arduino.h>
#include <cstdint>
#include <atomic>

#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"

constexpr uint8_t emergencyButtonPin = 18;
constexpr uint8_t emergencyButtonLedPin = 19;

constexpr uint8_t nextSoundButtonPin = 13;
constexpr uint8_t nextSoundButtonLedPin = 2;

constexpr uint8_t previousSoundButtonPin = 33;
constexpr uint8_t previousSoundButtonLedPin = 12;

constexpr uint8_t potentiometerPin = 32;
constexpr byte motorPort = static_cast<byte>(DuploTrainHubPort::MOTOR);

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);

    // Configure the button pin as input with internal pull-up resistor
    ::pinMode(::emergencyButtonPin, INPUT_PULLUP);
    ::pinMode(::nextSoundButtonPin, INPUT_PULLUP);
    ::pinMode(::previousSoundButtonPin, INPUT_PULLUP);

    // Configure the LED pin as output
    ::pinMode(::emergencyButtonLedPin, OUTPUT);
    ::pinMode(::nextSoundButtonLedPin, OUTPUT);
    ::pinMode(::previousSoundButtonLedPin, OUTPUT);

    // Ensure the LED is off initially
    ::digitalWrite(::emergencyButtonLedPin, LOW);
    ::digitalWrite(::nextSoundButtonLedPin, LOW);
    ::digitalWrite(::previousSoundButtonLedPin, LOW);
}

void loop() {
    static Lpf2Hub myTrainHub;
    static std::atomic<bool> sendingOrderFlag(false);

    if ((!myTrainHub.isConnected()) && (!myTrainHub.isConnecting())) {
        // initialize the PoweredUpHub instance
        myTrainHub.init();
    }

    // connect flow. Search for BLE services and try to connect if the uuid of the hub is found
    if (myTrainHub.isConnecting()) {
        myTrainHub.connectHub();
        if (myTrainHub.isConnected()) {
            handlePostConnection(&myTrainHub);
        } else {
            Serial.println("Failed to connect to HUB");
        }
    }

    // if connected, you can set the name of the hub, the LED color and shut it down
    if (myTrainHub.isConnected()) {
        const auto positionSpeed = static_cast<int16_t>(::analogRead(::potentiometerPin));
        const uint8_t emergencyButtonState = ::digitalRead(::emergencyButtonPin);
        ::controlMotor(&myTrainHub, positionSpeed, emergencyButtonState, ::emergencyButtonLedPin,
                       ::motorPort, sendingOrderFlag);

        const uint8_t nextSoundButtonState = ::digitalRead(::nextSoundButtonPin);
        const uint8_t previousSoundButtonState = ::digitalRead(::previousSoundButtonPin);
        ::controlSounds(&myTrainHub, nextSoundButtonState, ::nextSoundButtonLedPin,
                        previousSoundButtonState, ::previousSoundButtonLedPin, sendingOrderFlag);
    } else {
        Serial.println("Train hub is disconnected");
    }
}
