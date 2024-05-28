

# BMS CAN Library

The `BMS CAN` library simplifies reading BMS data from a CAN bus using the Adafruit MCP2515 CAN controller. It provides an easy-to-use interface for accessing various BMS parameters such as SOC, battery voltage, current, cell voltages, temperature, and more.

## Features

- Read SOC, battery voltage, and current
- Calculate power output in kilowatts
- Read highest and lowest cell voltages
- Check battery charging status
- Read highest and lowest temperature
- Read battery cycle and energy cycle count
- Read battery failure level
- And more...

## Installation

Coming soon...

## Usage

Include the `BMS_CAN` library in your Arduino sketch. Call the `update()` method in your loop to keep the data current, and use the provided getter methods to access the BMS data.

### Example

```cpp
#include <BMS_CAN.h>

BMS_CAN bms_can(5, 250000);

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


  delay(1000);
}
```

## License

This library is licensed under the MIT License. See the `LICENSE` file for more details.
