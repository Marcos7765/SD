#include<avr/io.h>
#include<util/delay.h>

/*
A ideia geral é que a configuração e
uso das funcionalidades dos registra-
dores é manipulada pelos valores DDRX,
PORTX e PINX (X sendo a letra do re-
gistrador) para configuração, escrita
e leitura, respectivamente. Estes va-
lores são uint8_t, onde cada casa bi-
nária do inteiro se refere a um pino
diferente.
*/

void setup_c() {
  /*
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  O código abaixo, em C/C++ puro,
  é equivalente ao código acima.
  */
  DDRA = 0b00000111;
}

/*
void pisca(uint8_t pin, double time_ms){
  digitalWrite(pin, HIGH);
  delay(time_ms);
  digitalWrite(pin, LOW);
}
A função abaixo é equivalente a esta
função comentada, usando inline pra
tentar evitar criar uma f, enquanto
que uso um xor pra desligar a saída
ao invés do and do not pra poupar uma
operação. o volatile é necessário por
conta das ports já serem volatile por
si.
*/
inline void pisca(volatile uint8_t& port, uint8_t pin_mask, double time_ms){
  port |= pin_mask;
  _delay_ms(time_ms);
  port ^= pin_mask;
}

//a transformação de loop para loop_c fica como exercício para o leitor
void exec() {
  pisca(PORTA, (1 << 0), 5000);
  pisca(PORTA, (1 << 1), 2000);
  pisca(PORTA, (1 << 2), 5000);
}

//em c/c++ puro tem que fazer a main
int main(){
  setup_c();
  while (1){
    exec();
  }
}