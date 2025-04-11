#ifndef SENSOR
#define SENSOR

#include <Arduino.h>

class Sensor {
    public:
        Sensor(String description) : description(description) {}
        virtual float takeMeasurement() = 0;
        String description;
};

#endif