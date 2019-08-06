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
 * @brief Get the brightness percentage from an analog brightness sensor
 * 
 * The sensor is returning a range between 0 (complete dark) and 720 (maximum sensitivity of the sensor)
 * We map this range to 0-100 and return the corresponding value.
 * 
 * @param pin the analog pin connected to the brightness sensor
 * @return the percentage (0-100) perceived by the sensor
 */
int get_brightness_percentage(int pin){
    int raw = analogRead(pin);
    int percent = map(raw,0,720,0,100);
    return percent;
}
/**
 * @brief Get the temperature from a temperature sensor
 * 
 * The quick math involved to turn the analog voltage response into a readable
 * celsius value is taken from the datasheet of the sensor.
 * 
 * @param pin the analog pin connected to the temperature sensor
 * @return int the current temperature in degree celsius
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
int get_dB_from_noise_sensor(int pin)
{
    int loudnessSum = 0;
    for (int i = 0; i < 20; i++)
    {
        loudnessSum += analogRead(pin);
        delay(80);
    }
    int loudnessAvg = loudnessSum / 20;
    int dB = 20 * log10(loudnessAvg);
    return dB;
}

/**
 * @brief fade up a ChainableLED array, one led at a time.
 * 
 * @param array the ChainableLED object
 * @param len the number of LEDs on the ChainableLED array.
 */
void fadeStreetLampsUp(ChainableLED array, int len)
{
    for (byte lamp = 0; lamp < len; lamp++)
    {
        for (int i = 0; i <= 255; i+=5)
        {
            array.setColorRGB(lamp, i, i, i);
            delay(4);
        }
    }
}

/**
 * @brief fade down a ChainableLED array, one led at a time.
 * 
 * @param array the ChainableLED object
 * @param len the number of LEDs on the ChainableLED array.
 */
void fadeStreetLampsDown(ChainableLED array, int len)
{
    for (byte lamp = 0; lamp < len; lamp++)
    {
        for (int i = 255; i >= 0; i-=5)
        {
            array.setColorRGB(lamp, i, i, i);
            delay(4);
        }
    }
}