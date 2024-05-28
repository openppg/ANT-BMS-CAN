#ifndef BMS_CAN_H
#define BMS_CAN_H

#include <Arduino.h>
#include <Adafruit_MCP2515.h>

class BMS_CAN {
public:
    BMS_CAN(uint8_t csPin, long baudrate);
    bool begin();
    void update();

    float getSOC();
    float getBatteryVoltage();
    float getBatteryCurrent();
    float getPower();
    float getHighestCellVoltage();
    float getLowestCellVoltage();
    bool isBatteryCharging();
    float getHighestTemperature();
    float getLowestTemperature();
    uint32_t getBatteryCycle();
    float getEnergyCycle();

private:
    Adafruit_MCP2515 mcp;
    long baudrate;

    float soc;
    float batteryVoltage;
    float batteryCurrent;
    float highestCellVoltage;
    float lowestCellVoltage;
    bool batteryCharging;
    float highestTemperature;
    float lowestTemperature;
    uint32_t batteryCycle;
    float energyCycle;

    void parsePacket(uint32_t id, uint8_t *data, uint8_t len);

    // Mutex for thread safety
#ifdef FREERTOS_H
    SemaphoreHandle_t dataMutex;
#endif
};

#endif
