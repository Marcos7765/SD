uint8_t aceso = 0;
uint8_t leitura = 0;
void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, INPUT);
  pinMode(24, OUTPUT);
  //o 22 vai estar servindo como o +5V só pelo
  //caso de se ter trocado as palavras em
  //"Utilize o sinal de saída do botão 
  //como entrada do microcontrolador"
  digitalWrite(22, HIGH);

}

void loop() {
  leitura = digitalRead(23);
  if (leitura != aceso){
    aceso = leitura;
    if (aceso){
      digitalWrite(24, HIGH);
    } else{
      digitalWrite(24, LOW);
    }
    delay(50);
  }
}