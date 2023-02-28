import processing.serial.*; //Importa a biblioteca para abrir uma comunicação Serial
Serial myPort, myPort2; //Instância a biblioteca para a comuniação Serial
import controlP5.*;

PImage fundo;     //objeto de imagem para Background.

// Para textbox-----//
String text, text2;
ControlP5 cp5;
Textarea myTextarea;
//---------------------
       
PImage botaoOn; //Botão em estado pressionado (Ligado)
PImage botaoOff; //Botão em estado pressionado (desligado)
PImage botaoNot; //Botão Desabilitado
//=======================

int valor_recebido; //Cria uma variável para armazenas o valor recebido pela serial.
int valor_recebido2;
void setup()
{
  PFont font = createFont("GFSArtemisia-Bold",20); //Cria uma fonte font para trabalhar com texto.
  
  String portName = Serial.list()[0];   //Lista as portas COM (Serial) encontradas
                                        //Pega a primeira porta (Posição 0 em "Serial.list()[0]" e
                                        //armazena na variável portName.
  String portName2 = Serial.list()[33]; //Lista as portas COM (Serial) encontradas
                                        //Pega a primeira porta (Posição 0 em "Serial.list()[1]" e
                                        //armazena na variável portName.
  myPort2 = new Serial(this, portName2, 9600); //Abre uma comunicação Serial com baud rate de 9600
  myPort = new Serial(this, portName, 9600); //Abre uma comunicação Serial com baud rate de 9600
  
  size(1000, 600); //Define o tamanho da tela

  //Carrega imagem de fundo.
  fundo = loadImage("imagens/background.png");
  //================================================================
  
  cp5 = new ControlP5(this); 
  cp5.addTextfield("Dado").setPosition(100,230).setSize(200,40).setFont(font);
  cp5.addButton("Enviar").setPosition(100, 300).setSize(200, 40).setFont(font);
  myTextarea = cp5.addTextarea("txt")
              .setPosition(550,150)
              .setSize(300,400)
              .setFont(font)
              .setLineHeight(14)
              .setColor(color(128))
              .setColorBackground(color(255,255,255))
              .setColorForeground(color(25,25,112));
              ;
  myTextarea.setLineHeight(25); // configura o espaço entre linhas
  
}

void draw()
{
  background(fundo); //Atualiza a imagem de fundo (backgroud) da interface
  //println(text);
  
  //if (myPort.available() > 0) //Se algo for recebido pela serial
  //{
  //  valor_recebido = myPort.read(); //Armazena o que foi lido dentro da variável valor recebido
  //  //myTextarea.append(String.valueOf(valor_recebido));
  //  println(String.valueOf(valor_recebido));
  //}
  
  if (myPort2.available() > 0) //Se algo for recebido pela serial
  {
    text2 = myPort2.readStringUntil('\n');
    //valor_recebido2 = myPort2.read(); //Armazena o que foi lido dentro da variável valor recebido
    myTextarea.append(text2);
    println(text2);
    //println(String.valueOf(valor_recebido2));
  }
  

}

void Enviar() { //  Funcao chamada quando o botao Enviar é pressiondado  
  println();    //  deve ter o mesmo nome do botão.
  print("this is the text you typed :");
  text = cp5.get(Textfield.class, "Dado").getText();
  println(Integer.parseInt(text));
  myPort.write((byte)Integer.parseInt(text));
  text += '\n';
  myTextarea.append("Dado enviado: ");
  myTextarea.append(text);
  print(text);
}
