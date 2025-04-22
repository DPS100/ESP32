#ifndef CO2_SENSOR_H
#define CO2_SENSOR_H

#include "sensor.h"
#include <Arduino.h>

class CO2Sensor : public Sensor {
    public:
        const int gasConcPin = A0;

        CO2Sensor() : Sensor("CO2Sensor") {
            pinMode(gasConcPin, INPUT);
        }

        float takeMeasurement() override {
            return (float)analogRead(gasConcPin);
        }
};

#endif