/*
#include <LiquidCrystal.h>

// --- Initialize lcd ---
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

#define trigPin A2 //analog input 
#define echoPin A3 //analog output 

float duration, distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  lcd.setCursor(0, 1);
  lcd.print(distance);
  delay(1000);
}
*/

/*
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
	Serial.begin(9600);
   
  // Try to initialize!
	if (!mpu.begin()) {
		Serial.println("Failed to find MPU6050 chip");
		while (1) {
		  delay(10);
		}
	}
	// set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

	// set gyro range to +- 500 deg/s
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);

	 // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
	// Get new sensor events with the readings 
	sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
	 
  // Print out the values
	Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

	Serial.print("Rotation X: ");
	Serial.print(g.gyro.x);
	Serial.print(", Y: ");
	Serial.print(g.gyro.y);
	Serial.print(", Z: ");
	Serial.print(g.gyro.z);
	Serial.println(" rad/s");
	Serial.print(angle);

	Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

	Serial.println("");
	delay(500);
}
*/


#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

#include <LiquidCrystal.h>

// --- Initialize lcd ---
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

unsigned long timer = 0;

void setup(){
	Serial.begin(9600);
	lcd.begin(16, 2);
	Wire.begin();
	mpu.begin();
	Serial.print(F("MPU6050 status: "));
	Serial.println(F("Calculating offsets, do not move MPU6050"));
	delay(500);
	mpu.calcOffsets(true, true);
	mpu.setFilterGyroCoef(1.0);
	Serial.print("Done!\n");
}

void loop(){
	mpu.update();

	if ((millis() - timer) > 100) 
	{
		timer = millis();
		lcd.setCursor(0, 0);
		lcd.print(mpu.getAngleY());
		lcd.setCursor(0, 1);
		lcd.print(mpu.getAngleX());
		Serial.print("\n");
	}

	if ((millis() - timer) > 100) 
	{
		timer = millis();
		lcd.setCursor(0, 0);
		lcd.print(mpu.getAngleX());
		Serial.print("\n");
}


#include <LiquidCrystal.h>

const int L_IR = A2;   // Left IR sensor (analog input)
const int R_IR = A3;   // Right IR sensor (analog input)

// --- Initialize lcd ---
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 

void setup() {
  pinMode(L_IR, INPUT);
  pinMode(R_IR, INPUT);
	lcd.begin(16, 2);
}

void loop() {
  lcd.setCursor(0, 0);
	lcd.print(digitalRead(L_IR));
	lcd.setCursor(0, 1);
	lcd.print(digitalRead(R_IR));
}
