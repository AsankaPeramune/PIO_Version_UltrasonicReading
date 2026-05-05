#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor
{
private:
    HardwareSerial *_serial;
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _frame[4];

public:
    UltrasonicSensor(HardwareSerial &serial, uint8_t rxPin, uint8_t txPin);

    void init();
    int sensorRead();  // returns distance in mm, -1 if error/no data
};

#endif