#include "lora.hpp"

/**
 * @brief Configure the LoRaWAN stack before using it
 * 
 * We just reset the antenna, then we apply the config from the 
 * variables defined in the header (DEVICE_EUI, etc..)
 * 
 */
void setupLoRaStack()
{
  LoRaWAN.ON(socket);
  LoRaWAN.reset();
  LoRaWAN.setDeviceEUI(DEVICE_EUI);
  LoRaWAN.setAppEUI(APP_EUI);
  LoRaWAN.setAppKey(APP_KEY);
  LoRaWAN.setDataRate(5);
  LoRaWAN.saveConfig();
  LoRaWAN.getDeviceEUI();
  LoRaWAN.getAppEUI();
}

/**
 * @brief Send a message to a LoRa gateway. The latter is joined through Over-The-Air-Activation 
 * for the moment, but may be switch to ABP.
 * 
 * @param message the message to broadcast using the LoRa antenna.
 * The message must be a 31 char long payload, because of the datarate the antenna is using.
 */
void sendLoRaMessage(char *message)
{
  error = LoRaWAN.joinOTAA();
  if (error != 0)
  {
    Serial.print("Error while joining : ");
    Serial.println(error);
  }
  error = LoRaWAN.sendUnconfirmed(PORT, message);
  if (error != 0)
  {
    Serial.print("Error while sending message : ");
    Serial.println(error);
  }
}