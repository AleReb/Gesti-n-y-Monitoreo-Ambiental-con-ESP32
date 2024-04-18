
// Calibracion R1 x 1 minuto

uint16_t calibration(uint8_t ph, const char* filename, uint16_t numSamples = 60, float stabilityThreshold = 0, uint16_t maxStabilityAttempts = 100)
{
  uint32_t sum = 0;
  bool isStable = false;
  int stabilityAttempt = 0;

  // Stabilization period
  lcd.setCursor(0, 0);
  lcd.print("Stabilizing...");
  Serial.println("Stabilizing...");

  while (!isStable && stabilityAttempt < maxStabilityAttempts)
  {
    int prevReading = analogRead(sensorph);
    delay(3000);
    int currentReading = analogRead(sensorph);
    lcd.clear();
    // Stabilization period
    lcd.setCursor(0, 0);
    lcd.print("Stabilizing...");
    Serial.println("Stabilizing...");
    lcd.setCursor(0, 2);
    lcd.print("Stability value: " + String(currentReading - prevReading));


    Serial.print("Stability value: ");
    Serial.println(currentReading - prevReading);

    if (abs(currentReading - prevReading) <= stabilityThreshold)
    {

      isStable = true;
    }

    stabilityAttempt++;
    Serial.print("Stability Attempt: ");
    Serial.println(stabilityAttempt);
  }

  if (isStable)
  {
    lcd.clear();
    Serial.println("Stabilization successful!");

    for (int i = 0; i < numSamples; i++)
    {
      sum += analogRead(sensorph);
      calibration_analog_Display(ph, i);
      lcd.setCursor(0, 2);
      voltage = sum / 4095.0 * 3.3; ////////////mejorando lijeramente la lectura
      lcd.print("raw: " + String(sum / (i + 1)) + " V:" + voltage);
      Serial.println("raw: " + String(sum / (i + 1)));
      delay(1000);
    }

    uint16_t averageValue = sum / numSamples;

    // Save the calibration value to SPIFFS
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Guardando Calibracion");
    lcd.setCursor(0, 1);
    lcd.print("PH " + String(ph) + " ");
    String NP = String(averageValue);
    writeFile(LittleFS, filename, NP.c_str());
    delay(1000);
    readFile(LittleFS, filename);
    Serial.println(" dato recuperado: " + value);

    // Return the average value
    return averageValue;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to stabilize");
    Serial.println("Failed to stabilize");
    delay(2000);
  }

  // Return to the calibration menu
  lcd.clear();
  showDisplay = 1;
  displayNumber = 0;

  // Return 0 if the calibration failed
  return 0;
}

void calibrationPH4()
{
  Serial.println("Starting calibration for PH 4");
  uint16_t tempR1 = calibration(4, "/r1.txt");
  if (tempR1 > 0) {
    R1 = tempR1;
    Serial.println("Calibration for PH 4 successful");
    // Return to the calibration menu
    lcd.clear();
    showDisplay = 1;
    displayNumber = 0;
  } else {
    Serial.println("Calibration for PH 4 failed");
  }
}

void calibrationPH7()
{
  Serial.println("Starting calibration for PH 7");
  uint16_t tempR2 = calibration(7, "/r2.txt");
  if (tempR2 > 0) {
    R2 = tempR2;
    Serial.println("Calibration for PH 7 successful");
    // Return to the calibration menu
    lcd.clear();
    showDisplay = 1;
    displayNumber = 0;
  } else {
    Serial.println("Calibration for PH 7 failed");
  }
}

void calibrationPH10()
{
  Serial.println("Starting calibration for PH 10");
  uint16_t tempR3 = calibration(10, "/r3.txt");
  if (tempR3 > 0) {
    R3 = tempR3;
    Serial.println("Calibration for PH 10 successful");
    // Return to the calibration menu
    lcd.clear();
    showDisplay = 1;
    displayNumber = 0;
  } else {
    Serial.println("Calibration for PH 10 failed");
  }
}
