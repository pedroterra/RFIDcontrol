
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>


#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10
#define RELAY1  7// Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String DataString = "";
String PedroTerra = "851662507";
String DaniCard = "3771470";
String AlexCard = "998851";

boolean OpenDoor = false;

void setup() {
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");

  Serial.begin(9600);         // Initialize serial communications with the PC
  while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();         // Init MFRC522 card
  Serial.println(F("RFID Control"));
  Serial.println("by Pedro Terra V1.0");
  Serial.println("");

}

void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(DataString);

  if (DataString == PedroTerra) {
    Serial.println("Usuário: Pedro Terra");
    OpenDoor = true;

  } else if (DataString == DaniCard) {
    Serial.println("Usuário: Daniel Sene");
    OpenDoor = true;

  } else if (DataString == AlexCard) {
    Serial.println("Usuário: Alex Uzeli");
    OpenDoor = true;

  } else {
    Serial.println("Cartão desconhecido");
    Serial.println("Acesso Negado!");
    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    Serial.println("");
  }

  if (OpenDoor == true) {
    Serial.println("Acesso Liberado");
    Serial.println("Solenide on");
    digitalWrite(6, HIGH);
    digitalWrite(RELAY1, 0);

    tmElements_t tm;
    if (RTC.read(tm)) {
      Serial.print("Time: ");
      print2digits(tm.Hour);
      Serial.write(':');
      print2digits(tm.Minute);
      Serial.write(':');
      print2digits(tm.Second);
      Serial.print(" Date: ");
      Serial.print(tm.Day);
      Serial.write('/');
      Serial.print(tm.Month);
      Serial.write('/');
      Serial.print(tmYearToCalendar(tm.Year));
      Serial.println();
    } else {
      if (RTC.chipPresent()) {
        Serial.println("The DS1307 is stopped.  Please run the SetTime");
        Serial.println("example to initialize the time and begin running.");
        Serial.println();
      } else {
        Serial.println("DS1307 read error!  Please check the circuitry.");
        Serial.println();
      }
    }
     delay(1000);
    digitalWrite(RELAY1, 1);
    digitalWrite(6, LOW);

    OpenDoor = false;
    Serial.println("");

    DataString = "";

    delay(2000);

  }
}



/*
   Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < 4; i++) {

    DataString += String(buffer[i], DEC);
  }
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

