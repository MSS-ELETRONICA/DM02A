/*
EXEMPLO: ACIONAMENTO DE APENAS UM MÓDULO DE 2 CANAIS COM ATUALIZAÇÃO DO NÍVEL ATUAL PELO FEEDBACK
USO:
  * ABRA O MONITOR SERIAL E USE OS COMANDOS:
    a : Envia um comando para mudar 1 nível do dimmer aumentando
    d : Envia um comando para mudar 1 nível do dimmer diminuindo
    A : Envia um comando para mudar 5 níveis do dimmer aumentando
    D : Envia um comando para mudar 5 níveis do dimmer diminuindo
    
    1 : Seleciona o envio de comandos para o canal 1
    2 : Seleciona o envio de comandos para o canal 2
    
    n : Solicita o nível atual do canal selecionado (resposta de 0 à 70)
    
LIGAÇÕES DO MÓDULO AO ARDUINO:
  GND: CONECTAR AO GND DO ARDUINO
  VCC: CONECTAR AO PINO DE SAÍDA DE 5V OU 3.3V
  SIG: CONECTAR AO PINO 12 DO ARDUINO
  CH: CONECTAR AO PINO 11 DO ARDUINO
  EN: SE FOR USAR APENAS UM MÓDULO, DEIXAR SEM CONEXÃO
  B1+, B1-, B2+, B2-: SÓ USAR SE DESEJAR CONECTAR INTERRUPTORES EXTERNOS E NESSE CASO DEVEM CHAVEAR O GND
  
LIGAÇÕES DO MÓDULO À REDE/CARGA:

  FASE: LIGAR A FASE 127V OU 220V
  NEUTRO: LIGAR O NEUTRO DA REDE OU FASE 2 (TODOS OS BORNES 'NEUTRO' ESTÃO INTERLIGADOS, ENTÃO USE UM PARA ENTRAR COM O NEUTRO E OS DEMAIS PARA SAIR PARA A CARGA)
  SAÍDA 2: SAÍDA 2 DO MÓDULO
  SAÍDA 1: SAÍDA 1 DO MÓDULO  
  
  
DURAÇÃO DOS PULSOS NO PINO 'SIG':

  Duração entre 500uS e 35000uS: Define o nível atual, sendo cada 500uS 1 nível, totalizando 70 níveis
  Menos de 500uS: Nível 0, saída desligada
  Entre 40000uS e 50000uS: Solicita o nível atual do canal em selecionado. Após enviar o pulso, coloque o pino que enviou o pulso como entrada. 100uS após receber o comando, o dimmer
	colocará o pino SIG em nível 0, passados mais 100uS o pino SIG enviará um pulso com duração correspondente ao nível atual do canal, sendo: 500uS para o nível 1, 35000uS para o nível 70 e 100uS para o nível 0.
*/

#include <DM02A.h>  //Inclui a biblioteca para comunicar com o dimmer DM02A

//Cria um novo objeto
DM02A dimmer(12, 11);//SIG, CH, EN - Se não usar o EN, deixar sem informar

/*Comandos disponíveis: 
  dimmer.EnviaNivel(nivel, canal);  //Define o nível no canal selecionado
  dimmer.feedback(canal); //Retorna o nível do canal selecionado
*/

char nivel_1 = 0;   //Armazena internamente um nível, nesse exemplo esse nível será atualizado de tempo em tempo com a função feedback
char nivel_2 = 0;

char nivel_1_tmp = 0;   //Armazena o nível temporariamente para comprar com o nível previamente ajustado e saber se mudou
char nivel_2_tmp = 0;

boolean canal = 0;	//Armazena internamente qual o canal selecionado
char comando = 0;	//Armazena o comando recebido pela porta serial (Enviado pelo monitor serial)

boolean novo_comando = 0;	//Indica se foi recebido um novo comando pela porta serial

unsigned long last_tempo = 0;//Armazena o momento da última solicitação automática de feedback, a função time() reinicia em 50 dias com o Arduino constantemente ligado

void setup(){
  
  //Força os dois canais iniciarem desligados
  dimmer.EnviaNivel(0,0);
  dimmer.EnviaNivel(0,1);
  
  Serial.begin(9600);//Inicializa a comunicação serial em 9600
  
  //Mostra a lista de comandos no monitor serial (Abra o monitor serial)
  Serial.println("Lista de comandos:");
  Serial.println("a : Envia um comando para mudar 1 nivel do dimmer aumentando");
  Serial.println("d : Envia um comando para mudar 1 nivel do dimmer diminuindo");
  Serial.println("A : Envia um comando para mudar 5 niveis do dimmer aumentando");
  Serial.println("D : Envia um comando para mudar 5 niveis do dimmer diminuindo");
  Serial.println("1 : Seleciona o envio de comandos para o canal 1");
  Serial.println("2 : Seleciona o envio de comandos para o canal 2");
  Serial.println("n : Solicita o nivel atual do canal selecionado (resposta de 0 a 70)");
}

void loop(){

  //A cada 500mS verifica se algum dos níveis do dimmer foi alterado pelas chaves da placa ou externas
  if(millis() > (last_tempo+2000)){//Verifica se já se passou 500ms, se sim, verifica se houve atualização do nível pelos botões do módulo dimmer ou por chaves externas
    last_tempo = millis();//Atualiza o tempo registrado para o tempo atual
    nivel_1_tmp = dimmer.feedback(0);//Solicita o nivel atual do canal 1
    if(nivel_1 != nivel_1_tmp){
      nivel_1_tmp = dimmer.feedback(0);
      if(nivel_1 != nivel_1_tmp){
        nivel_1 = nivel_1_tmp;//Atualiza o nível
        Serial.print("Feedback canal 1: ");
        Serial.println(nivel_1_tmp, DEC);
      }
    }
    nivel_2_tmp = dimmer.feedback(1);//Solicita o nível atual do canal 2
    if(nivel_2 != nivel_2_tmp){
      nivel_2_tmp = dimmer.feedback(1);
      if(nivel_2 != nivel_2_tmp){
        nivel_2 = nivel_2_tmp;//Atualiza o nível
        Serial.print("Feedback canal 2: ");
        Serial.println(nivel_2_tmp, DEC);
      }
    }
  }

  if (Serial.available()) {//Verificando se tem dado disponível na porta serial
    comando = (char)Serial.read();//Lê o dado recebido pela porta serial, esse dado irá representar um comando
    novo_comando = 1;//Indica que recebeu um novo comando
  }
  
  if(novo_comando){//Confere se recebeu um novo comando, essa variável recebe o valor 1 (HIGH) quando chega um novo dado pela porta serial, isso dentro da interrupção da porta serial
    novo_comando = 0;//Limpa a variável que indica que recebeu um novo comando para não executar o mesmo comando mais de uma vez
    // Serial.println(comando);
	if(canal){//Se estiver em 1 é o canal 2
      nivel_2 = dimmer.feedback(1);
    }else{//Se estiver em 0 é o canal 1
		  nivel_1 = dimmer.feedback(0);
    }
	  
      switch(comando){//Verifica qual é o comando, só executa comandos válidos
        case 'a':  //Aumenta o nível de 1 em 1 unidade
          if(canal == 0){//Em 0 é o canal 1,
            if(nivel_1 < 70){ //Máximo 70 níveis
              nivel_1 = nivel_1 + 1;//Incrementa 1 unidade

              dimmer.EnviaNivel(nivel_1,0);
              
              Serial.println(nivel_1, DEC);  //Só para demonstração, imprime o nível atual no monitor serial
            }
          }else{//Em 1 é o canal 2
            if(nivel_2 < 70){
              nivel_2 = nivel_2 + 1;//Incrementa 1 unidade
              
              dimmer.EnviaNivel(nivel_2,1);
              
              Serial.println(nivel_2, DEC);
            }
          }
        break;
        case 'd':
          if(canal == 0){
            if(nivel_1 > 0){
              nivel_1 = nivel_1 - 1;//Decrementa 1 unidade
              
              dimmer.EnviaNivel(nivel_1,0);
              
              Serial.println(nivel_1, DEC);
            }
          }else{
            if(nivel_2 > 0){
              nivel_2 = nivel_2 - 1;
              
              dimmer.EnviaNivel(nivel_2,1);
              
              Serial.println(nivel_2, DEC);
            }
          }
        break;
        
        case 'A':
           if(canal == 0){
             if(nivel_1 < 70){
                nivel_1 = nivel_1 + 5;  //Incrementa de 5 em 5
                if(nivel_1 > 70){nivel_1 = 70;}//Como pode incrementar de 1 em 1 ou de 5 em 5, poderá ocorrer caso de passar de 70, então limita em 70
                
                dimmer.EnviaNivel(nivel_1,0);
                
                Serial.println(nivel_1, DEC);
              }
           }else{
             if(nivel_2 < 70){
                nivel_2 = nivel_2 + 5;
                if(nivel_2 > 70){nivel_2 = 70;}//Como pode incrementar de 1 em 1 ou de 5 em 5, poderá ocorrer caso de passar de 70, então limita em 70
                
                dimmer.EnviaNivel(nivel_2,1);
                
                Serial.println(nivel_2, DEC);
              }
           }
        break;
        case 'D':
          if(canal == 0){
            if(nivel_1 > 5){
              nivel_1 = nivel_1 - 5;
              
              dimmer.EnviaNivel(nivel_1,0);
              
              Serial.println(nivel_1, DEC);
            }else{//Sendo menor do que 5, envia o nível 0
              nivel_1 = 0;
              
              dimmer.EnviaNivel(nivel_1,0);              
              
              Serial.println(nivel_1, DEC);
            }
          }else{
            if(nivel_2 > 5){
              nivel_2 = nivel_2 - 5;//Incrementa 1 unidade
              
              dimmer.EnviaNivel(nivel_2,1);
              
              Serial.println(nivel_2, DEC);
            }else{//Sendo menor do que 5, envia o nível 0
              nivel_2 = 0;
              
              dimmer.EnviaNivel(nivel_2,1);
              
              Serial.println(nivel_2, DEC);
            }
          }
        break;
        
        case '1':
          canal = 0;  //Define qual canal está sendo controlado no momento, nesse caso, canal 1
        break;
        case '2':
          canal = 1;  //Define qual canal está sendo controlado no momento, nesse caso, canal 2
        break;

        case 'n':  //Rotina para solicitar o feedback do dimmer
          if(canal){//Se estiver em 1 é o canal 2
            Serial.print("Feedback canal 2: ");
            Serial.println(dimmer.feedback(1), DEC);
          }else{//Se estiver em 0 é o canal 1
            Serial.print("Feedback canal 1: ");
            Serial.println(dimmer.feedback(0), DEC);
          }
        break;        
    }
  }
}