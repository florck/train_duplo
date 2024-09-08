//
// Created by Florian Thöni on 07/09/2024.
//
#include <unity.h>
#include "lib_train_4_emilien_native.h" // Include the header file

void testconvertPotPositionToSpeedMax() {
    TEST_ASSERT_EQUAL(77, convertPotPositionToSpeed(4095));
}

void testconvertPotPositionToSpeedMin() {
    const int actual = convertPotPositionToSpeed(0);
    TEST_ASSERT_EQUAL(-77, actual);
}

void testconvertPotPositionToSpeedMid() {
    TEST_ASSERT_EQUAL(0, convertPotPositionToSpeed(2047));
}


int runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(testconvertPotPositionToSpeedMax);
    RUN_TEST(testconvertPotPositionToSpeedMin);
    RUN_TEST(testconvertPotPositionToSpeedMid);
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
