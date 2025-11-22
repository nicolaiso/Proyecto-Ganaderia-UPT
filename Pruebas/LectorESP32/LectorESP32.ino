#include <SPI.h>
#include <MFRC522.h>


#define SS_PIN    5    
#define RST_PIN   21  


MFRC522 mfrc522(SS_PIN, RST_PIN);


String inputData = ""; 

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("Inicializando RC522...");

  
  SPI.begin(18, 19, 23, SS_PIN);

  mfrc522.PCD_Init();
  Serial.println("RFID RC522 listo");
}

void loop() {
  
  Serial.println("\nMENU:");
  Serial.println("1. Escribir en tarjeta");
  Serial.println("2. Leer tarjeta");
  Serial.print("Seleccione opcion: ");

  
  while (Serial.available() == 0) {
    
  }
  
  char opcion = Serial.read();
  
  while (Serial.available()) { Serial.read(); }

  if (opcion == '1') {
    escribirDatos();
  } 
  else if (opcion == '2') {
    leerDatos();
  } 
  else {
    Serial.println("Opcion invalida, intente de nuevo.");
    delay(2000);
  }
}


void escribirDatos() {
  Serial.println("\nIngrese informacion (max 15 caracteres, finalice con '#' ):");
  inputData = "";
  bool finalizado = false;

 
  while (!finalizado) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '#') {
        finalizado = true;
      } else {
        if (inputData.length() < 15) {
          inputData += c;
        }
      }
    }
  }
  
  Serial.print("Datos a escribir: ");
  Serial.println(inputData);
  Serial.println("Acerca la tarjeta al lector para escribir...");

  
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
   
  }
  
  byte block = 1; 
  byte buffer[16];
  memset(buffer, 0, 16);

  
  for (int i = 0; i < inputData.length(); i++) {
    buffer[i] = inputData.charAt(i);
  }
  
  
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
 
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Error en autenticacion: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA();
    return;
  }
  
  
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Error escribiendo: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.println("Datos escritos con exito!");
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
 
  delay(5000);
}


void leerDatos() {
  Serial.println("\nAcerca la tarjeta al lector para leer...");
  
  
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
   
  }
  
  byte block = 1; 
  byte buffer[18]; 
  byte size = sizeof(buffer);
  
 
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Error en autenticacion: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA();
    return;
  }
  
  status = mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Error leyendo: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.print("Datos leidos: ");
    for (byte i = 0; i < 16; i++) {
      Serial.print((char)buffer[i]);
    }
    Serial.println();
  }
  
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(10000);
}
