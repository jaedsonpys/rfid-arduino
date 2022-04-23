#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

#define LCD_PORT 0x27
#define SS_PIN 10
#define RST_PIN 9

#define REDLED_PIN 3
#define GREENLED_PIN 2

#define BUTTON_PIN 8
#define BUZZER_PIN 6

LiquidCrystal_I2C lcd(LCD_PORT, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

void startMessage(bool buzzerOn = true);
void printAllowed(String uid);
void printNotAllowed(String uid);
void registerCard();
void pushArray(String element);

String acceptedIDs[10] = {};
int lastIndex = 0;

bool registerMode = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(GREENLED_PIN, HIGH);

  lcd.init();
  lcd.backlight();
  startMessage(false);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    registerMode = true;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Registro");
    lcd.setCursor(0, 1);
    lcd.print("Aproxime...");

    tone(BUZZER_PIN, 700);
    delay(200);
    tone(BUZZER_PIN, 1500);
    delay(200);
    tone(BUZZER_PIN, 700);
    delay(200);
    tone(BUZZER_PIN, 1500);
    delay(200);
    tone(BUZZER_PIN, 700);
    delay(200);
    tone(BUZZER_PIN, 1500);
    delay(200);
    noTone(BUZZER_PIN);

    digitalWrite(REDLED_PIN, HIGH);
    digitalWrite(GREENLED_PIN, HIGH);
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uidRead = "";
  for (byte i = 0; i < rfid.uid.size; i++) {        
      uidRead.concat(String(rfid.uid.uidByte[i], HEX));
  }  

  if(registerMode) {
    registerCard(uidRead);
    delay(500);
  } else {
    int arraySize = sizeof(acceptedIDs) / sizeof(String) - 1;
    bool validID = false;
    
    for(int i = 0; i < arraySize; i++) {
      if (uidRead.equalsIgnoreCase(acceptedIDs[i])) {
        validID = true;
      }
    }

    if(validID) {
      printAllowed(uidRead);
      delay(1000);
    } else {
      printNotAllowed(uidRead);
      delay(1000);
    }
  }

  startMessage(false);
}

void registerCard(String uid) {
  pushArray(uid);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Novo cartao");
  lcd.setCursor(0, 1);
  lcd.print("adicionado");

  tone(BUZZER_PIN, 1500);
  delay(200);
  tone(BUZZER_PIN, 1500);
  delay(200);
  noTone(BUZZER_PIN);

  digitalWrite(REDLED_PIN, LOW);
  
  digitalWrite(GREENLED_PIN, HIGH);
  delay(200);
  digitalWrite(GREENLED_PIN, LOW);
  delay(200);
  digitalWrite(GREENLED_PIN, HIGH);
  delay(200);
  digitalWrite(GREENLED_PIN, LOW);
  delay(200);
  digitalWrite(GREENLED_PIN, HIGH);

  registerMode = false;
}

void printAllowed(String uid) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso liberado");
  lcd.setCursor(0, 1);
  lcd.print(uid);

  tone(BUZZER_PIN, 700);
  delay(1000);
  noTone(BUZZER_PIN);

  digitalWrite(GREENLED_PIN, LOW);
  delay(300);
  digitalWrite(GREENLED_PIN, HIGH);
  delay(300);
  digitalWrite(GREENLED_PIN, LOW);
  delay(300);
  digitalWrite(GREENLED_PIN, HIGH);
}

void printNotAllowed(String uid) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso negado");
  lcd.setCursor(0, 1);
  lcd.print(uid);

  tone(BUZZER_PIN, 700);
  delay(300);
  noTone(BUZZER_PIN);
  delay(300);
  tone(BUZZER_PIN, 700);
  delay(300);
  noTone(BUZZER_PIN);

  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(REDLED_PIN, HIGH);

  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, HIGH);
}

void pushArray(String element) {
  Serial.print("Element added in ");
  Serial.println(lastIndex);
  acceptedIDs[lastIndex] = element;
  lastIndex++;
}

void startMessage(bool buzzerOn = true) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bem vindo!");
  lcd.setCursor(0, 1);
  lcd.print("Aproxime cartao");

  int tone1 = 1437;
  int tone2 = 1337;

  if (buzzerOn) {
    for(int i = 0; i < 5; i++) {
      tone(BUZZER_PIN, tone2);
      delay(90);
      tone(BUZZER_PIN, tone1);
      delay(90);
    }

    tone(BUZZER_PIN, tone2);
    delay(120);
    noTone(BUZZER_PIN);
  }
}
