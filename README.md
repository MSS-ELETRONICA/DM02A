# DM02A
Biblioteca para o dimmer DM02A
Link do produto (https://www.msseletronica.com/detalhes/modulo-dimmer-2-canais-bivolt-para-arduino-esp8266-esp32-stm-dm02a_pid1349.html)

Link do manual (https://www.msseletronica.com/arquivos/DM02A.pdf)

O Módulo dimmer 2 canais bivolt - DM02A é ideal para automatizar lâmpadas e ventiladores usando um microcontrolador como Arduino, PIC, ESP8266, ESP32, STM e etc.

O produto possui 70 níveis de dimerização para cada canal. Todo o controle de sincronismo com a rede e dimerização é feito pelo módulo. Para ajustar o nível desejado, basta selecionar o canal a ser controlado (borne CH) e enviar um pulso entre 500 e 35000uS (cada 500uS corresponde à 1 nível) no borne SIG.

Para usar mais de um módulo em conjunto, basta usar o borne de controle EN que habilita ou desabilita o módulo desejado. Sendo assim, para cada módulo extra, será necessário apenas mais 1 pino de I/O do seu microcontrolador.

Para o controle mais básico, são necessários apenas 2 pinos de I/O do microcontrolador.

O módulo conta com 4 botões para o controle manual dos níveis, sendo 2 botões para cada canal. Além disso é possível adicionar 4 botões externos que ficam em paralelo com os botões da placa (Usar botão pulsante).

O módulo possui 3 LEDs para indicação, sendo LED ON, LED canal 1 e LED canal 2.

## Comandos disponíveis na biblioteca ##

//Cria um novo objeto do tipo DM02A

DM02A dimmer(SIG, CH, EN);  //SIG, CH, EN - Se não usar o EN, deixar sem informar

dimmer.EnviaNivel(nivel, canal);  //Define o nível no canal selecionado

dimmer.feedback(canal); //Retorna o nível do canal selecionado
