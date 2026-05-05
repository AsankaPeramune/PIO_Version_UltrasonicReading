#include "RadioInterface.h"

RadioInterface::RadioInterface(HardwareSerial &serial, uint8_t rxPin, uint8_t txPin, uint32_t baud)
{
    _serial = &serial;
    _rxPin = rxPin;
    _txPin = txPin;
    _baud = baud;
}

void RadioInterface::init()
{
    _serial->begin(_baud, SERIAL_8N1, _rxPin, _txPin);
}

void RadioInterface::sendDistance(uint16_t distance_mm)
{
    _serial->print("DIST=");
    _serial->print(distance_mm);
    _serial->print(" mm\n");
}