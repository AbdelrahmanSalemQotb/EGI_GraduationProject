
// create your own Blynk app Project then add your information below
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

//	// Uncomment for debugging
// #define Serial_Debug
// #define BLYNK_PRINT Serial
// #define BLYNK_DEBUG

#include <Wire.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>

// Configure WIFI_Hotspot User and Pass
#define MANAGER_USER "EGI"
#define MANAGER_PASS "123456789"


#define REPORTING_PERIOD_MS 350
#define LCD_I2C_Address 0x27
#define Buzzer_Pin D5
#define IR_Pin D6

LiquidCrystal_I2C lcd(LCD_I2C_Address, 20, 4);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;

int BPM;
int SPO2;
byte flag = 0;
float object;
int ambiant;
uint32_t tsLastReport = 0;

byte Heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000};

String ssid = ""; // Your WiFi credentials.
String pass = "";

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
#ifdef Serial_Debug
  Serial.println("Beat!");
#endif
  BPM = pox.getHeartRate();
  SPO2 = pox.getSpO2();
  if (SPO2 > 100 || SPO2 < 0)
  {
    SPO2 = 0;
  }
  if (digitalRead(IR_Pin))
  {
    SPO2 = 0;
    BPM = 0;
  }
}
void wifiSetup(void)
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //    wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  lcd.setCursor(0, 0);
  lcd.print("Connect To Manager");
  lcd.setCursor(0, 1);
  lcd.print("User > ");
  lcd.print(MANAGER_USER);
  lcd.setCursor(0, 2);
  lcd.print("Pass > ");
  lcd.print(MANAGER_PASS);
  lcd.setCursor(0, 3);
  lcd.print("IP > ");
  lcd.print("192.168.4.1");

  res = wm.autoConnect(MANAGER_USER, MANAGER_PASS); // password protected ap

  if (!res)
  {
#ifdef Serial_Debug
    Serial.println("Failed to connect");
#endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Failed to connect");
    lcd.setCursor(0, 1);
    lcd.print("");
  }
  else
  {
    ssid = wm.getWiFiSSID();
    pass = wm.getWiFiPass();
// if you get here you have connected to the WiFi
#ifdef Serial_Debug
    Serial.println("Connected... :)");
    Serial.println(ssid);
    Serial.println(pass);
#endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WIFI Connected");
  }
}

void LCD_Setup(void)
{
  lcd.init();

  // Make sure backlight is on
  lcd.backlight();
  lcd.createChar(0, Heart);
  // Clears the LCD screen
  lcd.clear();
}

void LCD_Report_Data(void)
{
  lcd.setCursor(0, 1);
  lcd.print("SPO2  = ");

  lcd.setCursor(0, 2);
  lcd.print("HR    = ");

  lcd.setCursor(0, 3);
  lcd.print("TEMP  = ");

  lcd.setCursor(8, 1);
  lcd.print("       ");
  lcd.setCursor(8, 1);
  lcd.print(SPO2);
  lcd.setCursor(12, 1);
  lcd.print('%');

  lcd.setCursor(8, 2);
  lcd.print("       ");
  lcd.setCursor(8, 2);
  lcd.print(BPM);
  lcd.setCursor(12, 2);
  lcd.write(0);
  lcd.print("BPM");

  lcd.setCursor(8, 3);
  lcd.print("       ");
  lcd.setCursor(8, 3);
  lcd.print((int)object);
  lcd.setCursor(12, 3);
  lcd.print((char)0xDF);
  lcd.print("C");
}

void setup()
{
#ifdef Serial_Debug
  Serial.begin(115200);
#endif
  LCD_Setup();
  wifiSetup();

  //    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.config(BLYNK_AUTH_TOKEN);
    if (!Blynk.connect())
    {
#ifdef Serial_Debug
      Serial.println("Server Not Connected");
#endif
      lcd.setCursor(0, 1);
      lcd.print("Server Not Connected");
      delay(1000);
    }
    else
    {
#ifdef Serial_Debug
      Serial.println("Server Connected");
#endif
      lcd.setCursor(0, 0);
      lcd.print("Server Connected");
      delay(1000);
    }
  }

  if (!mlx.begin(0x5A))
  {
#ifdef Serial_Debug
    Serial.println("MLX90614 NOT FOUND");
#endif
  }
  Wire.setClock(100000UL);
#ifdef Serial_Debug
  Serial.print("Initializing pulse oximeter..");
#endif
  // init buzzer
  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(IR_Pin, INPUT);
  // Initialize the PulseOximeter instance
  if (!pox.begin())
  {
#ifdef Serial_Debug
    Serial.println("FAILED");
#endif
  }
  else
  {
#ifdef Serial_Debug
    Serial.println("SUCCESS");
#endif
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Blynk.run();
  }
  // Make sure to call update as fast as possible
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    switch (flag)
    {
    case 0:
      BPM = pox.getHeartRate();
      SPO2 = pox.getSpO2();

      #ifdef Serial_Debug
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print("bpm / SpO2:");
        Serial.print(SPO2);
        Serial.println("%");
      #endif
      if (SPO2 > 100 || SPO2 < 0)
      {
        SPO2 = 0;
      }

      flag = 1;
      break;

    case 1:
      if ((SPO2 > 0 && SPO2 < 90) || (BPM > 100) || ((int)object > 37))
      {
        digitalWrite(Buzzer_Pin, HIGH);
        Blynk.logEvent("critical_value",  
       ((SPO2 > 0 && SPO2 < 90)?(String("SPO2: ") + String(SPO2)):"") +
 (BPM>100?(String("  HR: ") +String(BPM)) :"" ) +
 (((int)object > 37)? (String("\nTemp: ") + String((int)object)):"") );
        #ifdef Serial_Debug
          Serial.println(" Buzzer ON");
        #endif
      }
      else
      {
        digitalWrite(Buzzer_Pin, LOW);
        #ifdef Serial_Debug
          Serial.println(" Buzzer OFF");
        #endif
      }
      flag = 2;
      break;

    case 2:
      if (!digitalRead(IR_Pin))
      {
        object = mlx.readObjectTempC();
      }
      else
      {
        object = 0;
        SPO2 = 0;
        BPM = 0;
      }

      #ifdef Serial_Debug
        Serial.print("Object = ");
        Serial.print(object);
        Serial.println();
      #endif
      flag = 3;
      break;

    case 3:
      LCD_Report_Data();
      flag = 4;
      break;

    case 4:
      if (Blynk.connected())
      {
        Blynk.virtualWrite(V3, BPM);
        Blynk.virtualWrite(V4, SPO2);
        Blynk.virtualWrite(V5, object);
        flag = 0;
      }
      else
      {
      // server not connected ( hence no report )
      flag = 0;
      #ifdef Serial_Debug
        Serial.println("Server disconnected");
      #endif
      lcd.setCursor(0, 0);
      lcd.print("Server disconnected");
      }

      break;

    default:
    break;
    }
    tsLastReport = millis();
  }
}