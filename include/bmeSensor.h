#ifndef BME_SENSOR_H
#define BME_SENSOR_H

#include <Zanshin_BME680.h>
#include <Arduino.h>

class BMESensor {
    public:

        static

        BME680_Class BME680;
        int32_t temp, humidity, pressure, gas;
        int8_t measured = 0b0000;

        BMESensor() {
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

        float takeMeasurement() {
            BME680.getSensorData(temp, humidity, pressure, gas);
        }

        
};

#endif