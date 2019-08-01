# Smart city

This document describes the context, purpose and a quick overview of the smart city mockup project, specifically from a programming point of view.

- [Smart city](#smart-city)
  - [Purpose](#purpose)
  - [Hardware used](#hardware-used)
  - [Smart scenarios](#smart-scenarios)
  - [How to install](#how-to-install)
  - [How to configure](#how-to-configure)
  - [Technical deepdive & interesting bits](#technical-deepdive--interesting-bits)
    - [Debugging memory overflow & stack crashes](#debugging-memory-overflow--stack-crashes)
    - [LoraWAN Stack](#lorawan-stack)
  - [License](#license)


## Purpose



## Hardware used

Here is a list of each component : 
- Arduino Uno as the micro-controller, on top of which sits:
  - Grove Hat for easy cable management, with the following sensors : 
    - Temperature sensor
    - Noise sensor
    - Light sensor
    - Hall effect sensors
    - Various LEDs
    - Ultrasonic distance sensors,
  - A LoRaWAN antenna, to upload the gathered data to a nearby gateway

## Smart scenarios

The aforementionned sensors allows us to create some scenarios that a future, smart city could integrate : 

**Street lamps control**

The street lamps are reacting to a light level sensor, which determines the needed brightness level of the street lamps.

**Trash cans monitoring**

**Parking management**

**City's data gathering**



## How to install

This project is built on top of various frameworks that simplify the development process :
- [PlatformIO](https://platformio.org) for developing, compiling & deploying the code to the embedded devices,
- [Doxygen](http://www.doxygen.nl/index.html) for automating thendocumentation building.

Once platformIO is installed ( aka **pip install platformio**), you will need just three commands to build the project: 

```
git clone 
cd SmartCity
platformio run
```

If you also want to build the documentation yourself, just install doxygen (aka **apt install doxygen**), `cd` in the `SmartCity` directory and run :

```
doxygen
```

You will then have an `doc/` folder. Open the `ìndex.html` file with your browser to access the html doc.

## How to configure

The same boilerplate file is deployed to every arduino. The logic is the same everywhere, the arduino fetches the sensors and uploads their data to the cloud through an LoRa gateway. The only difference between each Arduino is which sensors are connected and on which pin. 

Therefore, you must adapt the `header.hpp` to reflects the sensors connected to the arduino and their positions on the Grove Hat.

Here is an example `header.hpp`, configured with both Street Lamps control and parking monitoring.

```c
#define STREETLAMPSCENARIO
//#define WASTESCENARIO
#define PARKINGSCENARIO
//#define CITYMETRICSCENARIO
```

You must also report the correct pin layout, such as :

```c
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
```

As a sidenote, 
```c
#define DEBUG
```
will make a more verbose output, usefull during testing. You can comment it when pushing the code for production use.


## Technical deepdive & interesting bits

### Debugging memory overflow & stack crashes

If sometimes unexpected things are happening after a while, it may be time to check the memory pressure evolution of the programm. Here is a quick function that returns the interval between the stack and the heap. 

```c
extern char *__brkval;
int freeMemory()
{
    char top;
    return &top - __brkval;
}
```
Just add `Serial.println(freeMemory());` in your loop. If this gradually decreases towards 0, you have a memory leakage somewhere and stuff will break at some point. Dunno when, but it will.


### LoraWAN Stack



## License

2019, k0rventen

**MIT License.**

Use it. Tweak it. Improve on it.
 