/*!
 * @file header.hpp
 *
 * @page Configuration
 *
 * To configure the programm to suits your particular setup,
 * you will need to adjust the variables in the header.hpp.
 *
 * Each preprocessor definition
 * will activate or deactivate a specified scenario.
 *
 * The DEBUG bit will allow more verbose output usefull during testing,
 * but should be disabled (aka commented) when running in prod.
 *
 * The LORA bit will enable the configuration of the LoRa
 * stack during setup and activate the transmission of
 * packets during runtime.
 *
 */
#include <Arduino.h>      // base framework
#include <Ultrasonic.h>   // For the ultrasonic sensors
#include <math.h>         // To do some maths
#include <ChainableLED.h> // Control the Leds for the street lamps

//#define DEBUG
#define LORA
//#define STREETLAMPSCENARIO
//#define WASTESCENARIO
#define PARKINGSCENARIO
//#define CITYMETRICSCENARIO

int UltrasonicSensors[] = {2, 3, 4};
int HallSensors[] = {6, 7, 8, 2, 3, 5};
int TemperatureSensor = A2;
int SoundSensor = A3;
int BrightnessSensor = A1;
int FloodSensor = 2;
int FloodLED = 3;
int WasteLEDs[] = {6, 7, 8};
int ParkingLEDS[] = {A2, A3, 9, 4, A0, A1};
int StreetLampsNumber = 8;
ChainableLED StreetLamps(4, 5, StreetLampsNumber);

const int RUNTIME_INTERVAL = 1000; //! time in ms between runs

bool StreetLampStatus = false; //! Status of the streetlamps
int UltraSonicSensorsLen;      //! Number of sensors in the array. Defined at runtime
int HallSensorsLen;            //! Number of sensors in the array.