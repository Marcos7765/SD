#include "DHT.h"
#include <avr/io.h>
#include<util/delay.h>
//valores padrão, defina lá pra cima
#ifndef DHTPIN
#define DHTPIN 22
#endif
#ifndef DHTTYPE
#define DHTTYPE DHT22
#endif
DHT dht(DHTPIN, DHTTYPE);

void ST_setup(){
  dht.begin();
  _delay_ms(1000); //datasheet da AOSONG sugere esperar 1s depois de iniciar,
                   //e no .cpp da classe ele não tem essa espera.
}

//o ponto td de ter esse sensor
//use isnan para checar se o valor está certo (servirá pra botar no display)
float getTemp(){
  return dht.readTemperature();
}

#define TESTE
#ifdef TESTE
void setup_c() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  ST_setup();
}

void loop_c() {
  float t = getTemp();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  delay(5000);
}

int main(){
  setup_c;
  while(1){
    loop_c();
  }
}
#endif