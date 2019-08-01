/*!
 * @file main.cpp
 *
 * @page "Technical overview"
 * 
 * @section Global
 * 
 * fezfzeafaze
 * 
 * @section tech Intersting bits
 * 
 *
 */
#include "header.hpp"
#include "sensors.hpp"

extern char *__brkval;

/**
@brief Quick function that returns the space between stack and heap.
        If it goes to 0, you'll run into some serious problems, aka overflow or crash. 
        
@return the interval between the stack and the heap.
*/
int freeMemory()
{
    char top;
    return &top - __brkval;
}

#ifdef CITYMETRICSCENARIO
/**
 * @brief Scenario that retrieves some data about the city
 * such as noise pollution, temperature and a flood indicator.
 * 
 */
void CityMetricsScenario()
{
    int dB = analogRead(SoundSensor);
    int celsius = get_temperature_from_temperature_pin(TemperatureSensor);
    bool flood = digitalRead(FloodSensor);
}
#endif

#ifdef STREETLAMPSCENARIO
/**
 * @brief Scenario that controls the street lamps based upon the current light level in the city.
 * For now it uses a threshold to control whether the lamps are on or off, but it could be more granular in the future,
 * by adjusting slighty the level of the street lamps based on a range instead. It would a smooth, slow fading effect.
 * It must be noted however that the toggling of the lamps is not a simple on/off, but a smooth fade up / down.
 * 
 */
void StreetLampsScenario()
{
    int brightness = analogRead(BrightnessSensor);

    if (brightness > BRIGHTNESSTHRESHOLD && !StreetLampStatus)
    {
        for (int i = 0; i < 255; i++)
        {
            
        }
        StreetLampStatus = true;
    }

    else if (brightness < BRIGHTNESSTHRESHOLD && StreetLampStatus)
    {
        for (int i = 255; i < 0; i--)
        {
            SetBrightnessOfStreetLamps(i,StreetLamps, NumberOfStreetLamps);
        }
        StreetLampStatus = false;
    }
}
#endif

#ifdef PARKINGSCENARIO
/**
 * @brief Scenario that controls the parking spots of the city.
 * 
 * Each parking spot is monitored and connected to a LED.
 * If the parking sport is taken, the LED is up.
 * 
 */
void ParkingScenario()
{

    for (int i = 0; i < HallSensorsLen; i++)
    {
        bool isTaken = !digitalRead(HallSensors[i]);
#ifdef DEBUG
        Serial.print("\t");
        Serial.print(i);
        Serial.print(" -> ");
        Serial.println(isTaken);
#endif
        if (isTaken)
        {
            SetLedStatus(ParkingLEDS[i], HIGH);
        }
        else
        {
            SetLedStatus(ParkingLEDS[i], LOW);
        }
    }
}
#endif

#ifdef WASTESCENARIO
/**
 * @brief Scenario which controls and monitor the city trash cans.
 * 
 * The trashes fullness level is measured through an ultrasonic sensor.
 * If the trash is "filled up", a LED lights up.
 * 
 */
void WasteScenario()
{
    for (int i = 0; i < UltraSonicSensorsLen; i++)
    {
        long distance = get_distance_from_ultrasonic_pin(UltrasonicSensors[i]);
#ifdef DEBUG
        Serial.print("\t");
        Serial.print(i);
        Serial.print(" -> ");
        Serial.println(distance);
#endif
        if (distance < ULTRASONICTHRESHOLD)
        {
            SetLedStatus(WasteLEDs[i], HIGH);
        }
        else
        {
            SetLedStatus(WasteLEDs[i], LOW);
        }
    }
}
#endif

/**
 * @brief Configuration code, before entering loopy runtime code.
 * 
 * Everything is configured using the \#include statements defined in the header.hpp
 */
void setup()
{
    Serial.begin(9600);
    Serial.println("Smart city starting ! ");
    Serial.println("Listing enabled scenarios..");
#ifdef WASTESCENARIO
    Serial.println("Waste management scenario is enabled");
    UltraSonicSensorsLen = sizeof(UltrasonicSensors) / sizeof(UltrasonicSensors[0]);
    Serial.print("Number of trash cans connected : ");
    Serial.println(UltraSonicSensorsLen);
#endif

#ifdef PARKINGSCENARIO
    Serial.println("Parking monitoring scenario is enabled");
    HallSensorsLen = sizeof(HallSensors) / sizeof(HallSensors[0]);
    Serial.print("Number of parking spots connected : ");
    Serial.println(HallSensorsLen);
#endif

#ifdef STREETLAMPSCENARIO
    Serial.println("Street lamp control scenario is enabled");
#endif

#ifdef CITYMETRICSCENARIO
    Serial.println("City metrics scenario is enabled");
#endif
}

/**
  * @brief  Loopy code for runtime.
  * It just launches the scenarios programmaticaly based on which scenarios are enabled.
  * 
  * Maybe in a future version, we'll move to freeRTOS, as an overkill maneuver.
  * 
  */
void loop()
{
#ifdef DEBUG
    Serial.print("Stack to heap interval : ");
    Serial.println(freeMemory());
#endif

#ifdef WASTESCENARIO
    WasteScenario();
#endif

#ifdef PARKINGSCENARIO
    ParkingScenario();
#endif

#ifdef CITYMETRICSCENARIO
    CityMetricsScenario();
#endif

#ifdef STREETLAMPSCENARIO
    StreetLampsScenario();
#endif

    delay(RUNTIME_INTERVAL);
}