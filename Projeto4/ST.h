#ifndef ST
#define ST

#include <avr/io.h>
#include<util/delay.h>

namespace ST {
float RH=0;
float Temp=0;

typedef struct IO {
  volatile uint8_t& DR;
  volatile uint8_t& PORT;
  volatile uint8_t& PIN;
  uint8_t PIN_MASK;
} IO;

void st_setup(IO cfg);
uint8_t lerDados(IO cfg);

}
#endif