#include <Arduino.h>
#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"
#include "lib_train_4_emilien_native.h"


// Define the GPIO pin connected to the button
constexpr uint8_t buttonPin = 18;

// GPIO for the LED
constexpr uint8_t ledPin = 19;

constexpr uint8_t potentiometerPin = 32;
constexpr byte motorPort = static_cast<byte>(DuploTrainHubPort::MOTOR);


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

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);
    // Configure the button pin as input with internal pull-up resistor
    ::pinMode(::buttonPin, INPUT_PULLUP);
    // Configure the LED pin as output
    ::pinMode(::ledPin, OUTPUT);
    // Ensure the LED is off initially
    ::digitalWrite(::ledPin, LOW);
}


void oldLoop() {
    static Lpf2Hub myTrainHub;

    // Variable to store the button state
    // Read the state of the button
    if ((!myTrainHub.isConnected()) && (!myTrainHub.isConnecting())) {
        // initialize the PoweredUpHub instance
        myTrainHub.init();
    }

    // connect flow. Search for BLE services and try to connect if the uuid of the hub is found
    if (myTrainHub.isConnecting()) {
        myTrainHub.connectHub();
        if (myTrainHub.isConnected()) {
            Serial.println("Connected to HUB");
            Serial.print("Hub address: ");
            Serial.println(myTrainHub.getHubAddress().toString().c_str());
            Serial.print("Hub name: ");
            Serial.println(myTrainHub.getHubName().c_str());
            myTrainHub.activateHubPropertyUpdate(HubPropertyReference::BATTERY_VOLTAGE,
                                                 hubPropertyChangeCallback);
        } else {
            Serial.println("Failed to connect to HUB");
        }
    }

    // if connected, you can set the name of the hub, the LED color and shut it down
    if (myTrainHub.isConnected()) {
        uint8_t buttonState = 0;


        //char hubName[] = "myTrainHub";
        //myTrainHub.setHubName(hubName);
        // Check if the button is pressed (LOW because of pull-up)
        buttonState = ::digitalRead(::buttonPin);
        if (buttonState == LOW) {
            // Print message to the serial monitor
            Serial.println("Button pressed!");
            ::digitalWrite(::ledPin, HIGH);
            Serial.println("Button pressed! LED on.");

            playAllSounds(&myTrainHub);
            myTrainHub.shutDownHub();

            ::digitalWrite(::ledPin, LOW);
        }
    } else {
        Serial.println("Train hub is disconnected");
    }
}


void loop() {
    static Lpf2Hub myTrainHub;
    static int16_t currentSpeed = 2048;
    static bool emergencyStop = true;

    if ((!myTrainHub.isConnected()) && (!myTrainHub.isConnecting())) {
        // initialize the PoweredUpHub instance
        myTrainHub.init();
    }

    // connect flow. Search for BLE services and try to connect if the uuid of the hub is found
    if (myTrainHub.isConnecting()) {
        myTrainHub.connectHub();
        if (myTrainHub.isConnected()) {
            Serial.println("Connected to HUB");
            Serial.print("Hub address: ");
            Serial.println(myTrainHub.getHubAddress().toString().c_str());
            Serial.print("Hub name: ");
            Serial.println(myTrainHub.getHubName().c_str());
            myTrainHub.activateHubPropertyUpdate(
                HubPropertyReference::BATTERY_VOLTAGE,
                hubPropertyChangeCallback);
        } else {
            Serial.println("Failed to connect to HUB");
        }
    }

    // if connected, you can set the name of the hub, the LED color and shut it down
    if (myTrainHub.isConnected()) {
        const auto position_speed = static_cast<int16_t>(::analogRead(::potentiometerPin));
        const int16_t actual_speed = ::convertPotPositionToSpeed(position_speed);
        Serial.println("Analog reading");
        Serial.println(position_speed);
        Serial.println(actual_speed);
        if (emergencyStop && (actual_speed == 0)) {
            emergencyStop = false;
        }
        if ((!emergencyStop) && (!((currentSpeed == 0) && (actual_speed == 0)))) {
            currentSpeed = actual_speed;
            myTrainHub.setBasicMotorSpeed(motorPort, actual_speed);
        }

        const uint8_t buttonState = ::digitalRead(::buttonPin);
        if (buttonState == LOW) {
            // Print message to the serial monitor
            Serial.println("Button pressed!");
            ::digitalWrite(::ledPin, HIGH);
            Serial.println("Button pressed! LED on.");

            emergencyStop = true;

            ::digitalWrite(::ledPin, LOW);
        }
        if (emergencyStop) {
            Serial.println("LED on.");
            ::digitalWrite(::ledPin, HIGH);
        } else {
            Serial.println("LED off.");
            ::digitalWrite(::ledPin, LOW);
        }

        ::delay(100);
    } else {
        Serial.println("Train hub is disconnected");
    }
}
