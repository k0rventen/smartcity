/**
 * @file lora.hpp
 * 
 * @page "Lora configuration"
 * 
 * @section conf "How to configure"
 * 
 * The lora.hpp references all the lora settings that will be applied 
 * to the lora antennas placed on the arduino.
 * 
 * These settings must match what is configured on the gateway, otherwise they may not be able to talk to each other.
 * 
 */
#include "arduinoUART.h"
#include "arduinoUtils.h"
#include "arduinoLoRaWAN.h"


static uint8_t socket = SOCKET1;
static uint8_t PORT = 3;
static uint8_t error;

// Garbage is 16,
// Parking is 15, 
// Lights & metrics is 14


static char DEVICE_EUI[] = "0102030405060714";  // DEV_EUI for indentification
static char DEVICE_ADDR[] = "06060714"; 
static char NWK_SESSION_KEY[] = "01020304050607080910111213141516"; // Creds
static char APP_SESSION_KEY[] = "000102030405060708090A0B0C0D0E0F"; // Creds


void setupLoRaStack(void);
void sendLoRaMessage(char *);