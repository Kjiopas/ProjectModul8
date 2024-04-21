#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
 
Servo myservo;
 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
 
#define Length 5
 
char Data[Length];
char Master[Length] = "7431";
byte datacount = 0;
char customKey;
byte wrongcounter=3;
const byte ROWS = 4;
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool closed = true;
 
byte rowPins[ROWS] = {1, 2, 3, 4};
byte colPins[COLS] = {5, 6, 7};
 
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
 
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  myservo.attach(9);
  for (int deg = 0; deg <= 180; deg += 1) { 
    myservo.write(deg);
    delay(15);
  }
  lcd.begin(16, 2);
  lcd.print("Smart Door Lock");
  lcd.setCursor(0, 1);
  lcd.print("Loading....");
  delay(1000);
  lcd.clear();
}
 
void loop() {
  if (closed) {
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    customKey = customKeypad.getKey();
 
    if (customKey != NO_KEY) {
      Data[datacount] = customKey;
      lcd.setCursor(datacount, 1);
      lcd.print("*");
      datacount++;
 
      if (datacount == Length-1) {
        if (strncmp(Data, Master, Length) == 0) { 
          lcd.clear();
          digitalWrite(10, HIGH);
          digitalWrite(11, LOW);
          digitalWrite(12, LOW);
          for (int deg = 180; deg >= 0; deg -= 1) { 
            myservo.write(deg);
            delay(15);
          }
          lcd.print(" Door is Opened ");
          closed = false;
          datacount = 0;
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wrong Password");
          delay(1000);
         lcd.setCursor(0, 1);
          lcd.print("Attempts left: ");
          lcd.print(wrongcounter-1);
          delay(1000);
          lcd.clear();
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);
          digitalWrite(12, LOW);
          datacount = 0;
          wrongcounter--;
        }
        if(wrongcounter==0)
        {
          lcd.setCursor(0,0);
          lcd.print("Entrance denied!");
          delay(1000);
          lcd.clear();
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);
          digitalWrite(12, LOW);
          datacount = 0; 
          wrongcounter=0;
           exit(0);
        }
      }
    }
  } else {
    customKey = customKeypad.getKey();
 
    if (customKey == '#') {
      lcd.clear();
      for (int deg = 0; deg <= 180; deg += 1) { 
        myservo.write(deg);
        delay(15);
      }
      lcd.print("Door is closed");
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      delay(3000);
      closed = true;
    }
    if (customKey == '*') {
      lcd.clear();
      for (int deg = 0; deg <= 180; deg += 1) { 
        myservo.write(deg);
        delay(15);
      }
      lcd.print("Secret door met");
      lcd.setCursor(0,1);
      lcd.print("Have a nice look");
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      delay(3000);
      closed = true;
      exit(0); 
    }
  }
}
