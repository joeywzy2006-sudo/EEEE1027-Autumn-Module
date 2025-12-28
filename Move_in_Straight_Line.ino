#include <LiquidCrystal.h>
#define enA 11
#define in1 12
#define in2 13
#define in3 1
#define in4 2
#define enB 3

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  analogWrite(enA, 100);
  analogWrite(enB, 95);

  lcd.begin(16, 2);
  lcd.print("Time: 0s");
  delay(1000);
}

void loop() {
  int duration = 10000;
  if (millis() < duration)
  {
    forward();
    for(int seconds = 1; seconds < 11; seconds++)
    {
      delay(1000);
      lcd.setCursor(6, 0);
      lcd.print(seconds);
      lcd.print("s");
    }
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print(duration/1000);
    lcd.print("s");
    stop();
    exit(0);
  }
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}





