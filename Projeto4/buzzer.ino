#include "buzzer.h"

#ifndef BUZZBASE
#define BUZZBASE 10
#endif
#ifndef BUZZINCR
#define BUZZINCR 64
#endif
#define BUZZBASECOMP 256-BUZZBASE

namespace BZ {
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
  BZincr = (BZincr + BUZZINCR)%BUZZBASECOMP;
  OCR0A = BUZZBASE + BZincr;
}

void BZ_clear(){
  BZincr = 0;
  OCR0A = 0;
  DDRB &= ~0b10000000;
}

void BZ_set(){
  DDRB |= 0b10000000;
}
}