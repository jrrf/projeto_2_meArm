#include "meArm_Adafruit.h"

#define pinISR 5

//char buf[14];

int x=0;
int y=180;
int z=100;

// 133 461
// 0 180
// 

//meArm braco1;
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
  posRepouso1();
  pinMode(pinISR, OUTPUT);
//  attachInterrupt(0,interrupt1, RISING); //pino 2
//  attachInterrupt(1,interrupt2, RISING); //pino 3
  
}

void loop() {
  receberValor();
  funPrint();
}

//x212 y231 z321
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
    for(int x=0;x<14;x++) //limpa o texto e o buffer seriais
      buf[x]='\0'; 
  }
}

void dividirString(char* dado){
  Serial.print("Dado recebido: ");
  Serial.print(dado);
  char* parameter;
  parameter = strtok(dado," ,");
  while(parameter !=NULL){        //Se NULL indica que chegou ao final da string
    setEixo(parameter);
    parameter=strtok(NULL," ,");  //NULL indica pra continuar de onde parou
  }
  //limpa o texto e o buffer seriais
  //for(int x=0;x<14;x++)
  //  buf[x]='\0';
}

void setEixo(char* dado){
  if(dado[0]=='x'){
    x = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco1.gotoPoint(x, braco1.getY(),braco1.getZ());
  }
  else if(dado[0]=='y'){ //1 = ativa/desativa INTCN
    y = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco1.gotoPoint(braco1.getX(),y,braco1.getZ());
  }
  else if(dado[0]=='z'){
    z = strtol(dado+1,NULL,10);  //Parametros: recebe String para converter, 
                                              //ponteiro caractere depois do inteiro, 
                                              //a base
    braco1.gotoPoint(braco1.getX(), braco1.getY(),z);
  }
  else if(dado[0]=='a'){
    braco1.openGripper();
  }
  else if(dado[0]=='f'){
    braco1.closeGripper();
    Serial.println("POOOORRAAAA");
  }
  else if(dado[0]=='b'){
    braco2.openGripper();
  }
  else if(dado[0]=='c'){
    braco2.closeGripper();
  }
}

void interrupt1(){
  Serial.println("Interrupcao 1");
  digitalWrite(pinISR,LOW); //prepara a interrupcao para quando terminar a tarefa
  //moveParaPonto1();
}

void interrupt2(){
  Serial.println("Interrupcao 2");
  //moveParaPonto2();
}

void fun(){
    //inicio
  braco1.openGripper();
  braco1.gotoPoint(0,180,100);
  delay(500);
  //pegar objeto
  braco1.gotoPoint(0,200,60);
  braco1.closeGripper();
  delay(500);

  //voltar ao inicio
  braco1.gotoPoint(0,180,100);
  delay(500);
  braco1.gotoPoint(100,130,80);
  delay(500);
  braco1.openGripper();
  delay(500);
  braco1.openGripper();
  braco1.gotoPoint(0,180,100);
  delay(500);
}

void moveParaPonto1(){
  pegarObjeto();
  
  
  posRepouso1();
}

void moveParaPonto2(){
  pegarObjeto();
  
  
  posRepouso1(); //finaliza
  liberarEsteira();
}

void liberarEsteira(){
  digitalWrite(pinISR, HIGH);
}

void posRepouso1(){
  braco1.gotoPoint(0,140,60);
  braco1.closeGripper();
}
void posRepouso2(){
  braco2.gotoPoint(0,140,60);
  braco2.closeGripper();
}

void posEsteira(){ //posiciona o braço acima da esteira
    braco1.gotoPoint(0,200,60);
}

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
  Serial.println(braco2.getZ());
}

void pegarObjeto(){
  posEsteira();
  braco1.openGripper();
  
  braco1.gotoPoint(0,200,60);  // baixa ate o objeto
  braco1.closeGripper();       // agarra o objeto
  posEsteira();             // levanta o objeto
}

