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

void RadioInterface::sendLine(const char *msg)
{
    _serial->print(msg);
    _serial->print("\n");
    _serial->flush();
}

void RadioInterface::sendDistance(const char *id, uint16_t d)
{
    _serial->print(id);
    _serial->print(" DIST=");
    _serial->print(d);
    _serial->print("\n");
}

void RadioInterface::sendLevel(const char *id, int lvl)
{
    _serial->print(id);
    _serial->print(" LEVEL=");
    _serial->print(lvl);
    _serial->print("\n");
}

void RadioInterface::sendRef(const char *id, int ref)
{
    _serial->print(id);
    _serial->print(" REF=");
    _serial->print(ref);
    _serial->print("\n");
}

bool RadioInterface::available()
{
    return _serial->available();
}

char RadioInterface::read()
{
    return (char)_serial->read();
}