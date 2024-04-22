#include<avr/io.h>
#include<util/delay.h>
#include<OneWire.h>
#include<DallasTemperature.h>
#include<stdlib.h>

#ifndef ST_pin
#define ST_pin 22 // padrão pin A
#endif

#define ST_ERR_READ 0xFFFF

OneWire oneWire(ST_pin);
DallasTemperature sensors(&oneWire);

void ST_setup() {
  Serial.begin(9600);
  sensors.begin();
}

float getTemp(){
  sensors.requestTemperatures();
  float T = sensors.getTempCByIndex(0);
  if (T != DEVICE_DISCONNECTED_C) {
    return T;
}
  //esse valor será o valor para erro em leituras
  return ST_ERR_READ;
}

#define TESTE
#ifdef TESTE
void setup_c(){
  ST_setup();
}

int main (){
  setup_c();
  float T;
  char str[12];
  while(1){
    T = getTemp();
    dtostrf(T, 4, 1, str);
    //sprintf(str, "Temp. %sC", str);
    Serial.println(str);
    _delay_ms(1000);
  }
}

//void loop() {
//  float temperatureC = getTemp();
//  if (temperatureC != DEVICE_DISCONNECTED_C) {
//    Serial.print("Temperature: ");
//    Serial.print(temperatureC);
//    Serial.println(" °C");
//  } else {
//    Serial.println("Error: Could not read temperature data");
//  }
//
//  delay(1000);
//}
#endif