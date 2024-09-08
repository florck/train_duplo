//
// Created by Florian Thöni on 07/09/2024.
//

#ifndef LIB_TRAIN_4_EMILIEN_H
#define LIB_TRAIN_4_EMILIEN_H
#include <atomic>

void controlSounds(Lpf2Hub *myTrainHub, uint8_t nextSoundButtonState,
                   uint8_t nextSoundButtonLedPin, uint8_t previousSoundButtonState,
                   uint8_t previousSoundButtonLedPin, std::atomic<bool> &sendingOrderFlag);

void controlMotor(Lpf2Hub *myTrainHub, int16_t positionSpeed, uint8_t emergencyButtonState,
                  uint8_t emergencyButtonLedPin, byte motorPort,
                  std::atomic<bool> &sendingOrderFlag);

void handlePostConnection(Lpf2Hub *myTrainHub);

void hubPropertyChangeCallback(void *hub, HubPropertyReference hubProperty, uint8_t *pData);

void playAllTones(Lpf2Hub *myTrainHub);

void playAllSounds(Lpf2Hub *myTrainHub);
#endif //LIB_TRAIN_4_EMILIEN_H
