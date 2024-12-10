#include <Wire.h>
#include <SHTSensor.h>
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <function/wifi.h>
#include <SensirionI2CScd4x.h>

SHTSensor sht(SHTSensor::SHT4X); // For SHT40
Adafruit_BMP280 bmp;
SensirionI2CScd4x scd40;

void printUint16Hex(uint16_t value)
{
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

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

void init_scd(void){
    uint16_t error;
    char errorMessage[256];

    scd40.begin(Wire);

    // stop potentially previously started measurement
    error = scd40.stopPeriodicMeasurement();
    if (error)
    {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd40.getSerialNumber(serial0, serial1, serial2);
    if (error)
    {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    else
    {
        printSerialNumber(serial0, serial1, serial2);
    }

    // Start Measurement
    error = scd40.startPeriodicMeasurement();
    if (error)
    {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println("Waiting for first measurement... (5 sec)");
}

void printCo2(uint16_t co2){
    Serial.print("CO2 = ");
    Serial.println(co2);

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
    Serial.print(pressure);
    Serial.println("hPa");
}

void updateValue(){
    // Publish dummy temperature data

    uint16_t error;
    char errorMessage[256];

    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    bool isDataReady = false;
    error = scd40.getDataReadyFlag(isDataReady);
    if (error)
    {
        Serial.print("Error trying to execute getDataReadyFlag(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }
    if (!isDataReady)
    {
        return;
    }
    error = scd40.readMeasurement(co2, temperature, humidity);
    if (error)
    {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    else if (co2 == 0)
    {
        Serial.println("Invalid sample detected, skipping.");
    }
    else
    {
        Serial.println("Co2:");
        Serial.print(co2);
    }

    co2_publish(co2);
    char buffer[20];
    //char *temp_char = buffer;
    sprintf(buffer, "%u", co2);
    lv_label_set_text(ui_Label8, buffer);
    lv_refr_now(NULL);

    temperature = sht.getTemperature();
    humidity = sht.getHumidity();
    float pressure = (bmp.readPressure() / 100.0F);


    temperature_publish(temperature);
    printTemperature(temperature);
    snprintf(buffer, sizeof(buffer), "%.1f", temperature);
    lv_label_set_text(ui_temp, buffer);
    lv_refr_now(NULL);

    humidity_publish(humidity);
    printHumidity(humidity);
    snprintf(buffer, sizeof(buffer), "%.1f", humidity);
    lv_label_set_text(ui_humi, buffer);
    lv_refr_now(NULL);

    pressure_publish(pressure);
    printPressure(pressure);
    snprintf(buffer, sizeof(buffer), "%.2f", pressure);
    lv_label_set_text(ui_press, buffer);
    lv_refr_now(NULL);
}