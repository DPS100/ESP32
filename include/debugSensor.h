#ifndef DEBUG_SENSOR_H
#define DEBUG_SENSOR_H

#include "sensor.h"

class DebugSensor : public Sensor {
    public:
        DebugSensor() : Sensor("DebugSensor") {}

        float takeMeasurement() override {
            return 25.0;  // Just a placeholder value
        }
};

#endif