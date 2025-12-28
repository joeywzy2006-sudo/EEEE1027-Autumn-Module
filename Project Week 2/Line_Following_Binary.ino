#include <LiquidCrystal.h>
#include <PinChangeInterrupt.h>
const int enA = 11;
const int in1 = 13;
const int in2 = 12;
const int in3 = 2;
const int in4 = 1;
const int enB = 3;

const int L_IR = 0;   // Left IR sensor (analog input)
const int R_IR = A3;   // Right IR sensor (analog input)

// --- Initialize lcd ---
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

// --- Initialize encoder ---
#define L_Encoder A2  // Left encoder (analog input)
#define R_Encoder A3  // Right encoder (analog input)
volatile long leftCount = 0;       // Left encoder pulse counter
volatile long rightCount = 0;      // Right encoder pulse counter
const int Pulse = 20;              // Pulses per revolution (PPR) of the encoder
const float Wheel_CIRC = 20.106;   // Wheel circumference in centimeters (e.g., 20.106cm for a 64mm wheel)
float L_Distance = 0, R_Distance = 0, Avg_Distance = 0;

// --- Initialize ultrasonic sensor ---
#define trig A2 //analog input 
#define echo A3 //analog output 
float duration = 0;
float distance = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);

  lcd.begin(16, 2); 
  lcd.setCursor(0, 0);
  lcd.print("Distance:000.0cm");
  lcd.setCursor(0, 1);
  lcd.print("Time:00.00s");


  pinMode(L_Encoder, INPUT_PULLUP);
  pinMode(R_Encoder, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(L_Encoder), L_Interrupt, RISING);
  attachPCINT(digitalPinToPCINT(R_Encoder), R_Interrupt, RISING);
}

void loop() {
  unsigned long currentMillis = millis();

    // --- digital line following system ---
  if ((digitalRead(R_IR) == 1) && (digitalRead(L_IR) == 1))
  {
    forward();
  }
  else if ((digitalRead(R_IR) == 1) && (digitalRead(L_IR) == 0))
  {
    turnLeft();
    delayMicroseconds(20);
    /*while (digitalRead(R_IR) == 1)
    {
      turnLeft();
    }*/
  }
  else if ((digitalRead(R_IR) == 0) && (digitalRead(L_IR) == 1))
  {
    turnRight();
    delayMicroseconds(20);
    while (digitalRead(L_IR) == 1)
    {
      turnRight();

    }
  }
  else if((digitalRead(R_IR) == 0) && (digitalRead(L_IR) == 0))
  {
    stop();
    while(1)
    {}
  }
  
  // --- Average disctance calculation --- 
  L_Distance = (float)(leftCount * Wheel_CIRC / Pulse);
  R_Distance = (float)(rightCount * Wheel_CIRC / Pulse);
  Avg_Distance = (float)((L_Distance + R_Distance) / 2);
  
  // --- lcd output ---
  lcd.setCursor(9, 0);
  //lcd.print(Avg_Distance, 1);
  lcd.print("cm    ");
  lcd.setCursor(5, 1); 
  lcd.print((float)currentMillis / 1000, 2);
  lcd.print("s    ");
}

// ============================
// Move Forward Functions
// ============================
void forward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, 110);
  analogWrite(enB, 110);
}

// ============================
// Stop Functions
// ============================
void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// ============================
// Turn Right Functions
// ============================
void turnLeft(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(enA, 130);
  analogWrite(enB, 130);
}

// ============================
// Turn Left Functions
// ============================
void turnRight(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  analogWrite(enA, 100);
  analogWrite(enB, 100);
}

// ============================
// Pulse Counter Functions
// ============================
void L_Interrupt(){
  leftCount ++;
}

void R_Interrupt(){
  rightCount ++;
}
