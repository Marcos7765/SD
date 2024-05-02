#include <stdlib.h>
#include<DHT.h>
#include<util/delay.h>
#include<string.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include <avr/eeprom.h>

// ========================================================================
// Configurando o LCD
// ========================================================================

// LCD - Pinos de controle
#define CONTR_LCD PORTH
#define RS PH5
#define E PH6

// LCD - Pinos de dados (8 bits)
#define DADOS_LCD PORTC
#define nibble_dados 1

#define pulse_enable() _delay_us(1); set_bit(CONTR_LCD, E); _delay_us(1); clr_bit(CONTR_LCD, E); _delay_us(45);

#define set_bit(y,bit) (y|=(1<<bit))   // Coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit))  // Coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit))   // Troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit))    // Retorna 0 ou 1 conforme leitura do bit

void lcd_cmd(unsigned char c, char cd)
{
  // cd : indica se instrução (0) ou caractere (1)
  if(cd == 0)
    clr_bit(CONTR_LCD,RS);
  else 
    set_bit(CONTR_LCD,RS);

  // Primeiro nibble de dados - 4 MSB
  #if (nibble_dados)  
    // Compila o código para os pinos de dados do LCD nos 4 MSB do PORT
    DADOS_LCD = (DADOS_LCD & 0x0F)|(0xF0 & c);
  #else 
    // Compila o código para os pinos de dados do LCD nos 4 LSB do PORT
    DADOS_LCD = (DADOS_LCD & 0xF0)|(c >> 4);
  #endif
  pulse_enable();
  
  // Segundo nibble de dados - 4 LSB
  #if (nibble_dados) 
    // Compila o código para os pinos de dados do LCD nos 4 MSB do PORT
    DADOS_LCD = (DADOS_LCD & 0x0F) | (0xF0 & (c << 4));
  #else 
    // Compila o código para os pinos de dados do LCD nos 4 LSB do PORT
    DADOS_LCD = (DADOS_LCD & 0xF0) | (0x0F & c);
  #endif
  pulse_enable();
  
  // Se for instrução de retorno ou limpeza espera LCD estar pronto
  if((cd == 0) && (c < 4)) _delay_ms(2);
}

void lcd_init_4bits()
{ 
  // Como o LCD será só escrito, R/W é sempre zero.
  clr_bit(CONTR_LCD,RS);  // RS em zero indicando que o dado para o LCD será uma instrução
  clr_bit(CONTR_LCD,E);   // Pino de habilitação em zero

  // Tempo para estabilizar a tensão do LCD, após VCC ultrapassar 4.5 V (na prática pode ser maior)
  _delay_ms(20);
  
  // Interface de 8 bits
  #if (nibble_dados)
    DADOS_LCD = (DADOS_LCD & 0x0F) | 0x30;
  #else
    DADOS_LCD = (DADOS_LCD & 0xF0) | 0x03;
  #endif
  pulse_enable(); // Habilitação respeitando os tempos de resposta do LCD
  _delay_ms(5);
  pulse_enable();
  _delay_us(200);
  pulse_enable(); 

  // Interface de 4 bits
  // Deve ser enviado duas vezes (a outra está abaixo)
  #if (nibble_dados)
    DADOS_LCD = (DADOS_LCD & 0x0F) | 0x20;
  #else
    DADOS_LCD = (DADOS_LCD & 0xF0) | 0x02;
  #endif
  pulse_enable();

  // Interface de 4 bits 2 linhas 
  // Aqui se habilita as 2 linhas
  lcd_cmd(0x28,0); 

  // São enviados os 2 nibbles (0x2 e 0x8)
  lcd_cmd(0x08,0); // Desliga o display
  lcd_cmd(0x01,0); // Limpa todo o display
  lcd_cmd(0x0C,0); // Mensagem aparente cursor inativo não piscando
  lcd_cmd(0x80,0); // Inicializa cursor na primeira posição a esquerda
}

void lcd_write(char *c)
{
  for (; *c!=0;c++) lcd_cmd(*c,1);
}

void lcd_set_cursor(unsigned char linha, unsigned char coluna) {
    unsigned char posicao = 0x80; // Endereço base da primeira linha do LCD

    // Ajusta a posição base dependendo da linha
    if (linha == 0) {
        posicao = 0x80; // Endereço base da primeira linha do LCD
    } else if (linha == 1) {
        posicao = 0xC0; // Endereço base da segunda linha do LCD
    }

    // Ajusta a posição base dependendo da coluna
    posicao += coluna - 1;

    // Envia o comando para definir a posição do cursor no LCD
    lcd_cmd(posicao, 0);
}

// ========================================================================
// Fim da configuração do LCD
// ========================================================================

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
#define debounce_ms 200
#define tempo_amostra_s 5

// ---------- VARIÁVEIS GLOBAIS ----------
float temperaturas[10];
uint8_t amostradas = 0;
uint8_t cfg = true;
uint8_t cfgMax = true;
uint8_t modo = INSTA;
float maxVals[3] = {42.0, 40.5, 5.0};
float minVals[2] = {35.0, 35.5};
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

inline void printTemp(float temp) {
  char tempStr[6]; // Buffer para armazenar a string de temperatura
  dtostrf(temp, 5, 2, tempStr); // Formata o valor de ponto flutuante em uma string
  lcd_set_cursor(1, 12);
  lcd_write(tempStr);
  lcd_set_cursor(1, 16);
  lcd_write("C");
}

inline void printMode(unsigned int mode) {
  const char *modes[] = {"INST","MED ","VAR "};
  lcd_set_cursor(1, 7);
  lcd_write(modes[mode]);
  lcd_write("|");
}

inline void printTime(struct Time *time) {
  char timeStr[9]; // Buffer para armazenar a string de tempo (HH:MM:SS)
  snprintf(timeStr, 9, "%02d:%02d", time->minute, time->second); // Formata o tempo
  lcd_set_cursor(1, 6);
  lcd_write("|");
  lcd_set_cursor(1, 1);
  lcd_write(timeStr);
}

inline void printCfg() {
  const char *cfgLabel = (cfgMax || (modo==VARIA)) ? " Max.|" : " Min.|";
  lcd_set_cursor(1, 1);
  lcd_write(cfgLabel);
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
      else{return;}
    } else{
      cfgMax = true;
      switchMode();
    }
    printCfg();
  } else{
    switchMode();
  }
  printMode(modo);
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
      printTemp(temperaturas[0]);
    break;
    case MEDIA:
      printTemp(mtmp);
    break;
    case VARIA:
      printTemp(gtVar);
    break;
  }
}

float potTemp;

void setup() {
  Serial.begin(9600);
  time.second = 0;  // Zerar segundo do timer
  time.minute = 0;  // Zerar minuto do timer
  DDRH = 0xff;      // Configurar portas de saida do LCD
  DDRC = 0xff;      // Configurar portas de saida do LCD
  lcd_init_4bits(); // Iniciar LCD
  PT_setup();       // Setup do potenciômetro
  BZ_setup();       // Setup do buzzer
  ST_setup();       // Setup do sensor
  BTS_setup();
  int_setup();
  potTemp = readPot(20,50);
  _delay_ms(10);
}

uint8_t checar = 0;

ISR(TIMER1_COMPA_vect){
  if (cfg){
    lcd_set_cursor(0, 2);
    lcd_write("Cfg |");
    printCfg();
  } else{
    lcd_set_cursor(0, 1);
    lcd_write("Tempo|");
    if (checar==0) {getTemp();} //vamos coletar amostras
    checar = (checar+1)%tempo_amostra_s;
    handleTemp();
    printStat();
    printTime(&time);
  }

  lcd_set_cursor(0, 7);
  lcd_write("Modo|");

  lcd_set_cursor(0, 12);
  lcd_write("Temp");

  printMode(modo);

  //desnecessariamente rodaria enquanto no cfg
  incrementTime(&time);
}

void loop() {

  handleBts();
  if (cfg){
    if (modo == VARIA){
      potTemp = readPot(0,10);
    }else{
      potTemp = readPot(20,50);
    }
    handleConfig(potTemp);
    printTemp(potTemp);
  }
  _delay_ms(debounce_ms);
}