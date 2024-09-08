#include <Arduino.h>
#include "Lpf2Hub.h"

#include "lib_train_4_emilien.h"
#include "lib_train_4_emilien_native.h"

// create a hub instance
constexpr byte port = static_cast<byte>(PoweredUpHubPort::A);
// Define the GPIO pin connected to the button
constexpr int buttonPin = 18;
constexpr int ledPin = 19; // GPIO for the LED
constexpr int potentiometerPin = 32;
constexpr byte motorPort = static_cast<byte>(DuploTrainHubPort::MOTOR);


void hubPropertyChangeCallback(void *hub, HubPropertyReference hubProperty, uint8_t *pData) {
    auto *myHub = static_cast<Lpf2Hub *>(hub);
    Serial.print("HubProperty: ");
    Serial.println(static_cast<byte>(hubProperty), HEX);

    if (hubProperty == HubPropertyReference::RSSI) {
        Serial.print("RSSI: ");
        Serial.println(myHub->parseRssi(pData), DEC);
        return;
    }

    if (hubProperty == HubPropertyReference::BATTERY_VOLTAGE) {
        Serial.print("BatteryLevel: ");
        Serial.println(myHub->parseBatteryLevel(pData), DEC);
        return;
    }

    if (hubProperty == HubPropertyReference::BUTTON) {
        Serial.print("Button: ");
        Serial.println(static_cast<byte>(myHub->parseHubButton(pData)), HEX);
        return;
    }
}

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);
    // Configure the button pin as input with internal pull-up resistor
    pinMode(buttonPin, INPUT_PULLUP);
    // Configure the LED pin as output
    pinMode(ledPin, OUTPUT);
    // Ensure the LED is off initially
    digitalWrite(ledPin, LOW);
}


void old_loop() {
    static Lpf2Hub myTrainHub;

    // Variable to store the button state
    // Read the state of the button
    if (!myTrainHub.isConnected() && !myTrainHub.isConnecting()) {
        myTrainHub.init(); // initialize the PoweredUpHub instance
        //myTrainHub.init("90:84:2b:03:19:7f"); //example of initializing a hub with a specific address
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
            myTrainHub.activateHubPropertyUpdate(HubPropertyReference::BATTERY_VOLTAGE, hubPropertyChangeCallback);
        } else {
            Serial.println("Failed to connect to HUB");
        }
    }

    // if connected, you can set the name of the hub, the LED color and shut it down
    if (myTrainHub.isConnected()) {
        int buttonState = 0;


        //char hubName[] = "myTrainHub";
        //myTrainHub.setHubName(hubName);
        // Check if the button is pressed (LOW because of pull-up)
        buttonState = digitalRead(buttonPin);
        if (buttonState == LOW) {
            // Print message to the serial monitor
            Serial.println("Button pressed!");
            digitalWrite(ledPin, HIGH);
            Serial.println("Button pressed! LED on.");

            playAllSounds(&myTrainHub);
            myTrainHub.shutDownHub();

            digitalWrite(ledPin, LOW);
        }
    } else {
        Serial.println("Train hub is disconnected");
    }
}


void loop() {
    static Lpf2Hub myTrainHub;
    static int currentSpeed;
    static bool emergencyStop = true;

    if (!myTrainHub.isConnected() && !myTrainHub.isConnecting()) {
        myTrainHub.init(); // initialize the PoweredUpHub instance
        //myTrainHub.init("90:84:2b:03:19:7f"); //example of initializing a hub with a specific address
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
            myTrainHub.activateHubPropertyUpdate(HubPropertyReference::BATTERY_VOLTAGE, hubPropertyChangeCallback);
        } else {
            Serial.println("Failed to connect to HUB");
        }
    }

    // if connected, you can set the name of the hub, the LED color and shut it down
    if (myTrainHub.isConnected()) {
        const int position_speed = analogRead(32);
        const int actual_speed = convertPotPositionToSpeed(position_speed);
        Serial.println("Analog reading");
        Serial.println(position_speed);
        Serial.println(actual_speed);
        if (emergencyStop && actual_speed == 0) {
            emergencyStop = false;
        }
        if (!emergencyStop && !(currentSpeed == 0 && actual_speed == 0)) {
            currentSpeed = actual_speed;
            myTrainHub.setBasicMotorSpeed(motorPort, actual_speed);
        }

        const int buttonState = digitalRead(buttonPin);
        if (buttonState == LOW) {
            // Print message to the serial monitor
            Serial.println("Button pressed!");
            digitalWrite(ledPin, HIGH);
            Serial.println("Button pressed! LED on.");

            emergencyStop = true;

            digitalWrite(ledPin, LOW);
        }
        if (emergencyStop) {
            Serial.println("LED on.");
            digitalWrite(ledPin, HIGH);
        } else {
            Serial.println("LED off.");
            digitalWrite(ledPin, LOW);
        }

        delay(100);
    } else {
        Serial.println("Train hub is disconnected");
    }
}
