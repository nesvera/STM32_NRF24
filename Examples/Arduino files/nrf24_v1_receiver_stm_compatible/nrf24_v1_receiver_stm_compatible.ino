/*
MIT License

Copyright (c) 2016 D. Nesvera

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*
Conexão com o rádio

  * CE    -   PIN 9
  * CSN   -   PIN 10
  * SCK   -   PIN 13
  * MOSI  -   PIN 11
  * MISO  -   PIN 12
  * IRQ   -   NULL
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RADIO_CE     9
#define RADIO_CSN    10
#define RADIO_SCK    13
#define RADIO_MOSI   11
#define RADIO_MISO   12

const uint64_t pipeIn = 0xE0E1E2E3E4E5E6E7;

RF24 radio( RADIO_CE, RADIO_CSN);                   // Seta os pinos do radio

unsigned long lastReceiv;
int cont = 0;

// The sizeof this struct should not exceed 32 bytes
// Estrutura do pacote que irá ser enviado pelo rádio
// alterar
struct EstruturaPacote{               //16 bytes serão enviados
   uint8_t xAxis;
   uint8_t yAxis;
   uint8_t zAxis;
   uint8_t text[28];
} pacote;

void setup() {
  Serial.begin(9600);
  
  // inicia configuração do rádio
  radio.begin();
  // Canais para escolha 80 - 90 - 100 - 110 - 120
  radio.setChannel(80);                                // escolher um canal - freq = 2400 MHz + channel * 1 MHz 
  radio.setPayloadSize(sizeof(EstruturaPacote));        // controldata é uma stringCallbackFunction - definir de 0 a 32 bytes
  radio.setAutoAck(false);                              // desabilita a transmissão de acknolodgment
  radio.setDataRate(RF24_1MBPS);                    
  radio.setCRCLength(RF24_CRC_16);
  
  radio.openReadingPipe(1,pipeIn);
  delay(1);

  radio.startListening();
  delay(750);

}

void loop()
{
    if( millis() - lastReceiv > 2000  ){                   // sinal perdido? reseta dados e tenta parear
      if(cont<2){
        Serial.println("Sem sinal!");
        cont++; 
        delay(100);
      } 
    }  
    
    cont = 0;

    if( radio.available() ){
      bool done = false;

      while( !done ){
        done = radio.read(&pacote, sizeof(EstruturaPacote));
        lastReceiv = millis();                               
      }
      
      Serial.println(pacote.xAxis);
      Serial.println(pacote.yAxis);
      Serial.println(pacote.zAxis);
      
      int i = 0;
      for( i = 0 ; i < sizeof(pacote.text) ; i++ ){
        Serial.print((char)pacote.text[i]);
      }
      
      Serial.print("\n\n");  
    }   
}


