/*  ----------------------------------------------------------------
  Pausoz pausoko motorra alde batetara 4095 pauso mugitu eta ondoren
  beste aldera pauso berak mugitzen dituen programa.
  Motorraren kontrola PAUSO ERDIKO KONTOLAREKIN diseinatua dago.
  by Axpi
  This example code is in the public domain.
  Eskerrak  http://www.prometec.net/motor-28byj-48                                        Prog_4_2

  Stepperra: 28BYJ-48 con Arduino
  --------------------------------------------------------------------
*/
#include <LiquidCrystal_I2C.h>
#define IN1  13
#define IN2  12
#define IN3  11
#define IN4  10

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPinB = 3;
const int buttonPinD = 5;

const int buttonPinC = 4;  //Konfiguratu botoia

const int ledPin = 9;
int x = 0; // x ardatzaren irakurketa aldagaia

int steps_left = 5000;     // Pauso gehiago edo gutxiago definitzeko parametroa
int Direction = 0;  //0=Geldik , 1=Aurrera  2=Atzera
int Steps = 0;
int i = 0; // Konfigurazio aldagaia
float zm = steps_left * 0.0056;

// Pauso erdiko Matrizea. Beste kontroletarako matrizea aldatu.

; int Paso [ 4 ][ 4 ] =
{ {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},

};

void setup()
{
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinD, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Kaixo,ongietorri");
  lcd.setCursor(0, 1);
  lcd.print("KONFIGURAZIORA!!");
  delay(3000);

  while (i == 0) {

    x = analogRead(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mugitu: ");
    lcd.print(zm);
    lcd.print("mm");

    if (x < 450) {
      steps_left = steps_left + 10;
      zm = steps_left * 0.0056;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mugitu: ");
      lcd.print(zm);
      lcd.print(" mm");
    }
    if (x > 600) {
      steps_left = steps_left - 10;
      zm = steps_left * 0.0056;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mugitu: ");
      lcd.print(zm);
      lcd.print(" mm");
    }

    int readingC = digitalRead(buttonPinC);
    lcd.setCursor(0, 1);
    lcd.print("C botoia: atzera");
    if (readingC == 0) {
      i = 1;
    }
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("Oteitza Luneta: ");
  lcd.setCursor(0, 1);
  lcd.print("Mugitu ");
  lcd.print(zm);
  lcd.print(" ZM  ");
}

void loop()
{
  int readingB = digitalRead(buttonPinB);
  Serial.print("BotoiaB: ");
  Serial.println(readingB);

  int readingD = digitalRead(buttonPinD);
  Serial.print("BotoiaD: ");
  Serial.println(readingD);
  Serial.print(" ");

  if (readingB == 0) {
    Direction = 1;
    digitalWrite(ledPin, 1);
    mugitu();
  }
  if (readingD == 0) {
    Direction = 2;
    digitalWrite(ledPin, 1);
    mugitu();
  }

}
void SetDirection()
{
  if (Direction == 0) {}

  else if (Direction == 1) {
    Steps++;
  }
  else if (Direction == 2) {
    Steps--;
  }

  Steps = ( Steps + 4 ) % 4 ;
}
void stepper()              //Pauso bat aurrera egiten duen Funtzioa
{
  digitalWrite( IN1, Paso[Steps][ 0] );
  digitalWrite( IN2, Paso[Steps][ 1] );
  digitalWrite( IN3, Paso[Steps][ 2] );
  digitalWrite( IN4, Paso[Steps][ 3] );
  SetDirection();

  delay(2);

  /*
    Serial.print("Pausoak:  ");
    Serial.println(steps_left);
    Serial.print("Pausoa:  ");
    Serial.println(Steps);
    Serial.print("Norantza:  ");
    Serial.println(Direction);
    Serial.println("");
  */

}

void mugitu()
{ while (steps_left > 0) {
    stepper() ;     // Pauso bat aurrera
    steps_left-- ;  // Pauso bat gutxiago egin da.
    delay (1) ;
  }
  digitalWrite(ledPin, 0);
  steps_left = zm / 0.0056;
}
