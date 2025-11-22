
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN  5  
#define RST_PIN 4

#define BTN_PIN 15


int lcdColumns = 20;
int lcdRows = 4;


int readsuccess;
char str[32] = "";
String UID_Result = "--------";



LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  


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
 

  pinMode(BTN_PIN, INPUT_PULLUP);
  

  lcd.init();

  lcd.backlight();


  SPI.begin();      

  mfrc522.PCD_Init(); 

  delay(1000);

  lcd.clear();
}

void loop(){

  
  lcd.setCursor(0,0);
  lcd.print("Introducir chip");
  lcd.setCursor(0,1);
  lcd.print("BTN State : ");
  lcd.print(digitalRead(BTN_PIN));

  readsuccess = getUID();
 
  if(readsuccess){
    lcd.setCursor(6, 2);
    lcd.print("              ");
    delay(500);
  }
  delay(10);
}
