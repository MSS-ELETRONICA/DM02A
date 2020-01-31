/*
  DM02A.cpp - Library for use with DM02A - MSS ELETRONICA
  MIT License
*/
 
#include "DM02A.h"
 
DM02A::DM02A(uint8_t pinSIG, uint8_t pinCH, uint8_t pinEN){
    pinMode(pinSIG, OUTPUT);
    pinMode(pinCH, OUTPUT);
    pinMode(pinEN, OUTPUT);
	digitalWrite(pinCH,LOW);	//Inicia selecionando o canal 1
	digitalWrite(pinSIG,HIGH);	//Previne o envio de pulso
	digitalWrite(pinEN,LOW);	//Inicia com a placa desativada
	_EN = 1;//Indica que está utilizando o pino EN
	_pinSIG = pinSIG;
	_pinCH = pinCH;
	_pinEN = pinEN;
}

DM02A::DM02A(uint8_t pinSIG, uint8_t pinCH){
    pinMode(pinSIG, OUTPUT);
    pinMode(pinCH, OUTPUT);	
	digitalWrite(pinCH,LOW);//Inicia selecionando o canal 1
	digitalWrite(pinSIG,HIGH);//Previne o envio de pulso
	_EN = 0;
	_pinSIG = pinSIG;
	_pinCH = pinCH;
}
 
void DM02A::EnviaNivel(uint8_t _nivel, uint8_t _canal){//Nível máximo 70
	if(_EN){
		digitalWrite(_pinEN,HIGH);
	}
	if(_canal >= 1){
		digitalWrite(_pinCH,HIGH);	//Se for 1 ou maior, seleciona o canal 2
	}else{
		digitalWrite(_pinCH,LOW); 	//Se for 0 (zero), selecoina o canal 1
	}
	pinMode(_pinSIG, OUTPUT);//Garantindo que o pino SIG é saída nesse momento
	if(_nivel > 0){
		if(_nivel > 70){//Se enviar um nível acima de 70, limita em 70 que é o máximo aceito pelo dimmer
			_nivel = 70;
		}
		digitalWrite(_pinSIG,LOW);//Coloca o pino de sinal em nível baixo durante o tempo que representa o nível desejado
		_cont = 0;
		while(_cont < _nivel){//Não tem como fazer 500us no delay, e o delayMicroseconds não tem como fazer 35ms, então faremos um loop
			delayMicroseconds(500);
			_cont = _cont + 1;
		}
        digitalWrite(_pinSIG,HIGH);
	}else{//Se for o nível 0 (zero), envia um pulso com um tempo menor que 500uS		
		digitalWrite(_pinSIG,LOW);
        delayMicroseconds(250);	//Enviando a metade do tempo máximo
        digitalWrite(_pinSIG,HIGH);
	}
	delayMicroseconds(100);	//Dá um tempo mínimo para o dimmer ler o comando
	if(_EN){
		digitalWrite(_pinEN,LOW);
	}
	delay(60);//Para não misturar os comandos
}

uint8_t DM02A::feedback(uint8_t _canal){//Solicita o nivel atual do canal selecionado
	if(_EN){
		digitalWrite(_pinEN,HIGH);
	}
	delayMicroseconds(100);
	if(_canal >= 1){
		digitalWrite(_pinCH,HIGH);	//Se for 1 ou maior, seleciona o canal 2
	}else{
		digitalWrite(_pinCH,LOW); 	//Se for 0 (zero), selecoina o canal 1
	}
	
	pinMode(_pinSIG, OUTPUT);//Garantindo que o pino SIG é saída nesse momento
	digitalWrite(_pinSIG,LOW);    //O pulso é em nível baixo
    delay(45);  //O pulso para solicitar o nível atual no dimmer deve entre 40 e 50mS, 45mS para dar uma margem de segurança
    digitalWrite(_pinSIG,HIGH);   //Volta para nível alto ao finalizar o pulso
    delayMicroseconds(100);    //Aguarda  100us que é o tempo que o dimmer aguarda para transformar o pino SIG em saída
          
    pinMode(_pinSIG,INPUT);       //Muda o pino de sinal do Arduino para entrada para poder ler o pulso enviado pelo dimmer informando o nível atual
    delayMicroseconds(100);    //Aguarda 100uS que é o tempo que o dimmer aguarda para enviar o feedback após transformar o pino SIG em saída
          
    _pulso = pulseIn(_pinSIG, LOW, 36000);//O pulso de feedback é em nível baixo, aguarda no máximo 36000uS lendo o pulso, se passar disso sai do 'pulseIn'
        
    pinMode(_pinSIG,OUTPUT);      //Após ler o pulso, volta o pino de sinal do Arduino para saída
    digitalWrite(_pinSIG,HIGH);   //Faz o pino de sinal do Arduino ficar em nível alto, já que o envio de sinal para o dimmer é nível baixo
    
	if(_EN){
		digitalWrite(_pinEN,LOW);
	}
	delay(60);	//Dá um tempo mínimo para o dimmer executar o comando
	_pulso = _pulso / 500;
	if(_pulso > 70){
		_pulso = 70;
	}
	return _pulso;
}