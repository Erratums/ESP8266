
#include <SoftwareSerial.h>

const byte cMsgRelayOn[] = {0xA2, 0xA3, 0xA4, 0xA5};
const byte cMsgRelayOff[] = {0xA1, 0xA2, 0xA3, 0xA4};

byte serialData[] = {0x01, 0x01, 0x01, 0x01};
byte dataIndex;

SoftwareSerial SerialOTG(2, 3);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  SerialOTG.begin(115200);

  Serial.println(F("Initializing . . . !"));

  clearData();
  for (int iCntr = 4; iCntr < 8; iCntr++) {
    pinMode(iCntr, OUTPUT);
    digitalWrite(iCntr, LOW);
  }
  delay(1000);
}

void clearData() {
  dataIndex = 0;
  for (int iCntr = 0; iCntr < 4; iCntr++) {
    serialData[iCntr] = 0x00;
  }
}

void loop() {  
  if (!SerialOTG.available()) {
    return;
  }

  clearData();
  while (SerialOTG.available()) {
    byte varData = SerialOTG.read();
    if ((dataIndex == 0) && (varData != 0xA0)) {
      // do nothing, wait for start bit [0xA0] { Ajmal }
    } else if (dataIndex < 4) {      
      serialData[dataIndex++] = varData;
      delay(15);
    }
  }

  if (dataIndex != 4) {
    return;
  }

  if (serialData[0] == 0xA0) {
    byte pinNo = serialData[1] + 3;    
    if (serialData[2] == 0x00) {
      // if (serialData[3] == cMsgRelayOff[pinNo]) {
        digitalWrite(pinNo, LOW);
        Serial.print(F("Turned Off: "));
        Serial.println(pinNo);
      // }
    } else if (serialData[2] == 0x01) {
      // if (serialData[3] == cMsgRelayOn[pinNo]) {
        digitalWrite(pinNo, HIGH);
        Serial.print(F("Turned On: "));
        Serial.println(pinNo);
      // }
    }
  }
}
