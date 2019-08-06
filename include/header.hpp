/*!
 * @file header.hpp
 *
 * @page Configuration
 * 
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
//#define LORA
#define STREETLAMPSCENARIO
//#define WASTESCENARIO
//#define PARKINGSCENARIO
//#define CITYMETRICSCENARIO

int UltrasonicSensors[] = {2,3};
int HallSensors[] = {2};
int TemperatureSensor = A0;
int SoundSensor = A1;
int BrightnessSensor = A2;
int FloodSensor = A3;
int WasteLEDs[] = {6,7};
int ParkingLEDS[] = {6};
int StreetLampsNumber = 8;
ChainableLED StreetLamps(8, 9, StreetLampsNumber);


bool StreetLampStatus = false;       //! Status of the streetlamps
int UltraSonicSensorsLen;            //! Number of sensors in the array. Defined at runtime
int HallSensorsLen;                  //! Number of sensors in the array.
char data[] = "68656C6C6F20776F726C640D0A0000";
