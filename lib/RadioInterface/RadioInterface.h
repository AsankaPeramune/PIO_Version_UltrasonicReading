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

    void sendLine(const char *msg);
    void sendDistance(const char *id, uint16_t distance);
    void sendLevel(const char *id, int level);
    void sendRef(const char *id, int ref);

    bool available();
    char read();
};

#endif