#include "potenciometro.h"

namespace PT {

float readPot(int16_t minV, int16_t maxV){
  ADCSRA ^= (0b01000000);
  while(ADCSRA & 0b01000000){};
  return minV+ (maxV-minV)*(ADC/1023.0);  
}

void PT_setup() {
  ADCSRA |= 0b10000011;
  ADMUX |=  0b01000000; //Tentei fazer um divisor de tensão (que acho que não estaria
  //funcionando nem na vida real), chequei a wiki do wokwi, vi um exemplo funcionando c o analogRead em
  //arduino e catei o ADMUX da implementação deles pra cá. Acaba que o AVCC é 5V.
  //Se houvesse um amp.op de buffer no wokwi talvez eu insistisse em usar os 2.56V.
}

}