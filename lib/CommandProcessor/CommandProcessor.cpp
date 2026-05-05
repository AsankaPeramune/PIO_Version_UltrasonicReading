#include "CommandProcessor.h"

#define NVS_NAMESPACE "wl_node"
#define KEY_REF_MM "ref_mm"

CommandProcessor::CommandProcessor(UltrasonicSensor &sensor, RadioInterface &radio, const char *id)
{
    _sensor = &sensor;
    _radio = &radio;
    _sensorID = id;

    _idx = 0;
    _referenceDistance = 0;
    _referenceSet = false;
    _waitingForRef = false;
}

void CommandProcessor::init()
{
    prefs.begin(NVS_NAMESPACE, false);

    _referenceDistance = prefs.getInt(KEY_REF_MM, 0);

    if (_referenceDistance >= 280 && _referenceDistance <= 7500)
    {
        _referenceSet = true;
        Serial.println("REF loaded");
    }
}

bool CommandProcessor::processSetRefValue(const char *cmd)
{
    int value;
    if (sscanf(cmd, "%d", &value) != 1) return false;

    if (value < 250 || value > 7500) return false;

    _referenceDistance = value;
    _referenceSet = true;
    _waitingForRef = false;

    prefs.putInt(KEY_REF_MM, value);

    _radio->sendLine("REF SET OK");

    return true;
}

void CommandProcessor::process()
{
    while (_radio->available())
    {
        char c = _radio->read();

        if (c == '\n' || c == '\r')
        {
            if (_idx == 0) continue;

            _buf[_idx] = '\0';
            _idx = 0;

            char *firstColon = strchr(_buf, ':');
            if (!firstColon) continue;

            char *afterRadio = firstColon + 1;
            while (*afterRadio == ' ') afterRadio++;

            char *secondColon = strchr(afterRadio, ':');
            if (!secondColon) continue;

            *secondColon = '\0';

            char *key = afterRadio;
            char *cmd = secondColon + 1;
            while (*cmd == ' ') cmd++;

            if (strcmp(key, _sensorID) != 0) continue;

            if (_waitingForRef)
            {
                if (!processSetRefValue(cmd))
                    _radio->sendLine("INVALID REF");
                continue;
            }

            if (strncmp(cmd, "DIST?", 5) == 0)
            {
                int d = _sensor->sensorRead();
                if (d > 0)
                    _radio->sendDistance(_sensorID, d);
            }
            else if (strncmp(cmd, "LEVEL?", 6) == 0)
            {
                if (!_referenceSet)
                {
                    _radio->sendLine("ERROR: REF NOT SET");
                }
                else
                {
                    int d = _sensor->sensorRead();
                    if (d > 0)
                        _radio->sendLevel(_sensorID, _referenceDistance - d);
                }
            }
            else if (strncmp(cmd, "SETREF", 6) == 0)
            {
                _waitingForRef = true;
                _radio->sendLine("Enter REF value");
            }
            else if (strncmp(cmd, "REF?", 4) == 0)
            {
                if (_referenceSet)
                    _radio->sendRef(_sensorID, _referenceDistance);
            }
            else
            {
                _radio->sendLine("UNKNOWN CMD");
            }
        }
        else
        {
            if (_idx < sizeof(_buf) - 1)
                _buf[_idx++] = c;
            else
                _idx = 0;
        }
    }
}