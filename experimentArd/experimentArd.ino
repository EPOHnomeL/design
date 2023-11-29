#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX
ModbusMaster node;
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);

  node.begin(1, Serial);
  delay(100);
  node.writeSingleRegister(0, 3);
  node.writeSingleRegister(1, 2);
  delay(100);

  node.setTransmitBuffer(0, 1);   // Set the state to Initialisation 
  node.setTransmitBuffer(1, 50); // Water Percentage
  node.setTransmitBuffer(2, 550); // Motor RPM
  node.setTransmitBuffer(3, 200); // Bucket RPM
  node.setTransmitBuffer(4, 90);  // Arm Angle
  node.setTransmitBuffer(5, 600);  // Time left
  node.setTransmitBuffer(6, 1); // Motor direction
  node.setTransmitBuffer(7, 0); // Bucket direction
  node.writeMultipleRegisters(0x0010, 8);
  delay(1000);
  node.clearTransmitBuffer();
  node.setTransmitBuffer(0, 2);   // Set the state to Initialisation 
  node.setTransmitBuffer(1, 30); // Water Percentage
  node.setTransmitBuffer(2, 330); // Motor RPM
  node.setTransmitBuffer(3, 100); // Bucket RPM
  node.setTransmitBuffer(4, 88);  // Arm Angle
  node.setTransmitBuffer(5, 1000);  // Time left
  node.setTransmitBuffer(6, 1); // Motor direction
  node.setTransmitBuffer(7, 1); // Bucket direction
  node.writeMultipleRegisters(0x0020, 8);
  delay(1200);
  node.clearTransmitBuffer();
  node.setTransmitBuffer(0, 3);   // Set the state to Initialisation 
  node.setTransmitBuffer(1, 60); // Water Percentage
  node.setTransmitBuffer(2, 720); // Motor RPM
  node.setTransmitBuffer(3, 330); // Bucket RPM
  node.setTransmitBuffer(4, 92);  // Arm Angle
  node.setTransmitBuffer(5, 550);  // Time left
  node.setTransmitBuffer(6, 0); // Motor direction
  node.setTransmitBuffer(7, 0); // Bucket direction
  node.writeMultipleRegisters(0x0030, 8);
  delay(1000);
  // node.writeSingleRegister(0, 4);
  
}

void loop() {

}
