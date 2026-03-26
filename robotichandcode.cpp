#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

Servo servos[5];

// Servo signal pins
const int servoPins[5] = {13, 12, 14, 27, 26};

// Incoming FSR values
int fsrValues[5] = {0, 0, 0, 0, 0};

// Servo angle limits
const int minAngle[5] = {0, 0, 0, 0, 0};
const int maxAngle[5] = {180, 180, 180, 180, 180};

String incomingData = "";

void parseData(String data) {
  int index = 0;
  int lastPos = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data.charAt(i) == ',') {
      if (index < 5) {
        fsrValues[index] = data.substring(lastPos, i).toInt();
        index++;
        lastPos = i + 1;
      }
    }
  }

  // Last value
  if (index < 5) {
    fsrValues[index] = data.substring(lastPos).toInt();
  }
}

int mapFSRToAngle(int fsrValue, int servoIndex) {
  // Adjust these based on your actual FSR readings
  int minFSR = 500;
  int maxFSR = 3000;

  fsrValue = constrain(fsrValue, minFSR, maxFSR);

  return map(fsrValue, minFSR, maxFSR, minAngle[servoIndex], maxAngle[servoIndex]);
}

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32_HAND"); // Receiver Bluetooth name
  Serial.println("Hand receiver started");

  for (int i = 0; i < 5; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPins[i], 500, 2400);
    servos[i].write(0);
  }
}

void loop() {
  while (SerialBT.available()) {
    char c = SerialBT.read();

    if (c == '\n') {
      parseData(incomingData);
      incomingData = "";

      for (int i = 0; i < 5; i++) {
        int angle = mapFSRToAngle(fsrValues[i], i);
        servos[i].write(angle);

        Serial.print("FSR");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(fsrValues[i]);
        Serial.print(" -> Servo: ");
        Serial.println(angle);
      }

      Serial.println("----");
    } else if (c != '\r') {
      incomingData += c;
    }
  }
}
