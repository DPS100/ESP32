#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "sensor.h"
#include <Arduino.h>

class LightSensor : public Sensor {
    public:
        const int lightPin = A3;

        LightSensor() : Sensor("LightSensor") {
            pinMode(lightPin, INPUT);
        }

        float takeMeasurement() override {
            return (float)analogRead(lightPin);
        }
};

#endif