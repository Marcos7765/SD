#include <avr/io.h>
#include <util/delay.h>
//#include <avr/pgmspace.h>
#include "lcdcontrol.h"

void cmd_lcd(unsigned char c, char cd)
{
  DADOS_LCD = c;

  if(cd == 0) {
    clr_bit(CONTR_LCD,RS);
  } 
  else {
    set_bit(CONTR_LCD,RS);
  }

  pulso_enable();

  if((cd==0) && (c<4)) {
    _delay_ms(2);
  }
}

void init_lcd()
{
  DDRB = 0xFF;
  DDRA = 0xFF;
  clr_bit(CONTR_LCD,RS);
  _delay_ms(15); 
  DADOS_LCD = 0x38;
  pulso_enable();
  _delay_ms(5);
  pulso_enable();
  _delay_us(200);
  pulso_enable();
  pulso_enable();
  cmd_lcd(0x08,0);
  cmd_lcd(0x01,0);
  cmd_lcd(0x0C,0);
  cmd_lcd(0x80,0);
}

void lcd_write(char *c)
{
  for (; *c != 0; c++) cmd_lcd(*c, 1);
}

void lcd_set_cursor(unsigned char linha, unsigned char coluna) {

  unsigned char posicao = 0x80;

  if (linha == 0) {
      posicao = 0x80;
  } else if (linha == 1) {
      posicao = 0xC0;
  }

  posicao += coluna;

  cmd_lcd(posicao, 0);
}

void lcd_clear() {
  for (unsigned char i = 0; i < 16; i++) {
    for (unsigned char j = 0; j < 2; j++) {
      lcd_set_cursor(j, i);
      lcd_write(' ');
    }
  }
  lcd_set_cursor(0, 0);
}