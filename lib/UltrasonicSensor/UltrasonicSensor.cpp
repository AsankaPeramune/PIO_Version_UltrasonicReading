#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(HardwareSerial &serial, uint8_t rxPin, uint8_t txPin)
{
    _serial = &serial;
    _rxPin = rxPin;
    _txPin = txPin;
}

void UltrasonicSensor::init()
{
    _serial->begin(9600, SERIAL_8N1, _rxPin, _txPin);
}

int UltrasonicSensor::sensorRead()
{
    while (_serial->available() >= 4)
    {
        if (_serial->read() == 0xFF)
        {
            _frame[0] = 0xFF;
            _frame[1] = _serial->read();
            _frame[2] = _serial->read();
            _frame[3] = _serial->read();

            uint8_t checksum = (_frame[0] + _frame[1] + _frame[2]) & 0xFF;

            if (checksum == _frame[3])
            {
                int distance = (_frame[1] << 8) | _frame[2];
                return distance;
            }
            else
            {
                return -1; // checksum error
            }
        }
    }
    return -1; // no valid data yet
}