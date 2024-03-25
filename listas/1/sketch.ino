//A explicação das diferenças está no código em C.

void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
}

void pisca(uint8_t pin, double time_ms){
  digitalWrite(pin, HIGH);
  delay(time_ms);
  digitalWrite(pin, LOW);
}

void loop() {
  pisca(22, 5000);
  pisca(23, 2000);
  pisca(24, 5000);
}