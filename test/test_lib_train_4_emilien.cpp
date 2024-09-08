//
// Created by Florian Thöni on 07/09/2024.
//
#include <unity.h>
#include "lib_train_4_emilien_native.h" // Include the header file

void testconvertPotarPositionToSpeedMax() {
    TEST_ASSERT_EQUAL(77, convertPotarPositionToSpeed(4095));
}

void testconvertPotarPositionToSpeedMin() {
    const int actual = convertPotarPositionToSpeed(0);
    TEST_ASSERT_EQUAL(-77, actual);
}

void testconvertPotarPositionToSpeedMid() {
    TEST_ASSERT_EQUAL(0, convertPotarPositionToSpeed(2047));
}


int runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(testconvertPotarPositionToSpeedMax);
    RUN_TEST(testconvertPotarPositionToSpeedMin);
    RUN_TEST(testconvertPotarPositionToSpeedMid);
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
