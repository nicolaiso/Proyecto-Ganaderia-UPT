


#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN  5  
#define RST_PIN 4


int readsuccess;
char str[32] = "";
String UID_Result = "";


MFRC522 mfrc522(SS_PIN, RST_PIN);


int getUID() {  
  if(!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if(!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  
  byteArray_to_string(mfrc522.uid.uidByte, mfrc522.uid.size, str);
  UID_Result = str;
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  return 1;
}

void byteArray_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len*2] = '\0';
}

void setup(){


  Serial.begin(115200);
  Serial.println();
  delay(1000);

  SPI.begin();      
 
  mfrc522.PCD_Init(); 

  delay(1000);

  Serial.println();
  Serial.println("Porfavor colocar el chip Rfid");
}

void loop(){

  
  readsuccess = getUID();
 
  if(readsuccess){
    Serial.println();
    Serial.print("UID : ");
    Serial.println(UID_Result);
    delay(500);
  }
  delay(10);
}
