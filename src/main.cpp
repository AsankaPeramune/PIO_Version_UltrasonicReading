#include <Arduino.h>
#include <UltrasonicSensor.h>
#include <RadioInterface.h>

#define SENSOR_RX_PIN   4
#define SENSOR_TX_PIN   5
#define RADIO_RX_PIN    6
#define RADIO_TX_PIN    7

#define SEND_INTERVAL_MS   (10UL * 1000UL)

HardwareSerial SensorSerial(0);
HardwareSerial RadioSerial(1);

// Create objects
UltrasonicSensor sensor(SensorSerial, SENSOR_RX_PIN, SENSOR_TX_PIN);
RadioInterface radio(RadioSerial, RADIO_RX_PIN, RADIO_TX_PIN, 38400);

unsigned long lastSendTime = 0;

void setup()
{
    Serial.begin(115200);

    sensor.init();
    radio.init();

    Serial.println("System started");
}

void loop()
{
    unsigned long now = millis();

    if (now - lastSendTime >= SEND_INTERVAL_MS)
    {
        lastSendTime = now;

        int distance = sensor.sensorRead();

        if (distance > 0)
        {
            Serial.printf("Distance: %d mm\n", distance);

            radio.sendDistance(distance);
        }
        else
        {
            Serial.println("Sensor read failed");
        }
    }
}