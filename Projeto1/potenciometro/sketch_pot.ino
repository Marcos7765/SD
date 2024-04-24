#include<avr/io.h>
//codado pra operar no A0 (PF0 no pinout)

//#define TESTE

//usando minV e maxV inteiros pra economizar op c float
//deixando a res padrão
float readPot(int16_t minV, int16_t maxV){
  ADCSRA ^= (0b01000000);
  while(ADCSRA & 0b01000000){};
  return minV+ (maxV-minV)*(ADC/1023.0);  
}

void PT_setup() {
  // put your setup code here, to run once:
  ADCSRA |= 0b10000011;
  ADMUX |=  0b11000000;
  #ifdef TESTE
  Serial.begin(9600);
  #endif
}

#ifdef TESTE

float val;
void PT_loop() {
  val = readPot(20, 50);
  Serial.println(val);
}

int main(){
  PT_setup();
  while(1){PT_loop();}
}
#endif