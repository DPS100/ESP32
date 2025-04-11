#ifndef MODULE
#define MODULE

#include <PubSubClient.h>
#include <vector>
#include "sensor.h"
#include "secrets.h"

// #if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
//   #include <WiFi.h>
//   #include <HTTPClient.h>
// #elif defined(ARDUINO_SAMD_MKRWIFI1010)
  #include <WiFiNINA.h>
  #include <ArduinoHttpClient.h>
// #else
//   #error "Unsupported board"
// #endif

#define HW_REG_TOPIC "hw_reg_service"
#define SENSOR_PUB_TOPIC "sensor_service"

class Module{
    public:
        static Module* getInstance();
        static Module* getInstance(bool deferConnection);
        Module* registerSensor(Sensor* sensor);
        Module* blink(int duration, int freq);
        Module* connect();
        bool broadcast();
        String ssid = SECRET_SSID;
        String pass = SECRET_PASS;

    private:
        Module();
        Module(bool deferConnection);
        static Module* instance;
        std::vector<Sensor*> sensors;
        PubSubClient mqttClient;
        WiFiClient wifiClient;
        // HTTPClient http;
        HttpClient http;
        String broker = BROKER;
        int        port     = 1883;
        String mac = "NO:MAC";
        String topic = "sensor_service/" + mac + "/";
        // void callback(char* topic, byte* payload, unsigned int length);
        bool sendMessage(int sensorID, float measurement, int time);
        bool registeredBackend = true;
};

#endif

// #ifndef MODULE
// #define MODULE

// #include <PubSubClient.h>
// #include <HTTPClient.h>
// #include <WiFi.h>
// #include <vector>
// #include "sensor.h"
// #include "secrets.h"

// #define HW_REG_TOPIC "hw_reg_service"
// #define SENSOR_PUB_TOPIC "sensor_service"

// class Module{
//     public:
//         static Module* getInstance();
//         static Module* getInstance(bool deferConnection);
//         Module* registerSensor(Sensor* sensor);
//         Module* blink(int duration, int freq);
//         Module* connect();
//         bool broadcast();
//         String ssid = SECRET_SSID;
//         String pass = SECRET_PASS;

//     private:
//         Module();
//         Module(bool deferConnection);
//         static Module* instance;
//         std::vector<Sensor*> sensors;
//         WiFiClient wifiClient;
//         PubSubClient mqttClient;
//         HTTPClient http;
//         String broker = BROKER;
//         int        port     = 1883;
//         String mac = WiFi.macAddress();
//         String topic  = "sensor_service/" + mac + "/";
//         // void callback(char* topic, byte* payload, unsigned int length);
//         bool sendMessage(int sensorID, float measurement, int time);
// };

// #endif