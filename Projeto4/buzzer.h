#ifndef BZ
#define BZ
#include <avr/io.h>

namespace BZ{
uint8_t BZincr = 0;
void BZ_setup();
void sirene();
void BZ_clear();
void BZ_set();
}
#endif