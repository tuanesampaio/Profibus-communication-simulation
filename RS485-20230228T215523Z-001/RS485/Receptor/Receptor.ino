//-------Bibliotecas_utilizadas------------//

#include <SoftwareSerial.h>
#include <util/crc16.h>

//---------------------------------------------
//------------Mapeamento_de_hardware-----------

#define Pino_RS485_RX    10 //Pinos de comunicacao serial do modulo RS485
#define Pino_RS485_TX    11

#define SSerialTxControl 3 //Pino de controle transmissao/recepcao

#define Pin13LED         13 //Define led 13 para mostrar atividade na comunicacao

//----------------------------------------
//---------------Configuracao------------//

#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(Pino_RS485_RX, Pino_RS485_TX); //Cria a serial por sofware para conexao com modulo RS485

int incomingByte = 0;     // variável para o dado recebido.
byte ED =   B00010110;    //ED para finalizar o Pacote.
byte data[8];             //Vetor para armazenar dados recebidos.
int i = 0;
char prefixo[8][5] = {"SD3:" , "DA: " , "SA: " , "FC: " , "DU: " , "FCS:" , "CRC:" , "ED: "};

//------------------------------------------
//------------------------------------------
//--------Prototipo de Funcoes---------//

bool Func_Checksum() //Retorna 1 caso erro 
{                    //Retorna 0 caso checksum correto
  byte soma = 0;
  for (int i = 0; i < sizeof data / sizeof data[0]; i++) {
    if (i != 6) { //se estiver na posição diferente do byte de CRC.
      soma += data[i];   //soma os valores.
    }
    //Serial.println(soma);
  }
  soma = ~soma; // Complemento de 1 do CHECKSUM realizado
  if(soma != 0){
    Serial.println("ERRO NO CHECKSUM");
    return true;
  }
  Serial.println("CHECKSUM CORRETO");
  return false;
}

bool Func_CRC() // Retorna 0 caso CRC correto
{               // Retorna 1 caso CRC incorreto
  uint8_t crc = 0, i;
  for (i = 0; i < sizeof data / sizeof data[0]; i++){
    if(i != 6){ // Passa pela posicao 6 que é a do CRC.
      crc = _crc_ibutton_update(crc, data[i]); // a funcao _crc_ibutton_update usa o polinomio x^8 + x^5 + x^4 + 1
    }
  }
  crc = _crc_ibutton_update(crc, data[6]); // Checa por ultimo com o resto que esta na posicao 6.
  if(crc == 0){
    Serial.println("CRC CORRETO");
    return false;
  }
  Serial.println("ERRO NO CRC");
  return true;
}

//------------------------------------------

void setup()
{
  //Inicializa a serial do Arduino
  Serial.begin(9600);
  Serial.println("Modulo Receptor");
  Serial.println("Aguardando dados...");

  pinMode(Pin13LED, OUTPUT);
  pinMode(SSerialTxControl, OUTPUT);

  digitalWrite(SSerialTxControl, RS485Receive); //Coloca o modulo RS485 em modo de recepcao
  
  RS485Serial.begin(480); //Inicializa a serial do modulo RS485
}

void loop()
{
  if (RS485Serial.available() > 0)
  {
      incomingByte = RS485Serial.read(); // lê do buffer o dado recebido:

      Serial.print(prefixo[i]); // responde com o dado recebido:
      Serial.println(incomingByte);
      data[i] = incomingByte;
      i++;
            
      if(incomingByte == ED){
        Serial.println("Pacote Recebido");
        Serial.println("Teste para deteccao de erros");
        Func_Checksum();
        Func_CRC();
        i = 0;
      }
  }
  
}
