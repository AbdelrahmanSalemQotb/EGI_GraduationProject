#include <Wire.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>

#define REPORTING_PERIOD_MS     1000

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float object;
int BPM,SPO2;

char auth[] = "qXCkVaXrY7nfQW1Ovz4-8dy_7qpKDVjQ";             // You should get Auth Token in the Blynk App.
char ssid[] = "mostafa";                              // Your WiFi credentials.
char pass[] = "000011122";

bool flag=0;
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
//    BPM = pox.getHeartRate();
//    SPO2 = pox.getSpO2();
}

void setup()
{
    Blynk.begin(auth, ssid, pass);
    Serial.begin(115200);
    if (!mlx.begin(0x5A)) 
    {
      Serial.println("temp");
    }
    Wire.setClock(100000UL);
    Serial.print("Initializing pulse oximeter..");
    
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
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
            
            object=mlx.readObjectTempC(); 
            (object>0)&&(object<100)? : object=0;
            Serial.print("Object = "); Serial.print(object);
            Serial.println();
            flag=1;
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
