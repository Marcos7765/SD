#include<avr/io.h>
#include<util/delay.h>
//código adaptado de um feito pela minha dupla em arduino achando que teria que fazer essa tarefa em sala
// Definição dos pinos do display de 7 segmentos
uint8_t buttonState = 0;
// Padrões de segmento para os dígitos de 0 a 7
//(reescrevi porque esculhambei onde cada pino ia no display, escrevi os padrões na forma que vinham primeiro)
const uint8_t numbers[8] = {
  0b01000000,  // 0
  0b01110110,  // 1
  0b00100001,  // 2
  (~0b01011011),  // 3
  0b00010110,  // 4
  0b00001100,  // 5
  0b00001000,  // 6
  (~0b00011001)  // 7
};

// Variável para armazenar o número atual
uint8_t count = 0;

void setup_c() {
  DDRA = 0b01111111;
  PORTA = numbers[count];
}

void loop_c() {
  // Exibe o número atual no display de 7 segmentos
  buttonState = PINB & (0b00010000);

  //essa lógica está acontecendo para qualquer momento c o botão pressionado (detectando nível)
  if (buttonState) {
    for (;count<=7 ;count++){
      Serial.print(count);
      PORTA = numbers[count];
      _delay_ms(1000);
    }
    count = 0;
  }
}

int main(){
  setup_c();
  while(1){
    loop_c();
  }
}