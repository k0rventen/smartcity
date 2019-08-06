#include <Arduino.h>      // base framework
#include <Ultrasonic.h>   // For the ultrasonic sensors
#include <math.h>         // To do some maths
#include <ChainableLED.h> // Control the Leds for the street lamps

const int B = 4275;                  //! value of the thermistor
const long R0 = 100000;              //! R0 = 100k
const long ULTRASONICTHRESHOLD = 5;  //! Distance in centimeters that differentiates a full trash can from an empty one
const int BRIGHTNESSTHRESHOLD = 90; //! Arbitrary brightness threshold for day/night detection
const int RUNTIME_INTERVAL = 1000;   //! time in ms between runs

long get_distance_from_ultrasonic_pin(int pin);
int get_temperature_from_temperature_pin(int pin);
void SetLedStatus(int pin, bool status);
int get_dB_from_noise_sensor(int pin);
int get_brightness_percentage(int pin);
void fadeStreetLampsUp(ChainableLED array, int len);
void fadeStreetLampsDown(ChainableLED array, int len);