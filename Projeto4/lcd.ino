#include "lcd.h"

namespace LCD {

void printConfig(uint8_t cfgMax, uint8_t modo, float temp, float RH){
  char tempStr[6];
  char RHStr[6];
  const char *modes[] = {"INST","MED ","VAR "};
  dtostrf(temp, 5, 1, tempStr);
  dtostrf(RH, 5, 1, RHStr);
  lcd_set_cursor(0, 0);
  lcd_write("Temp");
  if(cfgMax){lcd_write("* "); lcd_set_cursor(1, 4);lcd_write(" ");lcd_set_cursor(0, 6);}
  else{lcd_write("  ");lcd_set_cursor(1, 4); lcd_write("*"); lcd_set_cursor(0, 6);}
  lcd_write(tempStr);
  lcd_write("C ");
  lcd_write("CFG");
  lcd_set_cursor(1, 0);
  lcd_write("UmiR");
  lcd_set_cursor(1, 5);
  lcd_write(RHStr);
  lcd_write("% ");
  lcd_write(modes[modo]);
}

void printExec(float pTotal, uint8_t estado, Time t){
  const char *modes[] = {"INST","MED ","VAR "};
  const char *stts[] = {"  OK","RUIM"};
  char ptStr[7];
  char timeStr[6];
  snprintf(timeStr, 6, "%02d:%02d", t.minute, t.second);
  dtostrf(pTotal, 6, 4, ptStr);
  lcd_set_cursor(0, 0);
  lcd_write("Tempo  P: ");
  lcd_write(ptStr);
  lcd_set_cursor(1, 0);
  lcd_write(timeStr);
  lcd_write(" Est:  ");
  lcd_write(stts[estado]);
}

}