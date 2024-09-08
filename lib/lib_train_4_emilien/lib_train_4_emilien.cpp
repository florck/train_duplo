//
// Created by Florian Thöni on 07/09/2024.
//
#include <cstdint>
#include <cmath>
#include <Arduino.h>

#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"
#include "lib_train_4_emilien_native.h"


constexpr std::array<byte, 5> idToSound{
    static_cast<byte>(DuploTrainBaseSound::BRAKE),
    static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE),
    static_cast<byte>(DuploTrainBaseSound::WATER_REFILL),
    static_cast<byte>(DuploTrainBaseSound::HORN),
    static_cast<byte>(DuploTrainBaseSound::STEAM)
};


void controlSounds(Lpf2Hub *myTrainHub, const uint8_t nextSoundButtonState,
                   const uint8_t nextSoundButtonLedPin, uint8_t previousSoundButtonState,
                   uint8_t previousSoundButtonLedPin, std::atomic<bool> &sendingOrderFlag) {
    //This is a loop ready function that uses static to navigate between the states.
    static uint8_t currentSound = 0;
    static bool currentlyPlaying = false;
    static bool notYetPlayed = false;
    static unsigned long playStartTime = 0.;
    static uint8_t actionedButtonLedPin = 0;
    bool asChanged = false;
    if ((nextSoundButtonState == LOW) && (!currentlyPlaying) && (!notYetPlayed)) {
        Serial.println("Detected a click on next.");
        asChanged = true;
        ++currentSound;
        if (currentSound > 4) {
            currentSound = 0;
        }
        actionedButtonLedPin = nextSoundButtonLedPin;
    }
    if ((previousSoundButtonState == LOW) && (!currentlyPlaying) && (!notYetPlayed)) {
        Serial.println("Detected a click on previous.");
        asChanged = true;
        --currentSound;
        if (currentSound < 0) {
            currentSound = 4;
        }
        actionedButtonLedPin = previousSoundButtonLedPin;
    }
    if (asChanged) {
        currentlyPlaying = true;
        notYetPlayed = true;
        playStartTime = ::millis();
        ::digitalWrite(actionedButtonLedPin, HIGH);

        while (sendingOrderFlag.load()) {
            Serial.println("Waiting for the flag to be ready.");
        }
        sendingOrderFlag.store(true);
        Serial.println(currentSound);
        byte setToneMode[8] = {0x41, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01};
        myTrainHub->WriteValue(setToneMode, 8);
        Serial.println("Start unable period");
    }
    if (currentlyPlaying && notYetPlayed && ((::millis() - playStartTime) >= 200)) {
        notYetPlayed = false;
        playStartTime = ::millis();
        Serial.println("Play the actual sound.");
        byte playSound[6] = {0x81, 0x01, 0x11, 0x51, 0x01, idToSound[currentSound]};
        myTrainHub->WriteValue(playSound, 6);
    }
    if (currentlyPlaying && (!notYetPlayed) && ((::millis() - playStartTime) >= 200) &&
        sendingOrderFlag.load()) {
        sendingOrderFlag.store(false);
    }
    if (currentlyPlaying && (!notYetPlayed) && ((::millis() - playStartTime) >= 1000)) {
        Serial.println("End unable period.");
        ::digitalWrite(actionedButtonLedPin, LOW);
        currentlyPlaying = false;
    }
}

void controlMotor(Lpf2Hub *myTrainHub, const int16_t positionSpeed,
                  const uint8_t emergencyButtonState, const uint8_t emergencyButtonLedPin,
                  const byte motorPort, std::atomic<bool> &sendingOrderFlag) {
    static int16_t currentSpeed = 2048;
    static bool emergencyStop = true;

    if (!sendingOrderFlag.load()) {
        sendingOrderFlag.store(true);
        const int16_t actualSpeed = ::convertPotPositionToSpeed(positionSpeed);

        if (emergencyStop && (actualSpeed == 0)) {
            emergencyStop = false;
        }
        if ((!emergencyStop) && (!((currentSpeed == 0) && (actualSpeed == 0)))) {
            currentSpeed = actualSpeed;
            myTrainHub->setBasicMotorSpeed(motorPort, actualSpeed);
        }
        if (emergencyButtonState == LOW) {
            // Print message to the serial monitor
            Serial.println("Button pressed!");
            ::digitalWrite(emergencyButtonLedPin, HIGH);
            Serial.println("Button pressed! LED on.");

            emergencyStop = true;

            ::digitalWrite(emergencyButtonLedPin, LOW);
        }

        if (emergencyStop) {
            ::digitalWrite(emergencyButtonLedPin, HIGH);
        } else {
            ::digitalWrite(emergencyButtonLedPin, LOW);
        }
        sendingOrderFlag.store(false);
    }
}

void handlePostConnection(Lpf2Hub *myTrainHub) {
    Serial.println("Connected to HUB");
    Serial.print("Hub address: ");
    Serial.println(myTrainHub->getHubAddress().toString().c_str());
    Serial.print("Hub name: ");
    Serial.println(myTrainHub->getHubName().c_str());
    myTrainHub->activateHubPropertyUpdate(
        HubPropertyReference::BATTERY_VOLTAGE,
        hubPropertyChangeCallback);
}

void hubPropertyChangeCallback(void *hub, HubPropertyReference hubProperty, uint8_t *pData) {
    auto *myHub = static_cast<Lpf2Hub *>(hub);
    Serial.print("HubProperty: ");
    Serial.println(static_cast<byte>(hubProperty), HEX);

    if (hubProperty == HubPropertyReference::RSSI) {
        Serial.print("RSSI: ");
        Serial.println(myHub->parseRssi(pData), DEC);
    }

    if (hubProperty == HubPropertyReference::BATTERY_VOLTAGE) {
        Serial.print("BatteryLevel: ");
        Serial.println(myHub->parseBatteryLevel(pData), DEC);
    }

    if (hubProperty == HubPropertyReference::BUTTON) {
        Serial.print("Button: ");
        Serial.println(static_cast<byte>(myHub->parseHubButton(pData)), HEX);
    }
}

void playAllTones(Lpf2Hub *myTrainHub) {
    ::delay(1000);
    myTrainHub->playTone(0x0a);
    Serial.println(0x0a);
    ::delay(1000);
    myTrainHub->playTone(0x0a);
    Serial.println(0x0a);
    ::delay(1000);
    myTrainHub->playTone(0x09);
    Serial.println(0X09);
    ::delay(1000);
    myTrainHub->playTone(0x09);
    Serial.println(0X09);
    ::delay(1000);
    myTrainHub->playTone(0x08);
    Serial.println(static_cast<byte>(8));
    ::delay(1000);
    myTrainHub->playTone(0x08);
    Serial.println(static_cast<byte>(8));
    ::delay(1000);
    myTrainHub->playTone(0x07);
    Serial.println(0x07);
    ::delay(1000);
    myTrainHub->playTone(0x07);
    Serial.println(0x07);
    ::delay(1000);
    myTrainHub->playTone(0x06);
    Serial.println(0x06);
    ::delay(1000);
    myTrainHub->playTone(0x06);
    Serial.println(0x06);
    ::delay(1000);
    myTrainHub->playTone(0x05);
    Serial.println(0x05);
    ::delay(1000);
    myTrainHub->playTone(0x05);
    Serial.println(0x05);
    ::delay(1000);
    myTrainHub->playTone(0x04);
    Serial.println(0x04);
    ::delay(1000);
    myTrainHub->playTone(0x04);
    Serial.println(0x04);
    ::delay(1000);
    myTrainHub->playTone(0x03);
    Serial.println(0x03);
    ::delay(1000);
    myTrainHub->playTone(0x03);
    Serial.println(0x03);
    ::delay(1000);
    myTrainHub->playTone(0x02);
    Serial.println(0x02);
    ::delay(1000);
    myTrainHub->playTone(0x02);
    Serial.println(0x02);
    ::delay(1000);
    myTrainHub->playTone(0x01);
    Serial.println(0x01);
    ::delay(1000);
    myTrainHub->playTone(0x01);
    Serial.println(0x01);
    Serial.println("Tone10!");
    // Optional: Add a small delay to debounce the button
    ::delay(1000);
}


void playAllSounds(Lpf2Hub *myTrainHub) {
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::HORN));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::HORN));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::HORN));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::HORN));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STEAM));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STEAM));
    ::delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STEAM));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STEAM));
    ::delay(1000);
}
