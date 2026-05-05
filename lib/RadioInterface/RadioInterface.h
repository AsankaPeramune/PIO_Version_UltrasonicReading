#ifndef RADIO_INTERFACE_H
#define RADIO_INTERFACE_H

#include <Arduino.h>

class RadioInterface
{
private:
    HardwareSerial *_serial;
    uint8_t _rxPin;
    uint8_t _txPin;
    uint32_t _baud;

public:
    RadioInterface(HardwareSerial &serial, uint8_t rxPin, uint8_t txPin, uint32_t baud);

    void init();
    void sendDistance(uint16_t distance_mm);
};

#endif