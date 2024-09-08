//
// Created by Florian Thöni on 07/09/2024.
//
#include <cmath>
#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"


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
