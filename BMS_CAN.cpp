#include "BMS_CAN.h"

BMS_CAN::BMS_CAN(uint8_t csPin, long baudrate)
    : mcp(csPin), baudrate(baudrate) {
  soc = 0.0;
  batteryVoltage = 0.0;
  batteryCurrent = 0.0;
  highestCellVoltage = 0.0;
  lowestCellVoltage = 0.0;
  batteryCharging = false;
  chargeWireConnected = false;
  batteryLowSOC = false;
  batteryReady = false;
  dischargeMOSStatus = false;
  chargeMOSStatus = false;
  highestTemperature = 0.0;
  lowestTemperature = 0.0;
  batteryCycle = 0;
  energyCycle = 0.0;
  batteryFailureLevel = 0;
#ifdef FREERTOS_H
  dataMutex = xSemaphoreCreateMutex();
#endif
}

bool BMS_CAN::begin() { return mcp.begin(baudrate); }

void BMS_CAN::update() {
  int packetSize = mcp.parsePacket();
  if (packetSize) {
    uint8_t data[8];
    uint32_t id = mcp.packetId();
    for (int i = 0; i < packetSize; i++) {
      data[i] = mcp.read();
    }
    parsePacket(id, data, packetSize);
  }
}

void BMS_CAN::parsePacket(uint32_t id, uint8_t *data, uint8_t len) {
  if (id == 0x18FF28F4) { // BMS Basic Information 1
    bool newChargeWireConnected = data[0] & 0x01;
    bool newBatteryCharging = data[0] & 0x02;
    bool newBatteryLowSOC = data[0] & 0x04;
    bool newBatteryReady = data[0] & 0x08;
    bool newDischargeMOSStatus = data[0] & 0x10;
    bool newChargeMOSStatus = data[0] & 0x20;
    float newSOC = data[1];
    float newBatteryVoltage = (data[4] | (data[5] << 8)) * 0.1;
    float newBatteryCurrent = ((data[2] | (data[3] << 8)) - 5000) * 0.1;
    uint8_t newBatteryFailureLevel = data[6];

#ifdef FREERTOS_H
    xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
    chargeWireConnected = newChargeWireConnected;
    batteryCharging = newBatteryCharging;
    batteryLowSOC = newBatteryLowSOC;
    batteryReady = newBatteryReady;
    dischargeMOSStatus = newDischargeMOSStatus;
    chargeMOSStatus = newChargeMOSStatus;
    soc = newSOC;
    batteryVoltage = newBatteryVoltage;
    batteryCurrent = newBatteryCurrent;
    batteryFailureLevel = newBatteryFailureLevel;
#ifdef FREERTOS_H
    xSemaphoreGive(dataMutex);
#endif
  } else if (id == 0x18FE28F4) { // BMS Basic Information 2
    float newHighestCellVoltage = (data[0] | (data[1] << 8)) * 0.001;
    float newLowestCellVoltage = (data[2] | (data[3] << 8)) * 0.001;
    float newHighestTemperature = data[4] - 40;
    float newLowestTemperature = data[5] - 40;

#ifdef FREERTOS_H
    xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
    highestCellVoltage = newHighestCellVoltage;
    lowestCellVoltage = newLowestCellVoltage;
    highestTemperature = newHighestTemperature;
    lowestTemperature = newLowestTemperature;
#ifdef FREERTOS_H
    xSemaphoreGive(dataMutex);
#endif
  } else if (id == 0x18A328F4) { // Battery Cycle Information
    uint32_t newBatteryCycle =
        (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    uint32_t newEnergyCycle =
        (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
    float newEnergyCycleWh = newEnergyCycle * 0.1;

#ifdef FREERTOS_H
    xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
    batteryCycle = newBatteryCycle;
    energyCycle = newEnergyCycleWh;
#ifdef FREERTOS_H
    xSemaphoreGive(dataMutex);
#endif
  }
}

float BMS_CAN::getSOC() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = soc;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getBatteryVoltage() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = batteryVoltage;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getBatteryCurrent() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = batteryCurrent;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getPower() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = (batteryVoltage * batteryCurrent) / 1000.0;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getHighestCellVoltage() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = highestCellVoltage;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getLowestCellVoltage() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = lowestCellVoltage;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isBatteryCharging() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = batteryCharging;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isChargeWireConnected() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = chargeWireConnected;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isBatteryLowSOC() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = batteryLowSOC;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isBatteryReady() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = batteryReady;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isDischargeMOSStatus() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = dischargeMOSStatus;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

bool BMS_CAN::isChargeMOSStatus() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  bool value = chargeMOSStatus;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getHighestTemperature() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = highestTemperature;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getLowestTemperature() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = lowestTemperature;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

uint32_t BMS_CAN::getBatteryCycle() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  uint32_t value = batteryCycle;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

float BMS_CAN::getEnergyCycle() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  float value = energyCycle;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}

uint8_t BMS_CAN::getBatteryFailureLevel() {
#ifdef FREERTOS_H
  xSemaphoreTake(dataMutex, portMAX_DELAY);
#endif
  uint8_t value = batteryFailureLevel;
#ifdef FREERTOS_H
  xSemaphoreGive(dataMutex);
#endif
  return value;
}
