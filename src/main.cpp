#include <Arduino.h>
#include <UltrasonicSensor.h>
#include <RadioInterface.h>
#include <CommandProcessor.h>

#define SENSOR_RX_PIN   4
#define SENSOR_TX_PIN   5
#define RADIO_RX_PIN    6
#define RADIO_TX_PIN    7

#define SEND_INTERVAL_MS (10UL * 1000UL)

HardwareSerial SensorSerial(0);
HardwareSerial RadioSerial(1);

UltrasonicSensor sensor(SensorSerial, SENSOR_RX_PIN, SENSOR_TX_PIN);
RadioInterface radio(RadioSerial, RADIO_RX_PIN, RADIO_TX_PIN, 38400);
CommandProcessor cmd(sensor, radio, "WL1");

unsigned long lastSend = 0;

void setup()
{
    Serial.begin(115200);

    sensor.init();
    radio.init();
    cmd.init();

    Serial.println("System Started");
}

void loop()
{
    cmd.process();

    if (millis() - lastSend > SEND_INTERVAL_MS)
    {
        lastSend = millis();

        int d = sensor.sensorRead();
        if (d > 0)
        {
            Serial.printf("Distance: %d mm\n", d);
            radio.sendDistance("WL1", d);
        }
    }
}