

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

int mapSignal(int rssi) {
  int minValue = -100;
  int maxValue = -30;
  int percentage = map(constrain(rssi, minValue, maxValue), minValue, maxValue, 0, 100);
  return percentage;
}

int mapSignalToLevels(int rssi, int numberOfLevels) {
  int minValue = -100;
  int maxValue = -30;
  int level = map(constrain(rssi, minValue, maxValue), minValue, maxValue, 1, numberOfLevels + 1);
  return level;
  }
  
// GetTimefromNTP
unsigned long getTime()
{
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}
