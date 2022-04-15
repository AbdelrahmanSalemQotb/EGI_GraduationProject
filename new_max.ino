#include <Wire.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

#define LCD_I2C_Address     0x27

#define Buzzer_Pin          5

LiquidCrystal_I2C lcd(LCD_I2C_Address, 20, 4);  

#define REPORTING_PERIOD_MS     300

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float object;
int BPM,SPO2;

char auth[] = "qXCkVaXrY7nfQW1Ovz4-8dy_7qpKDVjQ";             // You should get Auth Token in the Blynk App.
//char ssid[] = "mostafa";                              // Your WiFi credentials.
//char pass[] = "000011122";
char ssid[] = "Salem";                              // Your WiFi credentials.
char pass[] = "5522333123";

byte flag=0;
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
//    BPM = pox.getHeartRate();
//    SPO2 = pox.getSpO2();
}

void LCD_Setup(void)
{
  lcd.init();
  
  // Make sure backlight is on       
  lcd.backlight();
  
  // create a new characters
//  lcd.createChar(0, Heart);
  
  // Clears the LCD screen
  lcd.clear();
  
  
  lcd.setCursor(0, 1);
  lcd.print("SPO2  = ");
  
  lcd.setCursor(0, 2);
  lcd.print("HR    = ");

  lcd.setCursor(0, 3);
  lcd.print("TEMP  = ");
  lcd.noBlink();
}

void LCD_Report_Data (void)
{
   lcd.clear();
  
  
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
    Blynk.begin(auth, ssid, pass);

    LCD_Setup();   
    Serial.begin(115200);
    if (!mlx.begin(0x5A)) 
    {
      Serial.println("temp");
    }
    Wire.setClock(100000UL);
    Serial.print("Initializing pulse oximeter..");

    // init buzzer
    pinMode(Buzzer_Pin,OUTPUT);
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
    } else {
        Serial.println("SUCCESS");
    }
//    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    Blynk.run();
    
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        if(flag==0)
          {
            Serial.print("Heart rate:");
            BPM = pox.getHeartRate();
            Serial.print(BPM);
            Serial.print("bpm / SpO2:");
            SPO2 = pox.getSpO2();
            Serial.print(SPO2);
            Serial.println("%");
            if (SPO2>100&& SPO2<0)
            {
              SPO2=0;
            }
            
            flag=1;
          }
        else if ( flag ==1)
        {
            object=mlx.readObjectTempC(); 
            (object>0)&&(object<100)? : object=0;
            Serial.print("Object = "); Serial.print(object);
            Serial.println();
            flag=2;
        }
        else if (flag ==2)
        {
          LCD_Report_Data();
          flag=3;

        }
        else if ( flag == 3 )
        {
              if ((SPO2 > 0 && SPO2 < 90) || ( BPM > 100) || (object > 37))
              {
                digitalWrite(Buzzer_Pin,HIGH);
              }
              else
              {
                digitalWrite(Buzzer_Pin,LOW);
              }
              flag=4;
        
        }
        else
        {
            Blynk.virtualWrite(V3, BPM);
            Blynk.virtualWrite(V4, SPO2);
            Blynk.virtualWrite(V5, object);
            
            flag=0;
        }

          tsLastReport = millis();

    }
}
