
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(18, 19);

#else
#define mySerial Serial1
#endif

const int RELAY_PIN = A15;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial espSerial(0, 1);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  Serial.begin(115200);
  while (!Serial);  
  delay(100);
  Serial.println("\n\nAdafruit senzor test");

  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Senzorul de amprentă a fost detectat!");
  } else {
    Serial.println("Nu am detectat niciun senzor de amprentă! :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();

  lcd.begin(16, 2);
  // Afișeaza un mesaj prin LCD
  lcd.print("Salut!");
  pinMode(RELAY_PIN, OUTPUT);
}

void loop()                     // ruleaza la nesfârșit
{
  getFingerprintID();
  delay(1000);           
}

void lockDoor()
{
  digitalWrite(RELAY_PIN, LOW);   //setăm modulul releu pe low
  lcd.clear();  //curățăm LCD
  lcd.print("Închis!"); //Afișăm că este închis
}

void unlockDoor(int id)
{
  digitalWrite(RELAY_PIN, HIGH);  //setăm modulul releu pe high
  lcd.clear();  //curățăm LCD
  lcd.print("Deschis!"); //Afișăm că este deschis
  Serial.println(id); //Afișăm în serial monitor id-ul asociat persoanei care a introdus amprenta
}

void lcdOpen()
{
  lcd.clear();  //curățăm LCD
  lcd.print("5...");  //Îi spunem LCD-ului să afișeze „5...”
  delay(1000);
  lcd.clear();  //curățăm LCD
  lcd.print("4...");  //Îi spunem LCD-ului să afișeze „4...”
  delay(1000);
  lcd.clear();  //curățăm LCD
  lcd.print("3...");  //Îi spunem LCD-ului să afișeze „3...”
  delay(1000);
  lcd.clear();  //curățăm LCD
  lcd.print("2...");  //Îi spunem LCD-ului să afișeze „2...”
  delay(1000);
  lcd.clear();  //curățăm LCD
  lcd.print("1...");  //Îi spunem LCD-ului să afișeze „1...”
  delay(1000);
  lcd.clear();  //curățăm LCD
} 

void lcdWrongFingerprintId()
{
  lcd.clear();  //curățăm LCD
  lcd.print("Amprentă greșită!"); //LCD afișează „Amprentă greșită!” în cazul în care amprenta nu a fost înregistrată
  delay(3000);
  lcd.clear();  //curățăm LCD
}

uint8_t getFingerprintID() { 
  uint8_t p = finger.getImage();  //Stocăm imaginea preluata din amprentă și o stocăm in variabila p
   switch (p) {
     case FINGERPRINT_OK:
      break;
     case FINGERPRINT_NOFINGER:
       return p;
  }

  p = finger.image2Tz();  //convertește obiectul de tip finger într-un format specific folosint funcția image2tz()
  switch (p) {
    case FINGERPRINT_OK:  //acesta este cazul în care conversia s-a realizat cu succes.
      break;
  }

  p = finger.fingerSearch();  //se caută amprenta și se stochează în variabila p
  if (p == FINGERPRINT_OK) {    //dacă amprenta este gasită. nu se întamplă nimic.
  } else if (p == FINGERPRINT_NOTFOUND) {   
    Serial.println(0);
    lcdWrongFingerprintId();    //dacă amprenta nu este gasită, se apelează funcția lcdWrongFingerprintId()
    return p;
  }

  if(finger.fingerID == 1)  //Verificam dacă ID-ul amprentei este 1
  {
    unlockDoor(1);  //Daca ID-ul amprentei este 1, deschidem ușa persoanei cu ID-ul 1
  }
  else if(finger.fingerID == 2) //Verificam dacă ID-ul amprentei este 2
  {
    unlockDoor(2);  //Daca ID-ul amprentei este 2, deschidem ușa persoanei cu ID-ul 2
  }
    lcdOpen();  //Pornim display-ul
    lockDoor(); //închidem ușa

  return finger.fingerID;   //returnăm ID-ul amprentei găsite
}


