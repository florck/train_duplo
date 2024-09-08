//
// Created by Florian Thöni on 07/09/2024.
//
#include <unity.h>
#include "lib_train_4_emilien_native.h" // Include the header file

void testConvert11SpeedMax() {
    TEST_ASSERT_EQUAL(77, ::convertPotPositionToSpeed(4095));
}

void testConvert11SpeedMin() {
    TEST_ASSERT_EQUAL(-77, ::convertPotPositionToSpeed(0));
}

void testConvert11SpeedMid() {
    TEST_ASSERT_EQUAL(0, ::convertPotPositionToSpeed(2047));
}


uint8_t runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(::testConvert11SpeedMax);
    RUN_TEST(::testConvert11SpeedMin);
    RUN_TEST(::testConvert11SpeedMid);
    return UNITY_END();
}

void setup() {
    ::runUnityTests();
}

void loop() {
    // No code to run there, but needed by test framework.
}

int main() {
    return ::runUnityTests();
}
