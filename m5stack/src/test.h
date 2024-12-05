#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Replace these with your network credentials
#define WIFI_SSID "iPhone 36"
#define WIFI_PASSWORD "012345678"

// Adafruit IO credentials
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883          // Use 8883 for secure MQTT
#define AIO_USERNAME "doquangkhanh2" // Your Adafruit IO username
#define AIO_KEY "aio_RAVd39qB4SHcW7lWX9g33VUnEIfF"

// Create a WiFiClient and Adafruit MQTT client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feed to publish data to
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

void connectWiFi()
{
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        lv_obj_add_flag(ui_Image5, LV_OBJ_FLAG_HIDDEN);
    }
    Serial.println("\nWiFi connected!");
}

void connectMQTT()
{
    Serial.print("Connecting to MQTT");
    while (mqtt.connect() != 0)
    {
        Serial.println(mqtt.connectErrorString(mqtt.connect()));
        delay(5000);
    }
    Serial.println("\nMQTT connected!");
}

