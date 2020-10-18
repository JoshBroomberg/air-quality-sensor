/* Test sketch for Adafruit PM2.5 sensor with UART or I2C */

#include "Adafruit_PM25AQI.h"

// If your PM2.5 is UART only, for UNO and others (without hardware serial) 
// we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(2, 3);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

unsigned long lastBlockTime;
unsigned long lastBlockMilis;

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(1000);

  // Initialize software serial
  pmSerial.begin(9600);

  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");

  lastBlockTime = millis();
}

void loop() {
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    delay(1);
    return;
  }

  lastBlockMilis = millis() - lastBlockTime;
  lastBlockTime = millis();

  Serial.println(F("data start"));
  Serial.print(F("time: ")); Serial.println(lastBlockMilis/1000.0);
  Serial.print(F("s10: ")); Serial.println(data.pm10_standard);
  Serial.print(F("s25: ")); Serial.println(data.pm25_standard);
  Serial.print(F("s100: ")); Serial.println(data.pm100_standard);
  Serial.print(F("e10: ")); Serial.println(data.pm10_env);
  Serial.print(F("e25: ")); Serial.println(data.pm25_env);
  Serial.print(F("e100: ")); Serial.println(data.pm100_env);
  Serial.print(F("c03: ")); Serial.println(data.particles_03um);
  Serial.print(F("c05: ")); Serial.println(data.particles_05um);
  Serial.print(F("c10: ")); Serial.println(data.particles_10um);
  Serial.print(F("c25: ")); Serial.println(data.particles_25um);
  Serial.print(F("c50: ")); Serial.println(data.particles_50um);
  Serial.print(F("c100: ")); Serial.println(data.particles_100um);
  Serial.println(F("data end"));
}
