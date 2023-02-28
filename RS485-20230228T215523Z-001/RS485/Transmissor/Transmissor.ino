//-------Bibliotecas_utilizadas------------//

#include <SoftwareSerial.h>
#include <util/crc16.h>

//-------------------------------------------
//-------Mapeamento_de_Hardware------------//

//Pinos de comunicacao serial do modulo RS485
#define Pino_RS485_RX    10
#define Pino_RS485_TX    11

//Pino de controle transmissao/recepcao
#define SSerialTxControl 3

//Define led 13 para mostrar atividade na comunicacao
#define Pin13LED         13

//------------------------------------------
//----------------Configuracoes----------//

SoftwareSerial RS485Serial(Pino_RS485_RX, Pino_RS485_TX); //Cria a serial por sofware para conexao com modulo RS485

#define RS485Transmit    HIGH
#define RS485Receive     LOW

byte SD3 =  B10100010;    //Inicio de Frame = 162 (decimal).
byte DA =   B00000010;    //Endereço do Receptor = 2 (decimal).
byte SA =   B00000001;    //Endereço do Transmissor = 1 (decimal).
byte FC =   B00001010;    //Valor de FC (Frame check) para Diagnose dos Sinais de dados.
byte Dado = B00000001;    //Dado = 1 (decimal).
byte FCS =  B00000000;    //inicializado em 0, antes da logica do CHECKSUM.
byte CRC =  B00000000;    //inicializado em 0, antes da logica do CRC.
byte ED =   B00010110;    //ED = 22 (decimal) para finalizar o Pacote.

byte data[] = { SD3, DA, SA, FC, Dado, FCS, CRC, ED };

//------------------------------------------
//--------Prototipo de Funcoes---------//

void Func_Checksum()
{
  data[5] = 0;
  for (int i = 0; i < sizeof data / sizeof data[0]; i++) {
    if (i != 5 && i != 6) { //se estiver na posição diferente de FCS (Checksum) e CRC.
      data[5] += data[i];   //soma os valores.
    }
    //Serial.println(data[5]);
  }
  data[5] = ~data[5]; // Complemento de 1 do CHECKSUM realizado
}

void Func_CRC()
{
  uint8_t crc = 0, i;
  for (i = 0; i < sizeof data / sizeof data[0]; i++)
    if (i != 6) {
      crc = _crc_ibutton_update(crc, data[i]); // a funcao _crc_ibutton_update usa o polinomio x^8 + x^5 + x^4 + 1
    }
  data[6] = crc;  // Precisa ser 0 para que o dado esteja coerente
                  // ou o valor do resto para enviar em conjunto com a Palavra de dados
}

//------------------------------------------
//

void setup()
{
  Serial.begin(9600); //Inicializa a serial padrão do Arduino
  Serial.println("Modulo Transmissor");
  Serial.println("Pressione o botao para enviar os dados...");

  pinMode(Pin13LED, OUTPUT);
  pinMode(SSerialTxControl, OUTPUT);

  RS485Serial.begin(480); //Inicializa a serial do modulo RS485

  pinMode(A0, INPUT_PULLUP); //Seta o pino A0 como entrada e habilita o pull up

  //  Serial.println(data[6]);
  //  Serial.println(_crc_ibutton_update(0, SD3));
  //  Serial.println(data[5]);
  //  delay(1000);
}

void loop()
{
    if (Serial.available() > 0) //Se algo for recebido pela serial
    {
      Dado = Serial.read();
      data[4] = Dado;
      Func_Checksum();
      Func_CRC();
      //Serial.println("aqui");
      
    }
//      delay(5000);
//      Serial.write(data[4]);
    
    int  valor = digitalRead(A0); //Verifica se o botao foi pressionado
    if (valor == 0)
    {
      Serial.println("Botao pressionado. Enviando dados!");
  
      //Habilita o modulo para transmissao
      digitalWrite(SSerialTxControl, RS485Transmit);

      digitalWrite(Pin13LED, HIGH); //Liga o led 13 para mostrar que vai transmitir
      delay(10);
      //Envia a string
      for(int i = 0; i < 8; i++){
        RS485Serial.write(data[i]);
        delay(1000);
      }
      
      digitalWrite(Pin13LED, LOW);
      
      digitalWrite(SSerialTxControl, RS485Receive); //Desabilita o modulo para transmissao
      while (digitalRead(A0) == 0)
      {
        delay(50);
      }
    }
}
