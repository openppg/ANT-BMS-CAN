#include <BMS_CAN.h>
#ifdef ESP8266
   #define CS_PIN    2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S3)
   #define CS_PIN    14
#elif defined(TEENSYDUINO)
   #define CS_PIN    8
#elif defined(ARDUINO_STM32_FEATHER)
   #define CS_PIN    PC5
#elif defined(ARDUINO_NRF52832_FEATHER)  /* BSP 0.6.5 and higher! */
   #define CS_PIN    27
#elif defined(ARDUINO_MAX32620FTHR) || defined(ARDUINO_MAX32630FTHR)
   #define CS_PIN    P3_2
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
   #define CS_PIN    7
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_CAN)
   #define CS_PIN    PIN_CAN_CS
#elif defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W) // PiCowbell CAN Bus
   #define CS_PIN    20
#else
    // Anything else, defaults!
   #define CS_PIN    5
#endif

BMS_CAN bms_can(CS_PIN, 250000);  // Change CS pin and baud rate as needed

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

  Serial.print("Battery batteryLowSOC: ");
  Serial.println(bms_can.isBatteryLowSOC() ? "Yes" : "No");

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

  Serial.print("gBatteryFailureLevel: ");
  Serial.println(bms_can.getBatteryFailureLevel());

  delay(500);  // Adjust as needed
}
