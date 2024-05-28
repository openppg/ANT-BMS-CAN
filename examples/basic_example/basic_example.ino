#include <BMS_CAN.h>

BMS_CAN bms_can(5, 250000);  // Change CS pin and baud rate as needed

void setup() {
  Serial.begin(115200);
  if (!bms_can.begin()) {
    Serial.println("Error initializing BMS_CAN");
    while (1);
  }
}

void loop() {
  bms_can.update();

  Serial.print("SOC: ");
  Serial.print(bms_can.getSOC());
  Serial.println(" %");

  Serial.print("Battery Voltage: ");
  Serial.print(bms_can.getBatteryVoltage());
  Serial.println(" V");

  Serial.print("Battery Current: ");
  Serial.print(bms_can.getBatteryCurrent());
  Serial.println(" A");

  Serial.print("Power: ");
  Serial.print(bms_can.getPower());
  Serial.println(" kW");

  Serial.print("Highest Cell Voltage: ");
  Serial.print(bms_can.getHighestCellVoltage());
  Serial.println(" V");

  Serial.print("Lowest Cell Voltage: ");
  Serial.print(bms_can.getLowestCellVoltage());
  Serial.println(" V");

  Serial.print("Battery Charging: ");
  Serial.println(bms_can.isBatteryCharging() ? "Yes" : "No");

  Serial.print("Highest Temperature: ");
  Serial.print(bms_can.getHighestTemperature());
  Serial.println(" °C");

  Serial.print("Lowest Temperature: ");
  Serial.print(bms_can.getLowestTemperature());
  Serial.println(" °C");

  Serial.print("Battery Cycle: ");
  Serial.println(bms_can.getBatteryCycle());

  Serial.print("Energy Cycle: ");
  Serial.print(bms_can.getEnergyCycle());
  Serial.println(" Wh");

  delay(500);  // Adjust as needed
}
