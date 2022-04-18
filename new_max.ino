#define BLYNK_TEMPLATE_ID "TMPLak1A52NB"
#define BLYNK_DEVICE_NAME "EGI"
#define BLYNK_AUTH_TOKEN "xNSASj75s6HnPpghBgUfpXH8MkvoLlXj"

// Uncomment for debugging
#define Serial_Debug
#define BLYNK_PRINT Serial
#define BLYNK_DEBUG

#include <Wire.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h> 

#define MANAGER_USER            "EGI"
#define MANAGER_PASS            "EGI1718169"
#define REPORTING_PERIOD_MS     350
#define LCD_I2C_Address         0x27
#define Buzzer_Pin              D5


LiquidCrystal_I2C lcd(LCD_I2C_Address, 20, 4);  
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
PulseOximeter pox;

int   BPM   ;
int   SPO2  ;
byte flag = 0;
int object ;
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
  0b00000
};

String ssid = "Salem";                              // Your WiFi credentials.
String pass = "5522333123";



// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  #ifdef Serial_Debug
    Serial.println("Beat!");
   #endif
    BPM = pox.getHeartRate();
    SPO2 = pox.getSpO2();
    if (SPO2>100 || SPO2<0)
            {
              SPO2=0;
            }
}

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
void wifiSetup (void)
{
   WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
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
    lcd.print("connect to manager");
    lcd.setCursor(0, 1);
    lcd.print("User > ");
    lcd.print(MANAGER_USER);
    lcd.setCursor(0, 2);
    lcd.print("Pass > ");
    lcd.print(MANAGER_PASS);
    lcd.setCursor(0, 3);
    lcd.print("ip > ");
    lcd.print("192.168.4.1");

    res = wm.autoConnect(MANAGER_USER,MANAGER_PASS); // password protected ap
    
    if(!res) {
        #ifdef Serial_Debug
        Serial.println("Failed to connect");
        #endif
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Failed to connect");
        lcd.setCursor(0, 1);
        lcd.print("");
        
          
    } 
    else {
        ssid = wm.getWiFiSSID();
        pass = wm.getWiFiPass();
        //if you get here you have connected to the WiFi   
        #ifdef Serial_Debug 
        Serial.println("connected... :)");
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
  lcd.createChar(0,Heart);
  // Clears the LCD screen
  lcd.clear(); 
}

void LCD_Report_Data (void)
{
  //////////////////////////////////////////////////////
//  if(WiFi.status() == WL_CONNECTED)
//    {
//      #ifdef Serial_Debug
//      Serial.println("WIFI Connected");
//      #endif
//      lcd.setCursor(0, 0);
//      lcd.print("WIFI  ");
//      lcd.write(1);
//    }
//    else
//    {
//      #ifdef Serial_Debug
//      Serial.println("WIFI Not Connected");
//      #endif
//      lcd.setCursor(0, 0);
//      lcd.print("WIFI  ");
//      lcd.write(2);
//    }
//    
//  if(!Blynk.connect())
//    {
//      #ifdef Serial_Debug
//      Serial.println("Server Not Connected");
//      #endif
//      lcd.print("  Server  ");
//      lcd.write(2);
//    }
//    else
//    {
//      #ifdef Serial_Debug
//      Serial.println("Server Connected");
//      #endif
//      lcd.print("  Server  ");
//      lcd.write(1);
//    }
//  ////////////////////////////////////////////////
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
  lcd.setCursor(17, 2);
  lcd.write(0);

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
if(WiFi.status() == WL_CONNECTED){
    Blynk.config(BLYNK_AUTH_TOKEN);
    if(!Blynk.connect())
    {
      #ifdef Serial_Debug
      Serial.println("Server Not Connected");
      #endif
      lcd.setCursor(0, 1);
      lcd.print("Server Not Connected");
      delay (1000);
    }
    else
    {
      #ifdef Serial_Debug
      Serial.println("Server Connected");
      #endif
      lcd.setCursor(0, 0);
      lcd.print("Server Connected");
      delay (1000);
    }
}
    

    
    if (!mlx.begin(0x5A)) 
    {
       #ifdef Serial_Debug
      Serial.println("mlx90614 not found");
      #endif
    }
    Wire.setClock(100000UL);
    #ifdef Serial_Debug
    Serial.print("Initializing pulse oximeter..");
    #endif
    // init buzzer
    pinMode(Buzzer_Pin,OUTPUT);

    
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        #ifdef Serial_Debug
          Serial.println("FAILED");
        #endif
          
    } else {
        #ifdef Serial_Debug
          Serial.println("SUCCESS");
        #endif
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  if (Blynk.connected())
  {
    Blynk.run();
  } 
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        if(flag==0)
          {
            BPM = pox.getHeartRate();
            SPO2 = pox.getSpO2();
            
            #ifdef Serial_Debug
            Serial.print("Heart rate:");
            Serial.print(BPM);
            Serial.print("bpm / SpO2:");
            Serial.print(SPO2);
            Serial.println("%");
            #endif
            if (SPO2>100 || SPO2<0)
            {
              SPO2=0;
            }
            
            flag=1;
          }
        else if ( flag ==1)
        {
              if ((SPO2 > 0 && SPO2 < 90) || ( BPM > 100) || (object > 37))
              {
                digitalWrite(Buzzer_Pin,HIGH);
                #ifdef Serial_Debug
                Serial.println(" Buzzer ON");
                #endif
              }
              else
              {
                digitalWrite(Buzzer_Pin,LOW);
                #ifdef Serial_Debug
                Serial.println(" Buzzer OFF");
                #endif

              }
            flag=2;
        }
        else if (flag == 2)
        {
          LCD_Report_Data();
          flag=3;

        }
        else if ( flag == 3 )
        {
            object = mlx.readObjectTempC();
//            ambiant = mlx.readAmbientTempC();
            #ifdef Serial_Debug
            Serial.print("Object = "); Serial.print(object);
            Serial.println();
//            Serial.print("ambiant = "); Serial.print(ambiant);
//            Serial.println();
            #endif

              flag=4;
        }
        else if (Blynk.connected() && flag == 4 )
        {
            Blynk.virtualWrite(V3, BPM);
            Blynk.virtualWrite(V4, SPO2);
            Blynk.virtualWrite(V5, object);
            
            flag=0;
        }
        else
        {
          flag=0;
          #ifdef Serial_Debug
          Serial.println("Server disconnected");
          #endif
          lcd.setCursor(0, 0);
          lcd.print("Server disconnected");

        }

          tsLastReport = millis();

    }
}
