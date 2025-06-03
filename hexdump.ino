#include <Wire.h>

const int EEPROM_I2C_ADDRESS = 0x50; // AT24C256 default address
int memoryAddress = 0; // Start at address 0

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(1500);
}

void loop() {
  if (memoryAddress + 10 <= 1000) {
    // Print range: start to end address
    Serial.print("0x");
    printPaddedHex(memoryAddress);
    Serial.print(":0x");
    printPaddedHex(memoryAddress + 9);
    Serial.print(": ");

    // Print the 10 bytes
    for (int i = 0; i < 10; i++) {
      byte data = readEEPROMByte(EEPROM_I2C_ADDRESS, memoryAddress + i);

      Serial.print("0x");
      if (data < 0x10) Serial.print("0");
      Serial.print(data, HEX);

      if (i < 9) Serial.print(", "); // Don't print comma after last byte
    }

    Serial.println();
    memoryAddress += 10; // Move to next 10-byte block
  }
}

byte readEEPROMByte(int deviceAddress, unsigned int memAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write((memAddress >> 8) & 0xFF); // High byte
  Wire.write(memAddress & 0xFF);        // Low byte
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1);
  if (Wire.available()) {
    return Wire.read();
  } else {
    return 0xFF;
  }
}

void printPaddedHex(unsigned int value) {
  if (value < 0x1000) Serial.print("0");
  if (value < 0x100) Serial.print("0");
  if (value < 0x10) Serial.print("0");
  Serial.print(value, HEX);
}
