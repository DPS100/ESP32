/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <Arduino.h>
#include "module.h"
#include "lightSensor.h"
#include "co2Sensor.h"
#include "debugSensor.h"

Module* module = nullptr;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("BEEJ");
    delay(2000);
    Serial.println("BEEJ2");

    module = Module::getInstance(true)
        ->registerSensor(new DebugSensor())
        ->registerSensor(new LightSensor()) // TODO singleton on pins? Throw error? Throw errors to broker?
        ->registerSensor(new CO2Sensor())
        ->connect() // TODO connect to broker BEFORE registering to throw errors to server...
    ;
}

void loop() {
    Serial.println("Begun");
    module->broadcast(); // TODO broker reconnect attempt if disconnected
}