#ifndef DEBUG_SENSOR_H
#define DEBUG_SENSOR_H

#include "sensor.h"

class DebugSensor : public Sensor {
    public:
        DebugSensor() : Sensor("DebugSensor") {}
        DebugSensor(bool enabled) : Sensor("DebugSensor", enabled) {}

        float takeMeasurement() override {
            return 25.0;  // Just a placeholder value
        }
};

#endif