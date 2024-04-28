#include <avr/eeprom.h>

#define EEPROM_ADDRESS 0

void writeFloat(int address, float data) {
  eeprom_write_block((const void*)&data, (void*)address, sizeof(float));
}

float readFloat(int address) {
  float data;
  eeprom_read_block((void*)&data, (const void*)address, sizeof(float));
  return data;
}

void setup() {
  Serial.begin(9600);
  
  float dataToWrite[10] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
  
  for (int i = 0; i < 10; i++) {
    writeFloat(EEPROM_ADDRESS + i * sizeof(float), dataToWrite[i]);
    Serial.print("Dado ");
    Serial.print(i);
    Serial.print(" escrito na EEPROM: ");
    Serial.println(dataToWrite[i]);
    delay(100);
  }
  
  for (int i = 0; i < 10; i++) {
    float readData = readFloat(EEPROM_ADDRESS + i * sizeof(float));
    Serial.print("Dado ");
    Serial.print(i);
    Serial.print(" lido da EEPROM: ");
    Serial.println(readData);
  }
}

void loop() { }