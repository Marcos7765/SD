#ifndef LCD_CTRL
#define LCD_CTRL

#define set_bit(Y,bit_x) (Y|=(1<<bit_x))
#define clr_bit(Y,bit_x) (Y&=~(1<<bit_x))
#define tst_bit(Y,bit_x) (Y&(1<<bit_x))
#define cpl_bit(Y,bit_x) (Y^=(1<<bit_x))
#define pulso_enable() _delay_us(1); set_bit(CONTR_LCD,E); _delay_us(1); clr_bit(CONTR_LCD,E); _delay_us(45)
#define DADOS_LCD PORTA
#define CONTR_LCD PORTB
#define RS PB1
#define E PB0

void cmd_lcd(unsigned char c, char cd);
void init_lcd();
void lcd_set_cursor(unsigned char row, unsigned char column);
void lcd_write(char *c);

#endif