/*!
 * @file main.cpp
 *
 * @page "Main logic"
 * 
 * @section lol Technical overview
 * 
 * This is the main script, from which every lib is called.
 * 
 * The logic is very simple, depending on which compiler flags are enabled (from the header.hpp),
 * the setup function will initialize the corresponding sensors, and the main runtime loop will call
 * each enabled scenarios in a programmatic fashion.
 * 
 * I.E. if this is present in the header : 
 * 
 * @code
 * # define STREETLAMPSCENARIO
 * @endcode
 * 
 * then the main logic will activate the Street Lamp management scenario. 
 * 
 * Because we are doing this in an infinite loop, each run is seperated by a delay, specified in the header by RUNTIME_DELAY.
 * 
 * @section LoRa
 * 
 * Furthermore, because each arduinos is uploading its gathered data, we configure the LoRa stach during setup(), 
 * then once every 10 loop, we send a payload.
 * 
 * This is not the most "efficient" way to do so, because we may miss some informations along the way (some information may get changed from one run to another),
 * but because we are using LoRa, we have to keep in mind the Duty cycle and faire use of the bandwith.
 * 
 * 
 */

#include "header.hpp"
#include "sensors.hpp"
#include "lora.hpp"

extern char *__brkval;
int counter = 0;

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
    Serial.println("Metrics Scenario");

    // Noise pollution
    int dB = get_dB_from_noise_sensor(SoundSensor);
    Serial.println(dB);
    if (dB > 99)
    {
        dB = 99;
    }

    // Temperature
    int celsius = get_temperature_from_temperature_pin(TemperatureSensor);
    Serial.println(celsius);
    if (celsius > 99)
    {
        celsius = 99;
    }

    // Flood
    bool flood = digitalRead(FloodSensor);
    Serial.println(flood);
    if (flood)
    {
        digitalWrite(FloodLED, HIGH);
    }
    else
    {
        digitalWrite(FloodLED, LOW);
    }
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
    int brightness = get_brightness_percentage(BrightnessSensor);

    // Add to the payload
    
#ifdef DEBUG
    Serial.println("Street Lamp scenario : ");
    Serial.print("Brightness percentage is : ");
    Serial.println(brightness);
#endif
    if (brightness < BRIGHTNESSTHRESHOLD && !StreetLampStatus)
    {
        Serial.println("Brightness is above threshold, turning street lamps off !");
        fadeStreetLampsUp(StreetLamps, StreetLampsNumber);
        StreetLampStatus = true;
        LoRaPayload[13] = '1';
    }

    else if (brightness > BRIGHTNESSTHRESHOLD && StreetLampStatus)
    {
        Serial.println("Brightness is under threshold, turning street lamps on !");
        fadeStreetLampsDown(StreetLamps, StreetLampsNumber);
        StreetLampStatus = false;
        LoRaPayload[13] = '0';
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
            LoRaPayload[i + 7] = '1';
        }
        else
        {
            SetLedStatus(ParkingLEDS[i], LOW);
            LoRaPayload[i + 7] = '0';
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
    Serial.println("Waste");
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
            LoRaPayload[i + 4] = '1';
        }
        else
        {
            SetLedStatus(WasteLEDs[i], LOW);
            LoRaPayload[i + 4] = '0';
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
    Serial.begin(57600);
    delay(2000);
    Serial.println("Smart city starting ! ");
    delay(2000);
#ifdef WASTESCENARIO
    UltraSonicSensorsLen = sizeof(UltrasonicSensors) / sizeof(UltrasonicSensors[0]);
    LoRaPayload[0] = '1';
#endif

#ifdef PARKINGSCENARIO
    HallSensorsLen = sizeof(HallSensors) / sizeof(HallSensors[0]);
    LoRaPayload[1] = '1';
#endif

#ifdef STREETLAMPSCENARIO
    LoRaPayload[2] = '1';
    pinMode(BrightnessSensor,INPUT);
#endif

#ifdef CITYMETRICSCENARIO
    LoRaPayload[3] = '1';
    pinMode(FloodLED, OUTPUT);
#endif

#ifdef LORA
    setupLoRaStack();
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

    // Delay between each run
    delay(RUNTIME_INTERVAL);

    // Send message every 10 loop
    if (counter == 10)
    {
        counter = 0;
#ifdef LORA
        Serial.println("Sending LoRa frame");
        sendLoRaMessage(LoRaPayload);
#endif
    }
    counter++;
}
