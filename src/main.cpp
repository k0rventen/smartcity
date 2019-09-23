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

int counter = 0;
char LoRaPayload[31] = "000000000000000000000000000000"; //! payload to be transmitter

#ifdef CITYMETRICSCENARIO
/**
 * @brief Scenario that retrieves some data about the city
 * such as noise pollution, temperature and a flood indicator.
 * 
 */
void CityMetricsScenario()
{
    // Noise pollution
    int dB = get_dB_from_noise_sensor(SoundSensor);
    Serial.println(dB);
    if (dB > 99)
    {
        dB = 99;
    }
    int tens = dB / 10;
    int nums = dB % 10;
    char c_tens = tens + '0';
    char c_nums = nums + '0';
    // Add to the payload
    LoRaPayload[12] = c_tens;
    LoRaPayload[13] = c_nums;

    // Temperature
    int celsius = get_temperature_from_temperature_pin(TemperatureSensor);
    Serial.println(celsius);
    if (celsius > 99)
    {
        celsius = 99;
    }
     tens = celsius / 10;
     nums = celsius % 10;
     c_tens = tens + '0';
     c_nums = nums + '0';
    // Add to the payload
    LoRaPayload[14] = c_tens;
    LoRaPayload[15] = c_nums;

    // Flood
    bool flood = digitalRead(FloodSensor);
    Serial.println(flood);
    if (flood)
    {
        digitalWrite(FloodLED, HIGH);

        LoRaPayload[3] = '1';
    }
    else
    {
        digitalWrite(FloodLED, LOW);
        LoRaPayload[3] = '1';
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
    Serial.println(brightness);
    if (brightness > 99)
    {
        brightness = 99;
    }
    int tens = brightness / 10;
    int nums = brightness % 10;
    char c_tens = tens + '0';
    char c_nums = nums + '0';
    // Add to the payload
    LoRaPayload[10] = c_tens;
    LoRaPayload[11] = c_nums;

#ifdef DEBUG
    Serial.println("Street Lamp scenario : ");
    Serial.print("Brightness percentage is : ");

#endif
    if (brightness < BRIGHTNESSTHRESHOLD && !StreetLampStatus)
    {
        fadeStreetLampsUp(StreetLamps, StreetLampsNumber);
        StreetLampStatus = true;

        LoRaPayload[0] = '1';
    }

    else if (brightness > BRIGHTNESSTHRESHOLD && StreetLampStatus)
    {
        fadeStreetLampsDown(StreetLamps, StreetLampsNumber);
        StreetLampStatus = false;
        LoRaPayload[0] = '0';
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
            LoRaPayload[i * 2] = '1';
        }
        else
        {
            SetLedStatus(ParkingLEDS[i], LOW);
            LoRaPayload[i * 2] = '0';
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
            LoRaPayload[i * 2] = '1';
        }
        else
        {
            SetLedStatus(WasteLEDs[i], LOW);
            LoRaPayload[i * 2] = '0';
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
#endif

#ifdef PARKINGSCENARIO
    HallSensorsLen = sizeof(HallSensors) / sizeof(HallSensors[0]);
#endif

#ifdef STREETLAMPSCENARIO
    pinMode(BrightnessSensor, INPUT);
#endif

#ifdef CITYMETRICSCENARIO
    pinMode(FloodLED, OUTPUT);
#endif

#ifdef LORA
    delay(1000);
    setupLoRaStack();
    delay(1000);
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

    // Send message every 5 minutes
    // A loop is 5s, meaning for 5 minute , we'll need 600 / 5 = 120 cycles
    if (counter == 12)
    {
        counter = 0;
        
#ifdef LORA
        sendLoRaMessage(LoRaPayload);
#endif
    }
    counter++;
    Serial.println(counter);
}
