#include<avr/io.h>
#include<util/delay.h>
//código lá da lista anterior, adaptado
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
}

void handleA1(){
    if (buttonInput[1] != buttonState[1] && buttonInput[1]) {
        count = count<7 ? count+1 : 7;
        PORTA = numbers[count];
        Serial.print(count);
    }
    buttonState[1] = buttonInput[1];
}

void handleA2(){
    if (buttonInput[0] != buttonState[0] && buttonInput[0]) {
        count = 0;
        PORTA = numbers[count];
        Serial.print(count);
    }
    buttonState[0] = buttonInput[0];
}

void loop_c() {
  buttonInput[0] = PIND & 0b00000001;
  buttonInput[1] = PIND & 0b00000010;
  
  handleA1();
  handleA2();
  
  _delay_ms(100);
}

int main(){
  setup_c();
  while(1){
    loop_c();
  }
}

//pino, p.ino, entendeu? kkkkkkkkkkkkkkkkkkkkkk