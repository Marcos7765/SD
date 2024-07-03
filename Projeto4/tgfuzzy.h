#ifndef TGFUZZ
#define TGFUZZ
#include<math.h>
namespace TGF {

float updatePmts(float tempVals[3], float umiVals[3]);
float pertencimento(float x, float xZero, uint8_t ehTemp, uint8_t modo, uint8_t ehEsq);
float pTotal(float xs[3][2], float xZeros[3][2]);
uint8_t inferencia(float pTotal);
}

#endif