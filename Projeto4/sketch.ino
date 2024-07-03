#include<avr/io.h>
#include<util/delay.h>
#include<string.h>
#include"ST.h"
#include"buzzer.h"
#include"potenciometro.h"
#include"lcd.h"
#include"tgfuzzy.h"
#include"Arduino.h"

//PARÂMETROS
#define debounce_ms 200
#define tempo_amostra_s 5

ST::IO cfg = {DDRL, PORTL, PINL, 1};
LCD::Time tempo = {0, 0};

float pTotal = 0.0;
float temperaturas[10], umidades[10];
uint8_t amostradinho = 0;
float tempVals[3] = {20.0, 40.5, 5.0};
float umiVals[3] = {35.0, 35.5, 7.5};
float tempRanges[3][2] = {{10.0, 65.0},{20.0,60.0},{1.0,5.0}};
float umiRanges[3][2] = {{20.0, 40.5},{20.0,50.0},{5.0,10.0}};
uint8_t btsState[2] = {0,0};
uint8_t btsInput[2] = {0,0};
uint8_t emCfg = true;
uint8_t cfgTemp = true;
uint8_t modo = 0;
uint8_t prerigo =0; //pode ser representado por uma caveira
float potVal = 0.0;
uint8_t checar=0;

void incrementTime(struct LCD::Time *time) {
    time->second += 1;
    if (time->second == 60) {
        time->second = 0;
        time->minute = time->minute + 1;
        if (time->minute == 60) {time->minute = 0;}
    }
}

void int_setup(){
  TCCR1A = 0b00000000; //nn precisa mexer pro timer1
  TCCR1B = 0b00001101;
  OCR1A = 15624; //t_estouro invertido pra calc top
  TIMSK1 = 0b00000010;
  sei();
}

void main_setup(){
  ST::st_setup(cfg);
  LCD::lcd_setup();
  PT::PT_setup();
  BZ::BZ_setup();
  int_setup();
  _delay_ms(10);
}

ISR(TIMER1_COMPA_vect){
  //lcd_clear();
  if (emCfg){
    LCD::printConfig(cfgTemp, modo, tempVals[modo], umiVals[modo]);
  } else{
    if (checar==0) {
      ST::lerDados(cfg);
      Serial.print("Temp: "); Serial.print(ST::Temp);
      Serial.print("; RH: "); Serial.println(ST::RH);
      handleRead();
    }
    checar = (checar+1)%tempo_amostra_s;
    LCD::printExec(pTotal, prerigo, tempo);
    incrementTime(&tempo);
  }
}

inline void readBts(){
  btsInput[0] = PING & 0b00000001;
  btsInput[1] = PING & 0b00000010;
}

void cfgButton(){
  Serial.println("cfgButton!");
  if (emCfg){
    if (!cfgTemp) {modo = (modo + 1)%3;}
    cfgTemp = !cfgTemp;
  } else{
    modo = (modo + 1)%3;
  }
}
void modeButton(){
  Serial.println("modeButton!");
  if (emCfg){
    emCfg = false;
    TGF::updatePmts(tempVals, umiVals);
  } else{
    amostradinho = 0;
    tempo.second = 0;
    tempo.minute = 0;
    checar = 0;
    prerigo = 0;
    BZ_clear();
  }
}

void handleRead(){
  if (prerigo){sirene(); return;} //se deu ruim deu ruim e você tem que reiniciar
  //má separação de responsabilidades
  memmove(temperaturas+1, temperaturas, 9*sizeof(float));
  memmove(umidades+1, umidades, 9*sizeof(float));
  temperaturas[0] = ST::Temp;
  umidades[0] = ST::RH;
  amostradinho = (amostradinho == 10) ? 10 : amostradinho+1;

  float xs[3][2];
  xs[0][0] = temperaturas[0];
  xs[0][1] = umidades[0];
  xs[1][0] = 0;
  xs[1][1] = 0;
  xs[2][0] = 0; //qualquer valor menor ou igual a 0 já sera 100% ESQ, 0% DIR
  xs[2][1] = 0; //então usar o 0 de ponto inicial pro trono de maior variação
                //é a mesma coisa que arranjar o -Inf

  for (uint8_t i = 0; i<amostradinho; i++){
    xs[1][0] += temperaturas[i];
    xs[1][1] += umidades[i];
    if ((temperaturas[0]-temperaturas[i]) > xs[2][0]){xs[2][0] = (temperaturas[0]-temperaturas[i]);}
    if ((umidades[0]-umidades[i]) > xs[2][1]){xs[2][1] = (umidades[0]-umidades[i]);}
  }
  xs[1][0] /= amostradinho;
  xs[1][1] /= amostradinho;

  //escrevi a função sem concordar com o tipo de dado que já tinha e agora essa cagada aqui serve
  //pra resolver no menor esforço
  float xZeros[3][2] = {{tempVals[0],umiVals[0]},{tempVals[1],umiVals[1]},{tempVals[2],umiVals[2]}};

  pTotal = TGF::pTotal(xs, xZeros);
  Serial.print("pTotal: "); Serial.println(pTotal);
  if (TGF::inferencia(pTotal)){prerigo=1; BZ_set(); sirene();}
}

inline void handleBts(){
  readBts();
  if (btsInput[0] != btsState[0] && btsInput[0]){
    cfgButton();
  }
  if (btsInput[1] != btsState[1] && btsInput[1]){
    modeButton();
  }
  btsState[0] = btsInput[0];
  btsState[1] = btsInput[1];
}

void handleInput_cfg(){
  potVal = PT::readPot(cfgTemp ? tempRanges[modo][0] : umiRanges[modo][0], cfgTemp ? tempRanges[modo][1] : umiRanges[modo][1]);
  if (cfgTemp){tempVals[modo] = potVal;} else{umiVals[modo] = potVal;}
}

int main(){
  main_setup();
  Serial.begin(9600);
  Serial.println("! ");
  while(true){
    if (emCfg) {handleInput_cfg();}
    handleBts();
    _delay_ms(debounce_ms);
  }
}