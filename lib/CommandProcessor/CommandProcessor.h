#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>
#include <Preferences.h>
#include "UltrasonicSensor.h"
#include "RadioInterface.h"

class CommandProcessor
{
private:
    UltrasonicSensor *_sensor;
    RadioInterface *_radio;

    Preferences prefs;

    const char *_sensorID;

    char _buf[64];
    uint8_t _idx;

    int _referenceDistance;
    bool _referenceSet;
    bool _waitingForRef;

public:
    CommandProcessor(UltrasonicSensor &sensor, RadioInterface &radio, const char *id);

    void init();
    void process();

private:
    bool processSetRefValue(const char *cmd);
};

#endif