### Peças
#### Protótipo:
- Arduino Mega 2560;
- LCD 16x2 1602;
- Buzzer HXD;
- Sensor de temperatura DHT1111; (1x resistor)
- 2x botões (um para liga/desliga outro para modo); (2x resistores)
- Potênciometro L20;
- Resistores à gosto.


#### Projeto:
- 1x ATmega328p
- 1x LCD 16x2 1602;
- 1x Buzzer SFM-27;
- 1x Sensor de temperatura NTC (é à prova d'água! como todo NTC!); (1x resistor)
- 2x botões R13-507; (2x resistores)
- 1x Potênciometro L20;
- Resistores à gosto.
- A definir.

### Medições
- Geradas a cada 5s;
- As últimas 10 são salvas;
- O salvamento ocorre na EEPROM.

### Como alertar
Caso o buzzer não aproveite do PWM, usar LEDs.

### Formato LCD
Tempo Modo:<MODO>  
xx:xx Temp:xx.xC

Onde MODO: {"Inst.", "Méd.", "Var."}
### Quando alertar:
- Temperatura instantânea chega em determinado valor.
- Média móvel chega em determinado valor.
- Maior variação possível chega em determinado valor.

Inst.
Méd.
Var.

### Modos possíveis (mostrar no lcd)
- Temperatura instantânea
- Média móvel de temperatura
- Detectar choque térmico
  - Maior variação possível