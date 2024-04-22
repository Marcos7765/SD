//#include <avr/io.h>
#include<util/delay.h>
#include "DHT.h"
//valores padrão, defina lá pra cima
#ifndef DHTPIN
#define DHTPIN 22
#endif
#ifndef DHTTYPE
#define DHTTYPE DHT22
#endif
DHT dht(DHTPIN, DHTTYPE);

inline void ST_setup(){
  dht.begin();
  _delay_ms(1000);
}

//o ponto td de ter esse sensor
//use isnan para checar se o valor está certo (servirá pra botar no display)
inline float ST_getTemp(){
  return dht.readTemperature();
}

#define TESTE
#ifdef TESTE
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  ST_setup();
}

void loop() {
  float t = ST_getTemp();

  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C ");
  delay(2000);
}
#endif