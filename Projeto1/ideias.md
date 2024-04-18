### Peças
#### Protótipo:
- Arduino Mega 2560;
- LCD 16x2 (inserir nome);
- Buzzer (inserir nome, ver Como alertar);
- Sensor de temperatura NTC (é à prova d'água);
- Dois botões (um para liga/desliga outro para modo);
- Resistores à gosto.


#### Projeto:
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

Onde MODO = $\{\text{Inst.},\ \text{Méd.}\}$
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