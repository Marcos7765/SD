#include <avr/io.h>

#ifndef BUZZBASE
#define BUZZBASE 10
#endif
#ifndef BUZZINCR
#define BUZZINCR 64
#endif
#define BUZZBASECOMP 256-BUZZBASE

//usando o timer TC0 (13/PB7)
void BZ_setup() {
  //fast pwm, nn invertido
  DDRB |= 0b10000000;
  OCR0A = 0;
  TCCR0A |= 0b11000011;
  //sem prescale
  TCCR0B |= 0b00000001;
}

//variar
void sirene(){
  static uint8_t incr = 0;
  incr = (incr + BUZZINCR)%BUZZBASECOMP;
  OCR0A = BUZZBASE + incr;
}


void BZ_clear(){
  DDRB &= ~0b10000000;
}

void BZ_set(){
  DDRB |= 0b10000000;
}

#define TESTE
#ifdef TESTE
void loop_c() {
  // put your main code here, to run repeatedly:
  BZ_set();
  sirene();
  delay(2000);
  BZ_clear();
  delay(2000);
}
//no wokwi por alguma razão funciona melhor em arduino
//int main(){
//  BZ_setup();
//  while(1){loop_c();}
//}
void setup(){
  BZ_setup();
}

void loop(){
  loop_c();
}
#endif