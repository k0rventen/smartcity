/**
 * @file tests.cpp
 * 
 * @page Testing
 * 
 * @section introduction
 * 
 * They see me testin', they hatin'...
 * 
 * @section  tests "How to test"
 * 
 * PlatformIO makes it really easy to make unit tests. If your project is correctly strucutured,
 * (aka having all your functions placed under lib/), you just have to use the pre-written code and embed your functions.
 * 
 * PlatformIO provides a lot of tests like this one : 
 * 
 * @code
 * TEST_ASSERT_GREATER_OR_EQUAL(distance,distance1);
 * @endcode
 * 
 * and many other.
 * 
 * 
 * If you want to test this code, once the project is cloned and platformIO installed,
 * just ruin a quick `platformio test` !
 * 
 */
#include <Arduino.h>
#include <unity.h>
#include "sensors.hpp"
#include "header.hpp"

ChainableLED streetlamps(7,8, 3);


void test_noise()
{
    Serial.println("Try to make the less noise possible around the noise sensor..");
    delay(4000);
   int db = get_dB_from_noise_sensor(SoundSensor);
   Serial.println(db);
   Serial.println("Now speak up next to it");
   delay(3000);
   int db1 = get_dB_from_noise_sensor(SoundSensor);
   TEST_ASSERT_GREATER_OR_EQUAL(db,db1);
   Serial.print("Initial dB was ");
   Serial.print(db);
   Serial.print(" dB, second was ");
   Serial.print(db1);
   Serial.println(" dB");

}

void test_ultrasonic_sensor()
{
    Serial.print("Put your hand above the ultrasonic sensor connected on pin ");
    Serial.println(UltrasonicSensors[0]);
    delay(5000);
    int distance = get_distance_from_ultrasonic_pin(UltrasonicSensors[0]);
    TEST_ASSERT_LESS_THAN(10, distance);
    Serial.print("Remove your hand");
    Serial.println(UltrasonicSensors[0]);
    delay(5000);
    int distance1 = get_distance_from_ultrasonic_pin(UltrasonicSensors[0]);
    TEST_ASSERT_GREATER_THAN(distance, distance1);
}

void very_important_test(){
    int one = 1;
    int two = 2;
    TEST_ASSERT_GREATER_THAN(one,two);
    one ++;
    TEST_ASSERT_EQUAL(one,two);
}


void test_street_lamps()
{
    Serial.println("Ramping up the leds");
    fadeStreetLampsUp(streetlamps,1);
    Serial.println("All LEDS are up ? (y/n)");
    delay(4000);
    int rep = Serial.read();
    Serial.println(rep);
    TEST_ASSERT_EQUAL('y',(char) rep);
    Serial.println("Ramping down");
    fadeStreetLampsDown(streetlamps,1);
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    Serial.begin(9600);
    Serial.println("Test program is starting !");
    UNITY_BEGIN(); // IMPORTANT LINE!
}

void loop()
{
    RUN_TEST(test_ultrasonic_sensor);
    RUN_TEST(test_street_lamps);
    RUN_TEST(test_noise);
    UNITY_END(); // stop unit testing
}
