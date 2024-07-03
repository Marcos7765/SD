#ifndef PT
#define PT
#include <avr/io.h>

namespace PT {
    void PT_setup();
    float readPot(int16_t minV, int16_t maxV);
}

#endif