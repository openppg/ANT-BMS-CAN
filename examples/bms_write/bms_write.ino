#include <BMS_CAN.h>

BMS_CAN bms_can(5, 250000); // CS pin and baud rate

const int buttonPin = 2; // Pin connected to the button
bool buttonState = false; // Current state of the button
bool lastButtonState = false; // Previous state of the button
bool dischargeMOSState = false; // Local state of the Discharge MOS

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP); // Configure the button pin

  if (!bms_can.begin()) {
    Serial.println("Error initializing BMS_CAN");
    while (1);
  }
}

void loop() {
  bms_can.update();

  // Read SOC
  float soc = bms_can.getSOC();
  Serial.print("SOC: ");
  Serial.print(soc);
  Serial.println(" %");

  // Read current state of Discharge MOS from BMS
  bool currentDischargeMOSState = bms_can.isDischargeMOSStatus();

  // Read button state
  buttonState = digitalRead(buttonPin);

  // Check for button press (button state change)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Toggle the Discharge MOS state
    currentDischargeMOSState = !currentDischargeMOSState;
    bms_can.enableDischargeMOS(currentDischargeMOSState);
    Serial.print("Discharge MOS ");
    Serial.println(currentDischargeMOSState ? "enabled" : "disabled");
  }

  // Save the current button state for the next loop
  lastButtonState = buttonState;

  delay(100); // Adjust as needed
}
