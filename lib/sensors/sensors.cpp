#include "sensors.hpp"

/**
 * @brief Get the distance from ultrasonic pin object
 * 
 * @param pin the digital pin to use
 * @return (long) the distance in centimeters
 */
long get_distance_from_ultrasonic_pin(int pin)
{
    Ultrasonic sensor(pin);
    long distance = sensor.MeasureInCentimeters();
    return distance;
}

/**
 * @brief Get the temperature from temperature pin object
 * 
 * @param pin 
 * @return int 
 */
int get_temperature_from_temperature_pin(int pin)
{
    int raw_val = analogRead(pin);
    float R = 1023.0 / raw_val - 1.0;
    R = R0 * R;
    float temp = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to celsius
    return (int)temp;
}


/**
 * @brief Wrapper to toggle LEDs on or off, regardless of their analog or digital connection
 * 
 * @param pin the pin of the led (can be digital or analog)
 * @param status HIGH or LOW, aka ON or OFF
 */
void SetLedStatus(int pin, bool status)
{
    pinMode(pin, OUTPUT);
    delay(20);
    if (pin > 13 && status)
    {
        analogWrite(pin, 0);
    }
    else if (pin > 13 && !status)
    {
        analogWrite(pin, 255);
    }
    else
    {
        digitalWrite(pin, !status);
    }
}

/**
 * @brief Get the dB from noise sensor object
 * 
 * @param pin 
 * @return int 
 */
int get_dB_from_noise_sensor(int pin){
    int loudnessSum = 0;
    for (int i = 0; i < 20; i++)
    {
        loudnessSum+=analogRead(pin);
        delay(80);
    }
    int loudnessAvg = loudnessSum / 20;
    int dB = 20 * log10(loudnessAvg);
    return dB;
}

/**
 * @brief Wrapper around ChainableLED.setColorRGB() to loop around each item in the StreetLamps object.
 * 
 * @param brightness the desired brightness, ranging from 0 (off) to 255 (max)
 */
void SetBrightnessOfStreetLamps(int brightness, ChainableLED lamps, int numOfStreetLamps)
{
    for (int i = 0; i < numOfStreetLamps; i++)
    {
        lamps.setColorRGB(i, brightness, brightness, brightness);
    }
}

