#include "module.h"
#include <Arduino.h>
#include <stdio.h>
#include <string>
#include <ArduinoJson.h>

Module* Module::instance = nullptr;

Module::Module() : Module(true) {}

Module::Module(bool deferConnection) : wifiClient(), mqttClient(wifiClient), sensors(), http(wifiClient, broker.c_str(), 8000) {
    if(!deferConnection) {
        this->connect();
    }
}

Module* Module::getInstance() {
    return getInstance(true);
}

Module* Module::getInstance(bool deferConnection) {
    if(Module::instance == nullptr) {
        Module::instance = new Module(deferConnection);
    }
    return Module::instance;
}

Module* Module::connect() {
    // attempt to connect to WiFi network:
    delay(500);
    
    Serial.println("Attempting to connect to WPA SSID: \n");
    Serial.println(ssid.c_str());
    if(WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid.c_str(), pass.c_str());
        // connect();
    }
    Serial.println("\nYou're connected to the network\n");

    uint8_t mac_hardware[6];
    WiFi.macAddress(mac_hardware);  // fills mac[0..5]

    char macStr[18];
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
        mac_hardware[0], mac_hardware[1], mac_hardware[2], mac_hardware[3], mac_hardware[4], mac_hardware[5]);

    mac = String(macStr);
    Serial.println(mac); 
    topic = "sensor_service/" + mac + "/";

    // You can provide a username and password for authentication
    // mqttClient.setUsernamePassword("username", "password");

    if(!registeredBackend) {
        Serial.println("Attempting to connect to backend service...\n");

        // Construct full POST body
        JsonDocument doc;
        doc["hw_id"] = mac.c_str();
        doc["sensor_count"] = this->sensors.size();

        int i = 0;
        for (Sensor* sensor : this->sensors) {
            doc["sensor_descriptions"][i++] = sensor->description;
        }

        String jsonBody;
        serializeJson(doc, jsonBody);

        // Prepare request
        http.beginRequest();
        Serial.println("Began\n");
        http.post("/discover-module");  // Just the path, since host/port already in constructor
        Serial.println("Posted\n");
        http.sendHeader("Content-Type", "application/json");
        http.sendHeader("Accept", "application/json");
        http.sendHeader("Content-Length", jsonBody.length());
        Serial.println("Header\n");
        http.beginBody();
        Serial.println("Body begin\n");
        http.print(jsonBody);
        Serial.println("body done\n");
        http.endRequest();
        Serial.println("Finished HTTP\n");

        // Get response
        int statusCode = http.responseStatusCode();
        Serial.println("HTTP Response code:\n");
        Serial.println(statusCode);

        registeredBackend = (statusCode == 200 || statusCode == 409);
        if(!registeredBackend) exit(1);

        // Optionally: read and print response
        String response = http.responseBody();
        Serial.println("Response: \n");
        Serial.println(response.c_str());

        // connect();
    }
    

    Serial.println("\nAttempting to connect to the MQTT broker: \n");
    if(!mqttClient.connected()) {
        Serial.println(broker.c_str());
        Serial.println("\n");
    
        mqttClient.setServer(this->broker.c_str(), this->port);
    
        if (!mqttClient.connect(mac.c_str())) {
            Serial.println("MQTT connection failed!\n");
            // connect();
        }
    }
    Serial.println("You're connected to the MQTT broker!\n");

    return this;
}

Module* Module::registerSensor(Sensor* sensor) {
    this->sensors.push_back(sensor);
    return this;
}

bool Module::sendMessage(int sensorID, float measurement, int time) {
    // TODO test positive, negative, NAN
    this->mqttClient.publish((this->topic + String(std::to_string(sensorID).c_str())).c_str(), (std::to_string(time) + ":" + std::to_string(measurement)).c_str());
    Serial.println(sensorID);
    Serial.println(std::to_string(measurement).c_str());
    return true;
}

bool Module::broadcast() {
    // TODO sleep
    this->mqttClient.loop();


    for(int i = 0; i < this->sensors.size(); i++) {
        Sensor* sensor = this->sensors[i];
        // TODO current time each
        // TODO millis() rollover - manage here or db
        int time = -1;
        this->sendMessage(i, sensor->takeMeasurement(), time);
    }

    delay(30000);

    return false;
}

Module* Module::blink(int duration, int freq) {
    for(int i = 0; i < duration; i++) {
        this->mqttClient.loop();
        for(int j = 0; j < freq; j++) {
            Serial.println("LED_ON\n");
            delay(1000 / freq / 2);
            Serial.println("LED_OFF\n");
            delay(1000 / freq / 2);
        }
    }

    return this;
}
// #include "module.h"
// #include <Arduino.h>
// #include <stdio.h>
// #include <string>
// #include <ArduinoJson.h>

// Module* Module::instance = nullptr;

// Module::Module() : Module(false) {}

// Module::Module(bool deferConnection) : wifiClient(), mqttClient(wifiClient), sensors(), http() {
//     if(!deferConnection) {
//         this->connect();
//     }
// }

// Module* Module::getInstance() {
//     return getInstance(false);
// }

// Module* Module::getInstance(bool deferConnection) {
//     if(Module::instance == nullptr) {
//         Module::instance = new Module(deferConnection);
//     }
//     return Module::instance;
// }

// Module* Module::connect() {
//     // attempt to connect to WiFi network:
//     delay(500);
    
//     printf("Attempting to connect to WPA SSID: \n");
//     printf(ssid.c_str());
//     printf("\n");
//     if(WiFi.status() != WL_CONNECTED) {
//         WiFi.begin(ssid.c_str(), pass.c_str());
//         connect();
//     }
//     printf("\nYou're connected to the network\n");

//     // You can provide a username and password for authentication
//     // mqttClient.setUsernamePassword("username", "password");

//     if(!registeredBackend) {
//         printf("Attempting to connect to backend service...\n");
//         http.begin(this->wifiClient, "http://" + broker + ":8000/discover-module");
//         http.addHeader("Content-Type", "application/json");
//         http.addHeader("accept", "application/json");

//         JsonDocument doc;
//         doc["hw_id"] = mac.c_str();
//         doc["sensor_count"] = this->sensors.size();
//         int i = 0;
//         for(Sensor* sensor : this->sensors) {
//             sensor->description;
//             doc["sensor_descriptions"][++i] = sensor->description;
//         }

//         String jsonBody;
//         serializeJson(doc, jsonBody);
//         int httpResponseCode = http.POST(jsonBody);
//         printf("HTTP Response code: \n");
//         printf(std::to_string(httpResponseCode).c_str());
//         http.end();
//         registeredBackend = httpResponseCode == 200 || httpResponseCode == 409;
//         connect();
//     }
    

//     printf("\nAttempting to connect to the MQTT broker: \n");
//     if(!mqttClient.connected()) {
//         printf(broker.c_str());
//         printf("\n");
    
//         mqttClient.setServer(this->broker.c_str(), this->port);
    
//         if (!mqttClient.connect(mac.c_str())) {
//             printf("MQTT connection failed!\n");
//             connect();
//         }
//     }
//     printf("You're connected to the MQTT broker!\n");

//     return this;
// }

// Module* Module::registerSensor(Sensor* sensor) {
//     this->sensors.push_back(sensor);
//     return this;
// }

// bool Module::sendMessage(int sensorID, float measurement, int time) {
//     // TODO test positive, negative, NAN
//     this->mqttClient.publish((this->topic + String(std::to_string(sensorID).c_str())).c_str(), (std::to_string(time) + ":" + std::to_string(measurement)).c_str());
//     return true;
// }

// bool Module::broadcast() {
//     // TODO sleep
//     this->mqttClient.loop();
//     delay(1000);
//     printf("Spinning\n");


//     for(int i = 0; i < this->sensors.size(); i++) {
//         Sensor* sensor = this->sensors[i];
//         // TODO current time each
//         // TODO millis() rollover - manage here or db
//         int time = -1;
//         this->sendMessage(i, sensor->takeMeasurement(), time);
//     }

//     return false;
// }

// Module* Module::blink(int duration, int freq) {
//     for(int i = 0; i < duration; i++) {
//         this->mqttClient.loop();
//         for(int j = 0; j < freq; j++) {
//             printf("LED_ON\n");
//             delay(1000 / freq / 2);
//             printf("LED_OFF\n");
//             delay(1000 / freq / 2);
//         }
//     }

//     return this;
// }