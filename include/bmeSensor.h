#ifndef BME_SENSOR_H
#define BME_SENSOR_H

#include <Zanshin_BME680.h>
#include <Arduino.h>
#include "multiMeasurementSensor.h"

class BMESensor : public MultiMeasurementSensor<int32_t> {
    public:

        BME680_Class BME680;
        // int32_t temp, humidity, pressure, gas;
        static const std::vector<String> names ;

        BMESensor() : MultiMeasurementSensor("BME680", names) {
            Serial.println("Creating BME");
            while (!BME680.begin(I2C_STANDARD_MODE)) {
            Serial.println(F("BME680 init failed"));
                delay(1000);
            }
            BME680.setOversampling(TemperatureSensor, Oversample16);
            BME680.setOversampling(HumiditySensor, Oversample16);
            BME680.setOversampling(PressureSensor, Oversample16);
            BME680.setIIRFilter(IIR4);
            BME680.setGas(320, 150);
        }

        void takeMeasurements() override {
            BME680.getSensorData(readings[0], readings[1], readings[2], readings[3]);
            Serial.println(readings[0]);
        }

        
};

const std::vector<String> BMESensor::names = {"temp", "humidity", "pressure", "gas"};

#endif