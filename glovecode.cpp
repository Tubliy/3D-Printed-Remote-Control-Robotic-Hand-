#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// FSR analog pins
const int fsrPins[5] = {34, 35, 32, 33, 25};

// Smoothed values
int fsrValues[5];

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32_GLOVE"); // Bluetooth device name
  Serial.println("Glove transmitter started");
}

void loop() {
  // Read all FSRs
  for (int i = 0; i < 5; i++) {
    int raw = analogRead(fsrPins[i]);
    fsrValues[i] = raw;
  }

  // Send as CSV string: val1,val2,val3,val4,val5
  String data = "";
  for (int i = 0; i < 5; i++) {
    data += String(fsrValues[i]);
    if (i < 4) data += ",";
  }

  SerialBT.println(data);
  Serial.println(data);

  delay(50);
}
