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

// Bibliotecas para o rádio
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RADIO_CE     9
#define RADIO_CSN    10
#define RADIO_SCK    13
#define RADIO_MOSI   11
#define RADIO_MISO   12

#define ENABLE_SENSOR   5        // pino que alimenta/ativa o sensor
#define CAP_SENSOR_PIN  0        // pino analogico para leitura do sensor

unsigned long sent_at;
uint32_t value;
uint32_t cont;

// Endereço do rádio
const uint64_t pipeOut = 0xE0E1E2E3E4E5E6E7;

// Construtor
// Instancia o driver (chip enable pin, chip select pin)
RF24 radio( RADIO_CE, RADIO_CSN);

// The sizeof this struct should not exceed 32 bytes
// Estrutura do pacote que irá ser enviado pelo rádio
// alterar
struct EstruturaPacote{               //16 bytes serão enviados
   uint8_t xAxis;
   uint8_t yAxis;
   uint8_t zAxis;
   uint8_t text[28];
} txPackage;

void setup()
{
   // initialize serial communication
   // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
   // it's really up to you depending on your project)
   Serial.begin(9600);

   // Configuração do rádio
   radio.begin();
   // Canais para escolha 80 - 90 - 100 - 110 - 120
   radio.setChannel(80);                                                            // escolher um canal - freq = 2400 MHz + channel * 1 MHz 
   radio.setPayloadSize(sizeof(EstruturaPacote));                                   // controldata é uma stringCallbackFunction - definir de 0 a 32 bytes
   radio.setAutoAck(false);                                                         // desabilita a transmissão de acknolodgment
   radio.setDataRate(RF24_1MBPS);                                        
   radio.setCRCLength(RF24_CRC_16);

   radio.openWritingPipe(pipeOut);
   delay(1);

   value = 0;	
   cont = 0;			
	
   txPackage.xAxis = 0;
   txPackage.yAxis = 0;
   txPackage.zAxis = 0;
   memset( &txPackage.text, '\0', sizeof(txPackage.text)); // clean variables
   
   sent_at = 0;

   delay(500);
     
}

void loop()
{   
   // Send a message every second 
   if( millis() - sent_at > 1000 )
   {
     txPackage.xAxis = value;
     txPackage.yAxis = value+1;
     txPackage.zAxis = value+2;
	 
     value++;
		
      //sprintf((char *)txPackage.text, "Hello world!");
      snprintf((char *)txPackage.text, sizeof(txPackage.text), "S x: %d, y:%d, z:%d\n", txPackage.xAxis, txPackage.yAxis, txPackage.zAxis );
      
      radio.powerUp();
      delay(1);
      bool ok = radio.write( &txPackage, sizeof(EstruturaPacote) );
      
      if( !ok ){
         Serial.println("Falha no envio!");
         cont++;
      }else{
         cont--;
      }
		
			
      sent_at = millis();
    }
}

