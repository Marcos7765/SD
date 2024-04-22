#include<avr/io.h>
#include<util/delay.h>
//esse rascunho era para o dht22/dht11 em c puro, depois que eu percebi que tinha o dht22 no wokwi e lembrar o dht11 é o sensor do protótipo
//a ideia desse era usar o input c pullup pra ser o high do sinal, diferente do que usa a DHT.h
typedef struct ST_IO {
  volatile uint8_t& DR;
  volatile uint8_t& PORT;
  volatile uint8_t& PIN;
  uint8_t PIN_MASK;
} ST_IO;

//volta para input e garante o pullup
void solta(ST_IO cfg){
  cfg.DR &= ~(cfg.PIN_MASK);
  cfg.PORT |= cfg.PIN_MASK;
}
//vai pra output e escreve baixo
void abaixa(ST_IO cfg){
  cfg.DR |= cfg.PIN_MASK;
  cfg.PORT &= ~(cfg.PIN_MASK);
}

uint8_t escuta(ST_IO cfg){
  return cfg.PIN & cfg.PIN_MASK;
}

void ST_setup(ST_IO cfg){
  _delay_ms(1000); //o datasheet pede 1s de descanso presse sensor fuleiro acordar
  solta(cfg);
  //cfg.DR |= cfg.PIN_MASK;
}

//manda o sinal de start e espera o response, volta em input c pullup
inline void sendStart(ST_IO cfg){
  abaixa(cfg);
  _delay_ms(18);
  solta(cfg);
  _delay_us(20);
  while((cfg.PIN & cfg.PIN_MASK)){_delay_us(20);} //espera o response
  while(!(cfg.PIN & cfg.PIN_MASK)){_delay_us(20);} //espera o pull up do dht
  _delay_us(60); //espera o pull up acabar
  //daqui em diante são os dados/finalzinho do pullup
}

//espera o sinal de que irá mandar bit, identifica o bit e retorna imediatamente
inline uint8_t lerBit(ST_IO cfg){
  uint8_t res = 0;
  while((cfg.PIN & cfg.PIN_MASK)){_delay_us(5);} //espera start, delay baixo pra nn esperar dms dps
  _delay_us(50); //garante que acabou o start
  res = escuta(cfg);
  _delay_us(40); //somente o 1 está alto ainda (se for 0 e o último bit mandado ele ainda segura 50us no baixo)
  return escuta(cfg) && res;
}

uint8_t lerTemp(ST_IO cfg){
  sendStart(cfg);
  uint8_t temp;
  uint8_t res = 0;
  for (uint8_t i=0; i<40; i++){
    temp = lerBit(cfg);
    if ((i > 7) && (i < 16)){
      res = res | (temp << (i-8));
    }
  }
  return res;
}

ST_IO cfg = {DDRA, PORTA, PINA, 1};


int main(){
  ST_setup(cfg);
  uint8_t T;
  Serial.begin(9600);
  Serial.print("Teste");
  Serial.println("! ");
  while(1){
    T = lerTemp(cfg);
    Serial.println(T);
    _delay_ms(1000);
  }
}