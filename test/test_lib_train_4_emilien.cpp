//
// Created by Florian Thöni on 07/09/2024.
//
#include <unity.h>
#include "lib_train_4_emilien_native.h" // Include the header file

void testConvertPotentiometerPositionToSpeedMax() {
    TEST_ASSERT_EQUAL(77, convertPotentiometerPositionToSpeed(4095));
}

void testConvertPotentiometerPositionToSpeedMin() {
    const int actual = convertPotentiometerPositionToSpeed(0);
    TEST_ASSERT_EQUAL(-77, actual);
}

void testConvertPotentiometerPositionToSpeedMid() {
    TEST_ASSERT_EQUAL(0, convertPotentiometerPositionToSpeed(2047));
}


int runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(testConvertPotentiometerPositionToSpeedMax);
    RUN_TEST(testConvertPotentiometerPositionToSpeedMin);
    RUN_TEST(testConvertPotentiometerPositionToSpeedMid);
    return UNITY_END();
}

void setup() {
    runUnityTests();
}

void loop() {
    // No code to run there, but needed by test framework.
}

int main() {
    return runUnityTests();
}
