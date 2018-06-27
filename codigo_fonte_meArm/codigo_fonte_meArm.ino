/* Projeto Esteira
 * Microcontroladores
 * 27/06/18
 * Ultima Alteração: 27/06/18 - 01h06
*/

#include "meArm_Adafruit.h"

int x=0;
int y=180;
int z=100;

int tarefa=0; //recebe qual tarefa deve realizar
boolean bracoConfig = true; //define em qual braco seta posicao para teste true = braco1, false = braco2

meArm braco2;
meArm braco1(145, 49, -pi/4, pi/4,
      118,22,pi/4,3*pi/4,
      144,36, pi/4,-pi/4,
      105, 115, pi/2,pi/4);

void setup() {
  Serial.begin(9600);
  braco1.begin();
  braco2.begin(1);
  posRepouso1();
  posRepouso2();
  
}

void loop() {
  //receberValor();
  //funPrint();
  controleBraco();
}

void controleBraco(){

  if (tarefaRecebida()) {
    tarefa = Serial.read();
    
    if (tarefa == 'P') {
      Serial.print('P');
      moveParaPonto1();
      liberarEsteira();
    }
    
    else if (tarefa == 'M') {
      Serial.print('M');
      moveParaPonto2();
      liberarEsteira();
    }
  }
}

boolean tarefaRecebida(){
  return (Serial.available() > 0); //se maior que zero, tem tarefa
}

void moveParaPonto1(){
  pegarObjeto1();
  
  braco1.gotoPoint(-170,140,5); //leva ate a posicao de soltar
  delay(300);
  braco1.openGripper();

  posRepouso1();
  pegarObjeto2();
  
}

void moveParaPonto2(){
  pegarObjeto1();
  
  //INCOMPLETA
  
  posRepouso1();
}

void pegarObjeto1(){
  posEsteira1();
  braco1.openGripper();
  
  braco1.gotoPoint(0,215,0);  // baixa ate o objeto
  braco1.closeGripper();       // agarra o objeto
  posEsteira1();             // levanta o objeto
}

void pegarObjeto2(){
  braco2.gotoPoint(0,120,120); //pega objeto
  braco2.gotoPoint(-60,160,110);
  delay(300);
  braco2.gotoPoint(80,160,110);
  posRepouso2();  
}

void posRepouso1(){
  braco1.gotoPoint(0,140,60);
  braco1.closeGripper();
}

void posRepouso2(){
  braco2.gotoPoint(0,120,100);
  braco2.closeGripper();
}

void posEsteira1(){ //posiciona o braço acima da esteira
    braco1.gotoPoint(0,200,60);
}

void liberarEsteira(){
  Serial.print('C');
}


//************************* FUNCOES DE TESTES *******************************
void funPrint(){
  Serial.print("x = ");
  Serial.print(x);
  Serial.print(" y = ");
  Serial.print(y);
  Serial.print(" z = ");
  Serial.print(z);
  Serial.print(" GETs: x = ");
  Serial.print(braco1.getX());
  Serial.print(" y = ");
  Serial.print(braco1.getY());
  Serial.print(" z = ");
  Serial.print(braco1.getZ());
  Serial.print("   |   |   |   x = ");
  Serial.print(x);
  Serial.print(" y = ");
  Serial.print(y);
  Serial.print(" z = ");
  Serial.print(z);
  Serial.print(" GETs: x = ");
  Serial.print(braco2.getX());
  Serial.print(" y = ");
  Serial.print(braco2.getY());
  Serial.print(" z = ");
  Serial.print(braco2.getZ());
  Serial.print(" (((");
  Serial.println(bracoConfig);
}

void receberValor(){
  if(Serial.available()){   //verifica se tem entrada de dados
    char buf[14]; //guarda os dados recebidos
    int index=0;
    delay(20); //espera tempo de carregar todos os dados
    int qntChar = Serial.available(); //quantidade de dados recebido
    
    if(qntChar>14) //limita a quantidade de dados para n estourar o buf
      qntChar=14;
   
    while(qntChar--) //armazenando os dados no buf
      buf[index++]=Serial.read();
    dividirString(buf);
    for(int i=0;i<14;i++) //limpa o buffer serial
      buf[i]='\0'; 
  }
}

void dividirString(char* dado){

  Serial.print("Dado recebido: ");
  Serial.print(dado);
  char* parameter;
  parameter = strtok(dado," ,");
  while(parameter != NULL){        //Se NULL indica que chegou ao final da string
    if(bracoConfig) 
      setEixo(parameter, &braco1);
    else setEixo(parameter, &braco2);
    parameter = strtok(NULL, " ,");  //NULL indica pra continuar de onde parou
  }
}

void setEixo(char* dado,meArm *braco){
  if(dado[0]=='x'){
    x = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco->gotoPoint(x, braco->getY(),braco->getZ());
  }
  else if(dado[0]=='y'){ //1 = ativa/desativa INTCN
    y = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco->gotoPoint(braco->getX(),y,braco->getZ());
  }
  else if(dado[0]=='z'){
    z = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco->gotoPoint(braco->getX(), braco->getY(),z);
  }
  else if(dado[0]=='a'){
    braco1.openGripper();
  }
  else if(dado[0]=='f'){
    braco1.closeGripper();
  }
  else if(dado[0]=='b'){
    braco2.openGripper();
  }
  else if(dado[0]=='c'){
    braco2.closeGripper();
  }
  else if(dado[0]=='q'){
    moveParaPonto1();
  }
  else if(dado[0]=='w'){
    pegarObjeto2();
  }
  else if(dado[0]=='m'){
    bracoConfig=!bracoConfig;
  }
}

