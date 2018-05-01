/*
   GoBabyGo Car Controller
   Writen by FIRST Team 1939 (The Kuhnigits)
   Modified by Willem Hillier of CVU
   Switch Configuration:
    Left: Switches Up, Header closer to switches
    Right: Switches Down, Header farther from switches
    Top switch: L/R
    Bottom switch: F/B
*/

#include <Servo.h>

// Set this to true if using a Speed Potentiometer
boolean SPEED_POTENTIOMETER = true;
// boolean DISTANCE_WARNING = true;

// Invert one or two of the motors
boolean INVERT_1 = false;
boolean INVERT_2 = true;

// Constants
int SPEED_LIMIT = 500; // Between 0-512
int DEADBAND = 10;
int RAMP_UP = 7;
int RAMP_DOWN = 5;

// Middle value of the upper and lower limit of the motor controller, 1500 is for Sparks and Talon SR's
int PULSE = 1500; 

// Pins
int JOYSTICK_X = 9;
int JOYSTICK_Y = 10;
int JOYSTICK_XNeg = 8;
int JOYSTICK_YNeg = 11;
int MOTOR_1    = 6;
int MOTOR_2    = 5;
int SPEED_POT  = A5;


// -----Don't Mess With Anything Past This Line-----

Servo motor1;
Servo motor2;
Servo servo;

void setup() {
  pinMode(JOYSTICK_X, INPUT_PULLUP);
  digitalWrite(JOYSTICK_X, HIGH);
  pinMode(JOYSTICK_Y, INPUT_PULLUP);
  digitalWrite(JOYSTICK_Y, HIGH);
  pinMode(JOYSTICK_XNeg, INPUT_PULLUP);
  digitalWrite(JOYSTICK_XNeg, HIGH);
  pinMode(JOYSTICK_YNeg, INPUT_PULLUP);
  digitalWrite(JOYSTICK_YNeg, HIGH);
  motor1.attach(MOTOR_1);
  motor2.attach(MOTOR_2);
  pinMode(SPEED_POT, INPUT);
  Serial.begin(9600);
}
int x;
int y;
int xNeg;
int yNeg;
int Speed;
void loop() {
  //Read from the joystick
  x = digitalRead(JOYSTICK_X);
  y = digitalRead(JOYSTICK_Y);
  xNeg = digitalRead(JOYSTICK_XNeg);
  yNeg = digitalRead(JOYSTICK_YNeg);

  // Reads from the Speed Potentiometer and divides that value (max of 1024) by 4 ----- The values sent to the motor controllers are between 1756 and 1244 (+ or - 256 the PULSE) ----- These values are a good speed for the WildThing
  Speed = (1024-analogRead(SPEED_POT)) / 4;

  // Establish a speed limit
  int limit = SPEED_LIMIT;
  if (SPEED_POTENTIOMETER) limit = map(analogRead(SPEED_POT), 0, 1023, 0, SPEED_LIMIT);

  if(x == LOW && y == HIGH && yNeg == HIGH){
    drive(-Speed, Speed);
    Serial.println("Moving Right");
  }
  
  else if (xNeg == LOW && y == HIGH && yNeg == HIGH) {
      drive(Speed , -Speed);
      Serial.println("Moving Left");
  }
  
  if (y == LOW && x == HIGH && xNeg == HIGH) {
      drive(-Speed, -Speed);
      Serial.println("Moving Back");
  }
  
  else if (yNeg == LOW &&x == HIGH && xNeg == HIGH) {
      drive(Speed, Speed);
      Serial.println("Moving Forward"); 
  }

  if (y == HIGH && x == HIGH && yNeg == HIGH && xNeg == HIGH){
    drive(0 , 0);
  }
    
  delay(10); //Make loop run approximately 50hz

}

int prevLeft = 0;
int prevRight = 0;

void drive(int leftSpeed, int rightSpeed) {

  int leftOut = 0;
  int rightOut = 0;
  int ramp = 0;
   
  if (((leftSpeed * prevLeft) < 0) || (abs(leftSpeed) < abs(prevLeft))){
    ramp = RAMP_DOWN;
  }

  else{
    ramp = RAMP_UP;
  }

  if (leftSpeed < prevLeft){
    leftOut = prevLeft - ramp;
    
  }

  else if (leftSpeed > prevLeft){
    leftOut = prevLeft + ramp;
  }

  if (INVERT_1){
    leftOut = -leftOut;
  }
  motor1.writeMicroseconds(leftOut + PULSE);
  prevLeft = leftOut;
  
  if (((rightSpeed * prevRight) < 0) || (abs(rightSpeed) < abs(prevRight))){
    ramp = RAMP_DOWN;
  }

  else{
    ramp = RAMP_UP;
  }

  if (rightSpeed < prevRight){
    rightOut = prevRight - ramp;
    Serial.println("Is Ramping Down");
  }

  else if (rightSpeed > prevRight){
    rightOut = prevRight + ramp;
    Serial.println("Is Ramping Up");
  }

  if (INVERT_2){
    rightOut = -rightOut;
  }
  motor2.writeMicroseconds(rightOut + PULSE);
  prevRight = -rightOut;
}

