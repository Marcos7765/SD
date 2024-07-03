#include"tgfuzzy.h"
//é o TODO GLORIOSO FUUUZZZZZYYYY
namespace TGF {
float coefLinE[2][3], coefAngE[2][3];
float coefLinD[2][3], coefAngD[2][3];
const float critTempVals[3][2] = {{10.0, 65.0},{20.0,60.0},{0.5,5.0}};
const float critUmiVals[3][2] = {{20.0, 40.5},{20.0,50.0},{5.0,10.0}};

//pré-cálculo dos coeficientes pra nn calcular em td inferência
float updatePmts(float tempVals[3], float umiVals[3]){
  for (uint8_t i = 0; i<3; i++){ 
    //acreditando bastante que o compilador vai otimizar a divisão
    //do critTempVals e reordenar pra ser td esquerda -> td direita
    coefLinE[0][i] = tempVals[i]/(tempVals[i]-critTempVals[i][0]);
    coefLinE[1][i] = umiVals[i]/(umiVals[i]-critUmiVals[i][0]);
    coefLinD[0][i] = tempVals[i]/(tempVals[i]-critTempVals[i][1]);
    coefLinD[1][i] = umiVals[i]/(umiVals[i]-critUmiVals[i][1]);
    coefAngE[0][i] = (1-coefLinE[0][i])/critTempVals[i][0];
    coefAngE[1][i] = (1-coefLinE[1][i])/critTempVals[i][0];
    coefAngD[0][i] = (1-coefLinD[0][i])/critTempVals[i][1];
    coefAngD[1][i] = (1-coefLinD[1][i])/critTempVals[i][1];
  }
}

/*função de pertencimento que (pra esquerda p.ex.)
é um degrau até limite, e então se torna uma semireta
dos pontos (limite,1) até (Xzero,0). Aceito sugestões
de funções ou implementações mais eficientes.
*/
float pertencimento(float x, float xZero, uint8_t ehTemp, uint8_t modo, uint8_t ehEsq){
  if (ehEsq){
    if (ehTemp){
      if (x < critTempVals[modo][0]){return 1.0;}
    } else{
      if (x < critUmiVals[modo][0]){return 1.0;}
    }
    if (x >= xZero){return 0.0;}
    return coefAngE[(ehTemp)?0:1][modo]*x + coefLinE[(ehTemp)?0:1][modo];
  } else {
    if (ehTemp){
      if (x > critTempVals[modo][1]){return 1.0;}
    } else{
      if (x > critUmiVals[modo][1]){return 1.0;}
    }
    if (x <= xZero){return 0.0;}
    return coefAngD[(ehTemp)?0:1][modo]*x + coefLinD[(ehTemp)?0:1][modo];
  }
}

/*
Ideia: 
Variáveis fuzzy sinalizando o perigo de cada modo e uma saída de dois estados possíveis: "OK" E "RUIM";
Cada valor de cada modo pode pertencer a dois grupos, um esquerdo e um direito.
Para temperatura podemos interpretar o direito como quente e o esquerdo como frio para os modos MÉDIA E INST,
enquanto que umidade relativa como úmido (molhado) e árido (seco). Para VAR o lado esquerdo é o desejado para
as duas unidades.

PerigoMedia = (Quente AND (NOT Molhado)) OR ((NOT Frio) AND Seco) OR (Quente AND Seco);
PerigoInst  = (Quente AND (NOT Molhado)) OR ((NOT Frio) AND Seco) OR (Quente AND Seco);
PerigoVar   = (D1 AND (NOT E2)) OR ((NOT E1) AND D2) OR (D1 AND D2);

Desses três valores tiramos uma média ponderada, pensei em considerar mais a Media e menos o Inst:
PerigoTotal = (7,4,5)*(PerigoMedia, PerigoInst, PerigoVar) /16

Desse valor decidimos se é OK ou RUIM num teste qualquer de ser maior que um limiar arbitrário.
Vou ser otimista e definir esse valor como 0.7
*/
//float pertencimento(float x, float xZero, uint8_t ehTemp, uint8_t modo, uint8_t ehEsq){

/* Infelizmente nosso especialista de incêndios foi preso. 
Pelo lado bom ainda haverá show do bisteca e bochechinha na praça cívica.
As funções de saída deste sistema fuzzy portanto carecerão de um
conhecimento mais aprofundado na área.*/
float pTotal(float xs[3][2], float xZeros[3][2]){
  #define ehTemp 1
  #define ehRH 0
  #define esq 1
  #define dir 0
  float TI[2] = {pertencimento(xs[0][0], xZeros[0][0], ehTemp, 0, esq),pertencimento(xs[0][0], xZeros[0][0], ehTemp, 0, dir)};
  float UI[2] = {pertencimento(xs[0][1], xZeros[0][1], ehRH, 0, esq),pertencimento(xs[0][1], xZeros[0][1], ehRH, 0, dir)};
  
  float TM[2] = {pertencimento(xs[1][0], xZeros[1][0], ehTemp, 1, esq),pertencimento(xs[1][0], xZeros[0][0], ehTemp, 1, dir)};
  float UM[2] = {pertencimento(xs[1][1], xZeros[1][1], ehRH, 1, esq),pertencimento(xs[1][1], xZeros[0][1], ehRH, 1, dir)};
  
  float TV[2] = {pertencimento(xs[2][0], xZeros[2][0], ehTemp, 2, esq),pertencimento(xs[2][0], xZeros[2][0], ehTemp, 2, dir)};
  float UV[2] = {pertencimento(xs[2][1], xZeros[2][1], ehRH, 2, esq),pertencimento(xs[2][1], xZeros[2][1], ehRH, 2, dir)};
  #undef ehTemp
  #undef ehRH
  #undef esq
  #undef dir
  float PerigoMedia = fmax(fmin(TM[1],(1.0-UM[1])), fmax(fmin((1.0-TM[0]), UM[0]), fmin(TM[1],UM[0])));
  float PerigoInst  = fmax(fmin(TI[1],(1.0-UI[1])), fmax(fmin((1.0-TI[0]), UI[0]), fmin(TI[1],UI[0])));
  float PerigoVar   = fmax(fmin(TV[1],(1.0-UV[1])), fmax(fmin((1.0-TV[0]), UV[0]), fmin(TV[1],UV[0])));
  float PerigoTotal = (7.*PerigoMedia + 4.*PerigoInst + 5.*PerigoVar)/16.;
  return PerigoTotal;
}

//1->RUIM, 0->OK
inline uint8_t inferencia(float pTotal){return (pTotal > 0.7) ? 1 : 0;} //só pra expor o pTotal pra print
}