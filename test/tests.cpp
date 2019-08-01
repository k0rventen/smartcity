#include <Arduino.h>
#include <unity.h>
#include "sensors.hpp"
#include "header.hpp"
void test_led_builtin_pin_number(void) {
    TEST_ASSERT_EQUAL(13, LED_BUILTIN);
}

void test_led_state_high(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void test_led_state_low(void) {
    int dist = get_distance_from_ultrasonic_pin(UltrasonicSensors[0]);
}

void test_noise(void){ 
}
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_led_builtin_pin_number);

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    RUN_TEST(test_led_state_low);
        while (true)
    {
        Serial.println(get_dB_from_noise_sensor(SoundSensor));
        delay(1000);
    }
    UNITY_END(); // stop unit testing
}

