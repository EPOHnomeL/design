#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
ModbusMaster node;
float time = 500;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);

  node.begin(1, Serial);

  node.writeSingleRegister(0, 3); // Set state to initialization
  delay(1000);
  node.writeSingleRegister(1, 2); // Set current profile
  delay(1000);
  node.setTransmitBuffer(0, 4);   // Set the state to Mixing 
  node.setTransmitBuffer(2, analogRead(A0)); // Motor RPM
  node.setTransmitBuffer(3, 600); // Bucket RPM
  node.setTransmitBuffer(4, 90);  // Arm Angle
  node.setTransmitBuffer(5, (uint16_t)time);  // Time left
  node.setTransmitBuffer(6, 1); // Motor direction
  node.setTransmitBuffer(7, 0); // Bucket direction
  node.writeMultipleRegisters(0x0000, 8);
}

void loop() {
  time = time - 0.01;
  node.writeSingleRegister(2, analogRead(A0));
  node.writeSingleRegister(5, (uint16_t)time);
}