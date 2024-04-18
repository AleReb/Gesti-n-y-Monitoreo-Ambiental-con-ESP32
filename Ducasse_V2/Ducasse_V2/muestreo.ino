void capturedata()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= muestreo)//displayMain
    {
        previousMillis = currentMillis;
        lcd.clear();
        timestamp = getTime();
        datoSD = String(timestamp) + "," + String(globalPh) + "\n";
        Serial.println(datoSD);
        appendFile(SD, filenameCSV, datoSD.c_str());
        changePixel(0,0,255);
        lcd.setCursor(0, 1);
        lcd.print("Guardado SD");
        publish();
        delay(3000);
        offpixels();
        lcd.clear();
        changePixel(0,255,0);
    }
}
void publish()
{
    // Get current timestamp
    timestamp = getTime();
    Serial.print("time: ");
    Serial.println(timestamp);
    parentPath = databasePath + "/" + String(timestamp);
    json.set(id.c_str(), String(numeroID));
    json.set(phPath.c_str(), String(globalPh));
    json.set(timePath, String(timestamp));
    lcd.setCursor(0, 2);
    lcd.print("Publicando...");
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
}
