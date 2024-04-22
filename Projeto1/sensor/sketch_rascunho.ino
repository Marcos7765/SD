#include <avr/io.h>
#include<util/delay.h>
//esse rascunho era tentando fazer em C puro para o ds18b20
uint8_t teste1;
uint8_t teste2;

//garante que o bus está em saída e escreve 0
inline void send0(volatile uint8_t& reg, volatile uint8_t& port, uint8_t pinMask){
  reg |= pinMask;
  port &= ~pinMask;
}
//garante que o bus está em saída e escreve 1
inline void send1(volatile uint8_t& reg, volatile uint8_t& port, uint8_t pinMask){
  reg |= pinMask;
  port |= pinMask;
}

inline void inputMode(volatile uint8_t& reg, uint8_t pinMask){
  reg &= ~pinMask;
}

inline uint8_t readBit(volatile uint8_t& reg, volatile uint8_t& PIN, uint8_t pinMask){
  reg &= ~pinMask;
  return PIN & pinMask;
}

inline void write0(volatile uint8_t& reg, volatile uint8_t& port, uint8_t pinMask){
  send0(reg, port, pinMask);
  _delay_us(60);
  inputMode(reg, pinMask);
  _delay_us(10);
}

inline void write1(volatile uint8_t& reg, volatile uint8_t& port, uint8_t pinMask){
  send0(reg, port, pinMask);
  _delay_us(5);
  inputMode(reg, pinMask);
  _delay_us(65);
}

void resetSensor(volatile uint8_t& reg, volatile uint8_t& port, volatile uint8_t& PIN, uint8_t pinNo){
  uint8_t pinMask = (1 << pinNo);
  send0(reg, port, pinMask);
  _delay_us(480);
  inputMode(reg, pinMask);
  //teste2 = PINA;
  _delay_us(80);
  //teste1 = readBit(reg, PIN, pinMask);
  _delay_us(400);
}

void writeByte(volatile uint8_t& reg, volatile uint8_t& port, uint8_t pinNo, uint8_t data){
  uint8_t pinMask = 1 << pinNo;
  for (uint8_t i = 0; i<8; i++){
    if ((data >> i) & 1){
      write1(reg, port, pinMask);
    } else{
      write0(reg, port, pinMask);
    }
  }
}

uint8_t readSensor(volatile uint8_t& reg, uint8_t pinNo){
  uint8_t pinMask = (1 << pinNo);
  reg &= ~pinMask;
}

void setup_c() {
  Serial.begin(9600);
  send1(DDRA, PORTA, 0b00000001);
}

int main(){
  setup_c();
  while(1){
    resetSensor(DDRA, PORTA, PINA, 0);
    Serial.print(teste1);
    Serial.println(" teste1");
    Serial.print(teste2);
    Serial.println(" teste2");
    _delay_ms(2000);
  }
}
