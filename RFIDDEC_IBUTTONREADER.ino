
#include <SPI.h>
#include <MFRC522.h>
#include <OneWire.h>
OneWire  leitortag(2);    // 2 e' o pino do Arduino ligado ao leitor 

#define RST_PIN         9           
#define SS_PIN          10          

MFRC522 mfrc522(SS_PIN, RST_PIN);   
uint8_t readCard[8];
unsigned long card;
int buzzer = 4;

uint8_t x = 0;

//*************************************************************************************//
void setup() {
  Serial.begin(9600);                                           
  SPI.begin();                                                  
  mfrc522.PCD_Init();                                              
  pinMode(buzzer,OUTPUT);
}

byte addr[8];

//*************************************************************************************//
void loop() {

  if(consultaTag()){
      
      for(byte i=4;i>0;i--) {
           String digitos=String(addr[i], HEX);
           if (digitos.length()==1) digitos="0" + digitos;
           Serial.print(digitos);           
      }
      Serial.println();
      unsigned long tagdecimal=(unsigned long)addr[1];
      tagdecimal=tagdecimal|((unsigned long)addr[2]) << 8;
      tagdecimal=tagdecimal|((unsigned long)addr[3]) << 16;
      tagdecimal=tagdecimal|((unsigned long)addr[4]) << 24;
      
      Serial.println(tagdecimal);
      Serial.println();
  }
  
  
  
  
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //algumas variáveis que precisamos
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Procura novos cartões
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  digitalWrite(buzzer,HIGH);
  

  //-------------------------------------------
String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
     
  }
  Serial.println();
  
  conteudo.toUpperCase();
  

x = mfrc522.uid.size;
card = (unsigned long)mfrc522.uid.uidByte[0] << 24;
card += (unsigned long)mfrc522.uid.uidByte[1] << 16;
card += (unsigned long)mfrc522.uid.uidByte[2] << 8;
card += (unsigned long)mfrc522.uid.uidByte[3];

Serial.println(String(card));

digitalWrite(buzzer,LOW);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

boolean consultaTag() {
  boolean leituraok=false;
  if (leitortag.search(addr)) {
    if (OneWire::crc8(addr, 7) == addr[7]) { // testa a verificacao CRC
      if (addr[0]==1) {  // para tags iButton o primeiro byte sempre e' 1 
          leituraok=true;          
      } 
    }  
  }  
 leitortag.reset();
 return leituraok; 
}
//*************************************************************************************//
