//
// Created by Florian Thöni on 07/09/2024.
//

#ifndef LIB_TRAIN_4_EMILIEN_H
#define LIB_TRAIN_4_EMILIEN_H
void controlMotor(Lpf2Hub *myTrainHub, int16_t positionSpeed, uint8_t emergencyButtonState, uint8_t emergencyButtonLedPin, byte motorPort);
void handlePostConnection(Lpf2Hub *myTrainHub);
void hubPropertyChangeCallback(void *hub, HubPropertyReference hubProperty, uint8_t *pData);
void playAllTones(Lpf2Hub *myTrainHub);
void playAllSounds(Lpf2Hub *myTrainHub);
#endif //LIB_TRAIN_4_EMILIEN_H

