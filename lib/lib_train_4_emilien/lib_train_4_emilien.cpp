//
// Created by Florian Thöni on 07/09/2024.
//
#include <cmath>
#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"
#include "lib_train_4_emilien_native.h"

void controlMotor(Lpf2Hub *myTrainHub, const int16_t positionSpeed,
                  const uint8_t emergencyButtonState, const uint8_t emergencyButtonLedPin,
                  const byte motorPort) {
    static int16_t currentSpeed = 2048;
    static bool emergencyStop = true;

    const int16_t actualSpeed = ::convertPotPositionToSpeed(positionSpeed);
    Serial.println("Analog reading");
    Serial.println(positionSpeed);
    Serial.println(actualSpeed);

    if (emergencyStop && (actualSpeed == 0)) {
        emergencyStop = false;
    }
    if ((!emergencyStop) && (!((currentSpeed == 0) && (actualSpeed == 0)))) {
        currentSpeed = actualSpeed;
        myTrainHub->setBasicMotorSpeed(motorPort, actualSpeed);
        ::delay(100);
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
        Serial.println("LED on.");
        ::digitalWrite(emergencyButtonLedPin, HIGH);
    } else {
        Serial.println("LED off.");
        ::digitalWrite(emergencyButtonLedPin, LOW);
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
    delay(1000);
    myTrainHub->playTone(0x0a);
    Serial.println(0x0a);
    delay(1000);
    myTrainHub->playTone(0x0a);
    Serial.println(0x0a);
    delay(1000);
    myTrainHub->playTone(0x09);
    Serial.println(0X09);
    delay(1000);
    myTrainHub->playTone(0x09);
    Serial.println(0X09);
    delay(1000);
    myTrainHub->playTone(0x08);
    Serial.println(static_cast<byte>(8));
    delay(1000);
    myTrainHub->playTone(0x08);
    Serial.println(static_cast<byte>(8));
    delay(1000);
    myTrainHub->playTone(0x07);
    Serial.println(0x07);
    delay(1000);
    myTrainHub->playTone(0x07);
    Serial.println(0x07);
    delay(1000);
    myTrainHub->playTone(0x06);
    Serial.println(0x06);
    delay(1000);
    myTrainHub->playTone(0x06);
    Serial.println(0x06);
    delay(1000);
    myTrainHub->playTone(0x05);
    Serial.println(0x05);
    delay(1000);
    myTrainHub->playTone(0x05);
    Serial.println(0x05);
    delay(1000);
    myTrainHub->playTone(0x04);
    Serial.println(0x04);
    delay(1000);
    myTrainHub->playTone(0x04);
    Serial.println(0x04);
    delay(1000);
    myTrainHub->playTone(0x03);
    Serial.println(0x03);
    delay(1000);
    myTrainHub->playTone(0x03);
    Serial.println(0x03);
    delay(1000);
    myTrainHub->playTone(0x02);
    Serial.println(0x02);
    delay(1000);
    myTrainHub->playTone(0x02);
    Serial.println(0x02);
    delay(1000);
    myTrainHub->playTone(0x01);
    Serial.println(0x01);
    delay(1000);
    myTrainHub->playTone(0x01);
    Serial.println(0x01);
    Serial.println("Tone10!");
    // Optional: Add a small delay to debounce the button
    delay(1000);
}


void playAllSounds(Lpf2Hub *myTrainHub) {
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::BRAKE));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STATION_DEPARTURE));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::WATER_REFILL));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::HORN));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::HORN));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::HORN));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::HORN));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STEAM));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STEAM));
    delay(1000);
    myTrainHub->playSound(static_cast<byte>(DuploTrainBaseSound::STEAM));
    Serial.println(static_cast<byte>(DuploTrainBaseSound::STEAM));
    delay(1000);
}
