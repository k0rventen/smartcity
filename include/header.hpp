/*!
 * @file header.hpp
 *
 * @page Configuration
 *
 * This is the source documentation for the smart city mockup v2, specifically from a programming stand point.
 * To configure the programm to suits your particular setup, you will need to adjust the following variables, in the header.hpp : 
 * 
 * @code
#define DEBUG
#define STREETLAMPSCENARIO
#define WASTESCENARIO
#define PARKINGSCENARIO
#define CITYMETRICSCENARIO

int UltrasonicSensors[] = {2};
int HallSensors[] = {6};
int TemperatureSensor = A0;
int SoundSensor = A3;
int BrightnessSensor = A2;
int FloodSensor = A3;
int WasteLEDs[] = {5};
int ParkingLEDS[] = {A1};
int NumberOfStreetLamps = 8;
ChainableLED StreetLamps(8, 9, NumberOfStreetLamps);
 * @endcode
 * 
 * 
 */
#include <Arduino.h>      // base framework
#include <Ultrasonic.h>   // For the ultrasonic sensors
#include <math.h>         // To do some maths
#include <ChainableLED.h> // Control the Leds for the street lamps

#define DEBUG
//#define STREETLAMPSCENARIO
#define WASTESCENARIO
#define PARKINGSCENARIO
//#define CITYMETRICSCENARIO

int UltrasonicSensors[] = {4};
int HallSensors[] = {6};
int TemperatureSensor = A0;
int SoundSensor = A0;
int BrightnessSensor = A2;
int FloodSensor = A3;
int WasteLEDs[] = {5};
int ParkingLEDS[] = {A1};
int StreetLampsNumber = 3;
ChainableLED StreetLamps(8, 9, StreetLampsNumber);
ChainableLED ParkingLEDs(6,7, 6);
bool StreetLampStatus = false;       //! Status of the streetlamps
int UltraSonicSensorsLen;            //! Number of sensors in the array. Defined at runtime
int HallSensorsLen;                  //! Number of sensors in the array.