#include <LiquidCrystal.h>
#include <PinChangeInterrupt.h>
const int enA = 11;
const int in1 = 13;
const int in2 = 12;
const int in3 = 2;
const int in4 = 1;
const int enB = 3;

const int L_IR = A4;   // Left IR sensor (analog input)
const int R_IR = A5;   // Right IR sensor (analog input)

// --- PID parameters ---
float kp = 0.9;
float ki = 0.0005;
float kd = 0.85;

// --- Control variables ---
float integral = 0;
float prev_error = 0;
float mv; // Manipulated variable

// --- Base speed ---
int baseSpeed = 100;  // adjust depending on motor & surface

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

  delay(100);
}

void loop() {
  unsigned long currentMillis = millis();

  int L_Value = analogRead(L_IR);
  int R_Value = analogRead(R_IR);

  //float error = (float)L_Value - (float)R_Value;
  float error = (float)R_Value - (float)L_Value;

  // --- PID calculation ---
  integral += error;
  integral = constrain(integral, -5000, 5000);
  float derivative = error - prev_error;
  mv = (kp * error) + (ki * integral) + (kd * derivative);
  prev_error = error;

  // --- Calculate motor speeds ---
  int lSpeed = baseSpeed - (int)mv;
  int rSpeed = baseSpeed + (int)mv;

  // --- Constrain motor speeds ---
  lSpeed = constrain(lSpeed, -170, 170);
  rSpeed = constrain(rSpeed, -170, 170);

  // --- Drive motors ---
  setMotor(in1, in2, enA, lSpeed);
  setMotor(in3, in4, enB, rSpeed);

  // --- Average disctance calculation --- 
  L_Distance = (float)(leftCount * Wheel_CIRC / Pulse);
  R_Distance = (float)(rightCount * Wheel_CIRC / Pulse);
  Avg_Distance = (float)((L_Distance + R_Distance) / 2);

  // --- lcd output ---
  lcd.setCursor(9, 0);
  lcd.print(Avg_Distance, 1);
  lcd.print("cm    ");
  lcd.setCursor(5, 1); 
  lcd.print((float)currentMillis / 1000, 2);
  lcd.print("s    ");

  delay(25);

  if(L_Value <= 150 && R_Value <= 150)  // stop when both IR sense black
  {
    Stop();
    while(1);
    {}
  }
}

// ============================
// Motor Control Function
// ============================
void setMotor(int in1, int in2, int en, int speed){
  if (speed > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(en, speed);
  } 
  else if (speed < 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(en, -speed);
  } 
  else {
    // Brake/Stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(en, 0);
  }
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

// ============================
// Stop Function
// ============================
void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
