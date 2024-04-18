#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EasyButton.h>
#include "FS.h"
#include <LittleFS.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <Adafruit_NeoPixel.h>
#include "SD.h"
#include "SPI.h"
#include "Update.h"
// spiffs
#define FORMAT_LITTLEFS_IF_FAILED true
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 27
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

int displayNumber = 1; // id display
int showDisplay = 1;   // Update value rotary

// PH CONTROL
float const ph4 = 4.00;   // Value x PH Calculated
float const ph7 = 7.00;   // Value x PH Calculated
float const ph10 = 10.00; // Value x PH Calculated
float R1 = 999.00;        // Value x PH Calculated
float R2 = 1100.00;        // Value x PH Calculated
float R3 = 1200.00;        // Value x PH Calculated
int sensorph = 32;
float globalPh; // Variable global para actualizar valor de PH
//Temperatura
float tempC;
float voltage ;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Setup x lcd


// Buttons x navigations
const int button_NEXT = 17;
const int button_ENTER = 16;
EasyButton btn_ENTER(button_ENTER); // Check x click rotarys
EasyButton btn_NEXT(button_NEXT);   // Check x click rotarys
// Enter or Out Menus
boolean subMenu_Medir = false;
boolean subMenu_Calibrar = false;
boolean subMenu_Calibrar_2 = false;
boolean mainMenu = true;
// Variable x get memory spiffs
String value;
// object WifiManager
WiFiManager wm;
//// Firebase Setting
//// fire base  parameters

#define API_KEY       "AIzaSyB6mCpJNuFqAHd_I6U9u2oWYTEfm4LT8fs" //
#define DATABASE_URL  "https://macayabackup-default-rtdb.firebaseio.com/"
#define USER_EMAIL    "arebolledo@udd.cl"
#define USER_PASSWORD "unodos34"
int counter = 1;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;
///////////////////////////////////////// version y estanques
String tipoEst="acumulador";
String numeroID = "1"; 
String ver = "V0.2";
//////////////////////////////////////////////////////////////
// el numero id tambien es parte del path
// Setup data to send
String databasePath;
String id = "/id";
String phPath = "/ph";
String tempPath = "/temp";
String timePath = "/timestamp";
String parentPath;
FirebaseJson json;
// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.cl.pool.ntp.org");

int timestamp;
//Neopixel pins and object
const int  neopin = 26;
const int  numpixel = 1;
Adafruit_NeoPixel pixels(numpixel, neopin, NEO_GRB + NEO_KHZ800);
// local sd variable
char filenameCSV[25] = "";
String datoSD;
int measurementNumber = 0;

// Muestreo
unsigned long previousMillis = millis();
unsigned long previousMillis2 = millis();
unsigned long previousMillis3 = millis();
unsigned long currentMillis3 = millis();
unsigned long currentMillis4 = millis();
unsigned long preMiliMenu = millis();
unsigned long muestreo = 120000; //2 minutos cada cuanto guarda y manda
unsigned long intervalwifi = 300000; //cada 5 minutos revisa si tiene wifi
const int displayMain = 3000; // cada cuanto cambia el mostrar el ph
const int backMenu = 60000; //cada cuanto tiempo vuelve solo al menu principal
int muestMenu = 1;
int muestMenub = 1;
int enter = 0;
bool confirm = false;
bool temp = false;
//SD
#define SD_CS   5
#define SD_SCLK   18
#define SD_MISO   19
#define SD_MOSI   23

void setup() {
  // Serial init
  Serial.begin(115200);
  // Init and function btn_ENTER pressed
  btn_ENTER.begin();
  btn_NEXT.begin();
  btn_ENTER.onPressed(onPressed);
  btn_NEXT.onPressed(onPressed2);
  //Neopixel setup
  pixels.begin();
  // LCD INIT
  lcd.init();
  lcd.backlight();
  changePixel(255, 0, 0);
  lcd.setCursor(0, 0);
  lcd.print("Ducasse " + ver);
  delay(2000);
  // lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Setup Wifi          ");

  // lcd.clear();
  String wifiName = "Estanque " + numeroID + " WM";
  lcd.setCursor(0, 2);
  lcd.print("AP: ");
  lcd.print(wifiName);
//  lcd.setCursor(0, 3);
//  lcd.print();
  delay(2000);
  // WIFI MANAGER
  const char *menu[] = {"wifi", "param", "restart", "exit"}; // Disabled infobtn
  wm.setMenu(menu, 4);
  wm.setConnectTimeout(60); // tiempo en segundo
  bool res;
  res = wm.autoConnect(wifiName.c_str());
  if (!res)
  {
    Serial.println("Failed to connect");
    setupcheck("Wifi", "Error","");
  }
  else
  {
    setupcheck("Wifi", "Ok",WiFi.SSID());
    Serial.println("connected");
      delay(2000);
  }
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  setupcheck("DB connect", "Checking", WiFi.macAddress());
  // Firebase setup
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(1000);
    counter++;
    if (counter == 180)
    {
      setupcheck("DB connect", "Error", "");
      ESP.restart();
    }
  }
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  setupcheck("DB connect", "OK","");
  databasePath = "/UsersData/" + uid + "/readings";
  // ntp setup
  timeClient.begin();
  //SD setup
  SPI.begin(SD_SCLK , SD_MISO, SD_MOSI);
  setupcheck("Mount local DB ", "SD CHECK", "");
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    setupcheck("Mount local DB", "SD FAIL", "");
    // return;
  } else {
    int n = 0;
    snprintf(filenameCSV, sizeof(filenameCSV), "/datos%03d.csv", n); // includes a three-digit sequence number in the file name
    while (SD.exists(filenameCSV)) {
      n++;
      snprintf(filenameCSV, sizeof(filenameCSV), "/datos%03d.csv", n);
    }
    File file = SD.open(filenameCSV, FILE_READ);
    Serial.println(filenameCSV);
    file.close();
    // Mount spiff memory
    setupcheck("Mount local DB", "SD: " + String(filenameCSV), "");
    delay(2000);
  }
  delay(2000);
  setupcheck("Mount local cal", "Mounting","");
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
  {
    Serial.println("LittleFS Mount Failed");
    setupcheck("Mount local cal", "Error", "");
  }
  // Check Calibration PH4
  readFile(LittleFS, "/r1.txt");
  Serial.println(" dato recuperado: " + value);
  setupcheck("Calibracion check", "Ph4.0 " + value, "");
  R1 = value.toFloat();
  Serial.println(" calibracion recuperada: " + String(R1));
  delay(2000);
  // Check Calibration PH7

  readFile(LittleFS, "/r2.txt");
  Serial.println(" dato recuperado: " + value);
  setupcheck("Calibracion check", "Ph7.0 " + value, "");
  R2 = value.toFloat();
  Serial.println(" calibracion recuperada: " + String(R2));
  delay(2000);
  // Check Calibration PH10

  readFile(LittleFS, "/r3.txt");
  Serial.println(" dato recuperado: " + value);
  setupcheck("Calibracion check", "Ph10.0 " + value, "");
  R3 = value.toFloat();
  Serial.println(" calibracion recuperada: " + String(R3));
  delay(2000);

  //check tiempo de menu
  readFile(LittleFS, "/muestreo.txt");
  Serial.println(" dato recuperado: " + value);

  muestreo = value.toInt(); //cambiar a muestreo directamente
   setupcheck("Muestreo recuperado:", String(muestreo / 60000), "");
  Serial.println(" Muestreo recuperado: " + String(muestreo / 60000));
  delay(1000);
  if (muestreo < 120000) {
    muestreo = 120000;
    setupcheck("T muestreo", "minutos:" + String(muestreo / 60000), "por defecto");
  }
 
  

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (sensors.getDeviceCount() == 0) {
    temp = false;
    setupcheck("Sensor temp", "NO", "");
  } else {
temp = true;
setupcheck("Sensor temp", "OK", "");
  }

}

void loop() {
  // put your main code here, to run repeatedly:














}


// Función getPH: Mide el pH del agua a través de un sensor de pH. Obtiene el valor del sensor y luego calcula el pH utilizando las constantes de calibración R1, R2 y R3.
void getPH()
{
  int sensorValue = analogRead(sensorph);
  float pH;

  if (sensorValue <= R1)
  {
    pH = ph4 + (sensorValue - R1) * (ph7 - ph4) / (R2 - R1);
  }
  else if (sensorValue <= R2)
  {
    pH = ph7 + (sensorValue - R2) * (ph4 - ph7) / (R1 - R2);
  }
  else if (sensorValue <= R3)
  {
    pH = ph7 + (sensorValue - R2) * (ph10 - ph7) / (R3 - R2);
  }
  else
  {
    pH = ph10 + (sensorValue - R3) * (ph7 - ph10) / (R2 - R3);
  }

  globalPh = pH;
}
