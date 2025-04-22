#ifndef SENSOR
#define SENSOR

#include <Arduino.h>

class Sensor {
    public:
        Sensor(String description) : description(description), enabled(true) {}
        Sensor(String description, bool enabled) : description(description), enabled(enabled) {}
        virtual float takeMeasurement() = 0;
        String description;
        const bool enabled;
};

#endif