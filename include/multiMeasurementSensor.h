#ifndef MULTI_MES_SENSOR
#define MULTI_MES_SENSOR

#include "sensor.h"
#include <vector>
#include <cmath>

class IMultiMeasurementSensor : public Sensor {
    public:
        IMultiMeasurementSensor(String description) : Sensor(description, false) {}
        virtual ~IMultiMeasurementSensor() = default;
        virtual void takeMeasurements() = 0;

        float takeMeasurement() override {
            takeMeasurements();
            return NAN;
        }

        int numSensors;
        std::vector<Sensor*> sensors;
};

template <typename T>
class MultiSensorWrapper : public Sensor {
    public:
        MultiSensorWrapper(String description, T* reading) : Sensor(description), reading(reading) {}
        T* reading;
        float takeMeasurement() override {
            return static_cast<float>(*reading);
        }

};

template <typename T>
class MultiMeasurementSensor : public IMultiMeasurementSensor {
    public:

        MultiMeasurementSensor(String description, std::vector<String> sensorDescriptions) : IMultiMeasurementSensor(description) {
            numSensors = sensorDescriptions.size();
            readings = std::vector<T>(numSensors);
            sensors = std::vector<Sensor*>(numSensors);

            for(int i = 0; i < numSensors; i++) {
                sensors[i] = new MultiSensorWrapper<T>(description + "_" + sensorDescriptions[i], &readings[i]);
            }
        }

        void takeMeasurements() override = 0;
        std::vector<T> readings;
};


#endif