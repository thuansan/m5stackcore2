#include <Wire.h>
#include <SHTSensor.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <function/wifi.h>

SHTSensor sht(SHTSensor::SHT4X); // For SHT40
Adafruit_BMP280 bmp;

void init_sht(void)
{
    if (sht.init())
    {
        Serial.println("SHT40 initialized successfully!");
    }
    else
    {
        Serial.println("Failed to initialize SHT40. Check wiring!");
    }

    sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM);
}

void init_bmp(void)
{
    if (!bmp.begin(0x76))
    {
        Serial.print("Could not find a valid BMP280 sensor, check wiring!");
        while (1)
            ;
    }
    else
    {
        Serial.println("BMP280 initialized successfully!");
    }
}

void printTemperature(float temperature)
{
    Serial.print("Temperature = ");
    Serial.print(temperature, 2);
    Serial.println(" °C");
}

void printHumidity(float humidity)
{
    Serial.print("Humidity = ");
    Serial.print(humidity, 2);
    Serial.println(" %");
}

void printPressure(float pressure)
{
    Serial.print("Pressure = ");
    Serial.print(pressure / 100.0F);
    Serial.println("hPa");
}
