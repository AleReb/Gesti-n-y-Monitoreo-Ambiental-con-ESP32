// Function click encoder
void onPressed()
{
  preMiliMenu = currentMillis4;
    lcd.clear();
  Serial.println("Anotherbtn");
//  changeDisplay(showDisplay);
  if (displayNumber == 6) {
  lcd.clear();
      enter++;
  }
}
void onPressed2()
{
   preMiliMenu = currentMillis4;
    lcd.clear();
  if (displayNumber == 6 && enter == 1) {
      lcd.clear();
    muestMenu++;
    if (muestMenu == 4) {
      muestMenu = 1;
      lcd.clear();
    }

  } 
  if (displayNumber == 6 && enter == 2) {
      lcd.clear();
    muestMenub++;
    if (muestMenub == 3) {
      muestMenub = 1;
      lcd.clear();
    }

  } 
if (displayNumber != 6){
    lcd.clear();
    showDisplay = showDisplay + 1;
    Serial.println(showDisplay);
    limits(1, 6); // limite de pantallas
    lcd.clear();
  }
}

void limits(int min, int max)
{
  if (showDisplay == max + 1)
  {
    showDisplay = min;
    Serial.println("Limite Alcanzado");
  }
}
