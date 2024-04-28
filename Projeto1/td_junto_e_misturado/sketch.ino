#include <LiquidCrystal.h>
#include <stdlib.h>
#include<DHT.h>
#include<util/delay.h>
#include<string.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include <avr/eeprom.h>

enum MODO {
  INSTA=0,
  MEDIA=1,
  VARIA=2
};

struct Time {
  uint8_t second;
  uint8_t minute;
};

//PARÂMETROS
#define debounce_ms 50
#define tempo_amostra_s 5

// ---------- VARIÁVEIS GLOBAIS ----------
float temperaturas[10];
uint8_t amostradas = 0;
uint8_t cfg = true;
uint8_t cfgMax = true;
uint8_t modo = INSTA;
float maxVals[3] = {42.0, 40.5, 5.0};
float minVals[2] = {35.0, 35.5};
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
struct Time time;

#define ST
//LÓGICA DO SENSOR
#ifdef ST

DHT dht(22, DHT22);

void ST_setup(){
  dht.begin();
  _delay_ms(1000); //datasheet da AOSONG sugere esperar 1s depois de iniciar,
                   //e no .cpp da classe ele não tem essa espera.
}

//o ponto td de ter esse sensor
//use isnan para checar se o valor está certo (servirá pra botar no display)
float getTemp(){
  memmove(temperaturas+1, temperaturas, 9*(sizeof(float)));
  temperaturas[0] = dht.readTemperature();
  amostradas = (amostradas == 10) ? 10 : amostradas+1;
}
#endif

#define PT
//LÓGICA DO POTENCIÔMETRO
#ifdef PT

float readPot(int16_t minV, int16_t maxV){
  ADCSRA ^= (0b01000000);
  while(ADCSRA & 0b01000000){};
  return minV+ (maxV-minV)*(ADC/1023.0);  
}

void PT_setup() {
  ADCSRA |= 0b10000011;
  ADMUX |=  0b11000000;
}
#endif

#define BZ
//LÓGICA DO BUZZER
#ifdef BZ

void BZ_setup() {
  //fast pwm, nn invertido no timer 0
  DDRB |= 0b10000000;
  OCR0A = 0;
  TCCR0A |= 0b11000011;
  //sem prescale
  TCCR0B |= 0b00000001;
}

//variar
const uint8_t tons[] = {100, 130};
void sirene(){
  static uint8_t incr = 0;
  
  OCR0A = tons[incr];
  incr = (incr+1)%2;
}


void BZ_clear(){
  DDRB &= ~0b10000000;
}

void BZ_set(){
  DDRB |= 0b10000000;
}
#endif

//LÓGICA DE INTERRUPÇÃO
void int_setup(){
  TCCR1A = 0b00000000; //nn precisa mexer pro timer1
  TCCR1B = 0b00001101;
  OCR1A = 15624; //t_estouro invertido pra calc top
  TIMSK1 = 0b00000010;
  sei();
}

//LÓGICA DE MODOS
//vai ligar no modo cfg, dele pode ir para o exec e de lá só religando
void startExec(){
  cli();
  if (!cfg){
    //chamadas posteriores só resetam o timer/amostras
    getTemp();
    for (uint8_t i =1; i<10; i++){
      temperaturas[i] = 0;
    }
    amostradas = 1;
  }
  time.second = 0;
  time.minute = 0;
  cfg = false;
  sei();
}

void switchMode(){
  modo = (modo+1)%3;
}


inline void incrementTime(struct Time *time) {
    time->second += 1;
    if (time->second == 60) {
        time->second = 0;
        time->minute = time->minute + 1;
        if (time->minute == 60) {
            time->minute = 0;
        }
    }
}

inline void printTemp(float temp, LiquidCrystal &lcd) {
  lcd.setCursor(11, 1);
  lcd.print(String(temp, 1));
  lcd.setCursor(15, 1);
  lcd.print("C");
}

inline void printMode(unsigned int mode, LiquidCrystal &lcd) {
  const char testeooo[][5] = {"INST","MED ","VAR "};
  lcd.setCursor(6, 1);
  //lcd.print(String("0") + String(mode) + " |");
  lcd.print(String(testeooo[mode]) + "|");
}

inline void printTime(struct Time *time, LiquidCrystal &lcd) {
  lcd.setCursor(0,1);
  lcd.print(String(time->minute) + ":" + (time->second < 10 ? "0" : "") + String(time->second) + " |");
}

inline void printCfg(LiquidCrystal &lcd) {
  lcd.setCursor(0,1);
  lcd.print((cfgMax || (modo==VARIA)) ? String(" Max.|") : String(" Min.|"));
}

//LÓGICA DOS BOTÕES
void BTS_setup(){ //desnecessário mas deixei pra garantir
  DDRA &= ~(0b00000110);
}

//tentando bastante fazer o otimizador deixar eu chamar o switchMode de cfgButton só pra handleBts
inline void cfgButton(){
  return startExec();
}
//leia o de cima e troque os nomes
inline void modeButton(){
  if (cfg){
    if (cfgMax){
      if (modo != VARIA){cfgMax = false;}
      else{switchMode();}
    } else{
      cfgMax = true;
      switchMode();
    }
    printCfg(lcd);
  } else{
    switchMode();
  }
  printMode(modo, lcd);
}

uint8_t btsState[2] = {0,0};
uint8_t btsInput[2] = {0,0};

inline void readBts(){
  btsInput[0] = PINA & 0b00000010;
  btsInput[1] = PINA & 0b00000100;
}

//alterar a ordem -> alterar a prioridade
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

//LÓGICA DA EEPROM
#define EEPROM_ADDRESS 0

void writeFloat(int address, float data) {
  eeprom_write_block((const void*)&data, (void*)address, sizeof(float));
}

float readFloat(int address) {
  float data;
  eeprom_read_block((void*)&data, (const void*)address, sizeof(float));
  return data;
}

#define MAX_ADDR EEPROM_ADDRESS + modo*sizeof(float)
#define MIN_ADDR EEPROM_ADDRESS + 3 + modo*sizeof(float)
void handleConfig(float potVal){
  if (cfgMax){
    if (potVal != maxVals[modo]){
      maxVals[modo] = potVal;
      writeFloat(MAX_ADDR, potVal);
    }
  } else {
    if (potVal != minVals[modo]){
      minVals[modo] = potVal;
      writeFloat(MIN_ADDR, potVal);
    }
  }
}

void loadConfig(){
  for (uint8_t i = 0; i<3;i++){
    if (i<2){
      minVals[i] = readFloat(EEPROM_ADDRESS + 3 + i*sizeof(float));
    }
    maxVals[i] = readFloat(EEPROM_ADDRESS + i*sizeof(float));
  }
}

uint8_t apita = false;
float gtVar = 0;
float mtmp;

void handleTemp(){
  if (apita){
    sirene();
    return;
  }
  //inst
  if (temperaturas[0] > maxVals[0] || temperaturas[0] < minVals[0]){
    BZ_set();
    sirene();
    apita = true;
    Serial.print("inst: ");
    Serial.println(temperaturas[0]);
    return;
  }
  //média
  mtmp = 0;
  float cand;
  //sempre acontece depois de uma amostragem, ent nn tem como ter 0
  for (uint8_t i = 0; i<amostradas; i++){
    mtmp += temperaturas[i];
    cand = (temperaturas[0] - temperaturas[i]);
    cand = cand > 0 ? cand : -cand;
    gtVar = (gtVar > cand) ? gtVar : cand;
  }
  mtmp = mtmp/amostradas;
  if (mtmp > maxVals[1] || mtmp < minVals[1]){
    BZ_set();
    sirene();
    apita = true;
    return;
  }
  if (gtVar > maxVals[2]){
    Serial.print("var: ");
    Serial.println(gtVar);
    BZ_set();
    sirene();
    apita = true;
    return;
  }
}

void printStat(){
  switch (modo) {
    case INSTA:
      printTemp(temperaturas[0], lcd);
    break;
    case MEDIA:
      printTemp(mtmp, lcd);
    break;
    case VARIA:
      printTemp(gtVar, lcd);
    break;
  }
}

float potTemp;

void setup() {
  //time = (struct Time *)malloc(sizeof(struct Time));
  Serial.begin(9600);
  time.second = 0;
  time.minute = 0;
  lcd.begin(16, 2);
  PT_setup();
  BZ_setup();
  ST_setup();
  BTS_setup();
  int_setup();
  potTemp = readPot(20,50);
  _delay_ms(10); //evitar da interrupção estar síncrona com printTemp
}

uint8_t checar = 0;

ISR(TIMER1_COMPA_vect){
  lcd.setCursor(0, 0);
  if (cfg){
    lcd.print(" Cfg |");
    printCfg(lcd);
  } else{
    lcd.print("Tempo|");
    if (checar==0) {getTemp();} //vamos coletar amostras
    checar = (checar+1)%tempo_amostra_s;
    handleTemp();
    printStat();
    printTime(&time, lcd);
  }

  lcd.setCursor(6, 0);
  lcd.print("Modo|");

  lcd.setCursor(11, 0);
  lcd.print("Temp");

  printMode(modo, lcd);

  //desnecessariamente rodaria enquanto no cfg
  incrementTime(&time);
}

void loop() {

  handleBts();
  if (cfg){
    potTemp = readPot(20,50);
    handleConfig(potTemp);
    printTemp(potTemp, lcd);
  }
  _delay_ms(debounce_ms);
}