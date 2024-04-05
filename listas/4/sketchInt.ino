#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
//código lá da lista anterior, adaptado
//aqui vou usar interrupt de pino mas depois de pesquisar
//parece que o normal é usar um interrupt de timer
//ou montar um filtrinho passa baixa
//filtro:
//https://digilent.com/reference/learn/microprocessor/tutorials/debouncing-via-rc-filter/start
//software (usando a lib do arduino):
//https://digilent.com/reference/learn/microprocessor/tutorials/debouncing-via-software/start
uint8_t buttonState[2] = {0,0};
uint8_t buttonInput[2] = {0,0};
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
  //testando usar qualquer mudança lógica
  EIMSK = 0b00000011;
  EICRA = 0b00000101;
  sei();
}

void btA1(){
  count = count<7 ? count+1 : 7;
  PORTA = numbers[count];
  Serial.print(count);
}

void btA2(){
  count = 0;
  PORTA = numbers[count];
  Serial.print(count);
}

ISR(INT0_vect){
  //não parece ter mudança significativa na eficácia
  buttonInput[0] = PIND & 0b00000001;
  if (buttonInput[0] != buttonState[0] && buttonInput[0]) {
    btA2();
  }
  //esse delay horroroso é porque não tô usando um timer pra debounce
  _delay_ms(100);
  buttonState[0] = buttonInput[0];
}

//por alguma razão nn tá dando pra ativar esse botão sem antes ativar o outro
ISR(INT1_vect){
  //Serial.print(count);
  buttonInput[1] = PIND & 0b00000010;
  if (buttonInput[1] != buttonState[1] && buttonInput[1]) {
    btA1();
  }
  _delay_ms(100);
  buttonState[1] = buttonInput[1];
}

int main(){
  setup_c();
  while(1){
  }
}