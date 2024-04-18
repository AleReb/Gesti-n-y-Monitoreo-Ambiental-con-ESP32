// Text x main menu
void firtsDisplay(int display_Home)
{
  lcd.setCursor(0, 0);
  lcd.print(tipoEst + " " + numeroID );
    lcd.setCursor(0, 3);
    lcd.print("C/muestra:");
    lcd.setCursor(15, 3);
    lcd.print(muestreo / 60000);
    
  switch (display_Home)
  {
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("PH ACTUAL  ");
      lcd.setCursor(10, 1);
      lcd.print(globalPh);
      lcd.setCursor(17, 0);
      lcd.print("1/6");
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("Calibrar PH 4.0     ");
      lcd.setCursor(13, 1);
      lcd.setCursor(17, 0);
      lcd.print("2/6");
      /* code */
      break;
    case 3:
      lcd.setCursor(0, 1);
      lcd.print("Calibrar PH 7.0     ");
      lcd.setCursor(13, 1);
      lcd.setCursor(17, 0);
      lcd.print("3/6");
      break;
    case 4:
      lcd.setCursor(0, 1);
      lcd.print("Calibrar PH 10.0    ");
      lcd.setCursor(13, 1);
      lcd.setCursor(17, 0);
      lcd.print("4/6");

      break;
    case 5:
      lcd.setCursor(0, 1);
      lcd.print("Delete WIFI");
      lcd.setCursor(13, 1);
      lcd.setCursor(0, 2);
      lcd.print("M:" + String(WiFi.macAddress()));
      lcd.setCursor(17, 0);
      lcd.print("5/6");
      break;
    case 6:
      lcd.setCursor(0, 1);
      lcd.print("Minutos muestreo");
      //      lcd.setCursor(0, 2);
      //      lcd.print("enter para cambiar");
      lcd.setCursor(17, 0);
      lcd.print("6/6");
      /* code */
      break;
  }
}

void setupcheck(String part , String state , String foot)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PH Check Boot");
  lcd.setCursor(0, 1);
  lcd.print(part);
  lcd.setCursor(0, 2);
  lcd.print(state);
  lcd.setCursor(0, 3);
  lcd.print(foot);
  delay(2000);
}

// Text x calibration menu
void calibration_analog_Display(int PH, int time)
{
  lcd.setCursor(0, 0);
  lcd.print("Adjust PH: " + String(PH) + "           ");
  lcd.setCursor(0, 1);
  lcd.print("Tiempo Res: " + String(time) + "          ");
}
