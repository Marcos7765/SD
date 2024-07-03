#ifndef LCD
#define LCD
#include<stdio.h>
#include"lcdcontrol.h"


namespace LCD {
typedef struct Time {
  uint8_t second;
  uint8_t minute;
} Time;

inline void lcd_setup(){init_lcd();}
void printConfig(uint8_t cfgMax, uint8_t modo, float temp, float RH);
void printExec(float, uint8_t estado, Time tempo);
}
#endif