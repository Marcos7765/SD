#include <avr/io.h>
#include<util/delay.h>
#include"ST.h"

namespace ST {

void solta(IO cfg){
  cfg.DR &= ~(cfg.PIN_MASK);
  cfg.PORT |= cfg.PIN_MASK;
}

void abaixa(IO cfg){
  cfg.DR |= cfg.PIN_MASK;
  cfg.PORT &= ~(cfg.PIN_MASK);
}

inline uint8_t escuta(IO cfg){
  return cfg.PIN & cfg.PIN_MASK;
}

void st_setup(IO cfg){
  _delay_ms(1000); //o datasheet pede 1s de descanso presse sensor fuleiro acordar
  solta(cfg);
}

//manda o sinal de start e espera o response, volta em input c pullup
inline void sendStart(IO cfg){
  solta(cfg); 
  _delay_ms(1);
  abaixa(cfg);
  _delay_ms(2); //min de 1ms de delay
  solta(cfg);
  _delay_us(30);
  while((escuta(cfg))){} //espera o response
  while(!(escuta(cfg))){} //espera o pull up do dht
  //daqui em diante são os dados/finalzinho do pullup
}

//espera o sinal de que irá mandar bit, identifica o bit e retorna imediatamente
inline uint8_t lerBit(IO cfg){
  while(escuta(cfg)){} //espera start
  while(!(escuta(cfg))){}//espera o start acabar
  _delay_us(40); //espera pra diferenciar 0 de 1
  return escuta(cfg) ? 1 : 0; //se ainda for alto é 1, cc é 0
}

uint8_t lerDados(IO cfg){
  sendStart(cfg);
  uint8_t tmp = 0;
  uint8_t res[5] = {0,0,0,0,0};
  for (uint8_t i=0; i<5; i++){
    for (uint8_t j = 8; j-- > 0; ){ //inspirado no memset
    //(uint8_t j = 7; j!=0b11111111; j--) tb funfa
      tmp = lerBit(cfg);
      res[i] |= (tmp << j);
    }
  }
  if (res[4] != ((uint8_t) (res[3]+res[2]+res[1]+res[0]))){return -1;} //compilador mudava o tipo (checksum)

  uint16_t t = (((uint16_t ) (res[2] & 0b01111111)) << 8) | res[3]; //essas conversões eu tive que
  uint16_t umi = (((uint16_t ) res[0]) << 8) | res[1];              //checar a implementação na
  Temp = (t * 0.1)*((res[2] & 0b10000000) ? -1 : 1 );               //bib da adafruit
  RH = umi * 0.1;
  return 0;
}

}