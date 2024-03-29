#include<avr/io.h>
#include<util/delay.h>

uint8_t aceso = 0;
uint8_t leitura = 0;

void setup_c() {
  DDRA = 0b00000101;
  //o 22 vai estar servindo como o +5V só para
  //caso se tenha trocado as palavras em
  //"Utilize o sinal de saída do botão 
  //como entrada do microcontrolador"
  PORTA = 0b00000001;
}

void loop_c() {
  leitura = PINA & (0b00000010);
  if (leitura != aceso){
    aceso = leitura;
    PORTA ^= 0b00000100;
    _delay_ms(50);
  }
}

int main(){
  setup_c();
  while (1){
    loop_c();
  }
}