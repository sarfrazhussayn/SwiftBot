// Motor encoder output pulse per rotation (change as required)
double enc_count_rev = 1120; //16*70 CPR

// Encoder output to Arduino Interrupt pin
#define ENCA 2
#define ENCB 3

// PWM/Speed pins for Motors
#define ENA 9
#define ENB 6

// Motor Directions
#define IN1A 10
#define IN2A 11
#define IN1B 7
#define IN2B 8

// PID constants
double ki = 0.7;
double kp = 0.5;
float error = 0;
long cumerror = 0;
long out = 0;
double gain = 1.1;  //have to set this by comparing motors

// RPM calculation
long currentMillis;
long prevMillis = 0;
double interval = 10;     //ms
double left_rpm, right_rpm;
double elapsedTime;
volatile long encoderValueA;
volatile long encoderValueB;

void setup() {
  // Setup Serial Monitor
  Serial.begin(57600);

  // Set PWM and DIR connections as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);

  // Attach interrupt
  attachInterrupt(digitalPinToInterrupt(ENCA), updateEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB), updateEncoderB, RISING);

  // Move motor forward
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN2A, LOW);
  analogWrite(ENA, 100);
  digitalWrite(IN1B, LOW);
  digitalWrite(IN2B, HIGH);
  analogWrite(ENB, 120);
}

void loop() {
  currentMillis = millis(); 
  elapsedTime = currentMillis - prevMillis;
  if(elapsedTime > interval) {
    noInterrupts();
    prevMillis = millis();
    right_rpm = (encoderValueA/enc_count_rev)*60*1000;
    right_rpm = right_rpm/interval;
    left_rpm = (encoderValueB/enc_count_rev)*60*1000;
    left_rpm = left_rpm/interval;
    encoderValueA = 0;
    encoderValueB = 0;
    interrupts();  
  } 
  error = right_rpm - left_rpm; 
  cumerror += error*elapsedTime/1000;
  out = kp*error+ ki*cumerror;
    
  if (error >=0) {
  left_rpm = left_rpm + abs(out)*gain;
  left_rpm = left_rpm*(255/150);
  analogWrite(ENB, left_rpm);
  //  delay(10);
  }
  else {
  left_rpm = left_rpm - abs(out)*gain;
  left_rpm = left_rpm*(255/150);
  analogWrite(ENB, left_rpm);
  //  delay(10);
  }
  Serial.print(error); Serial.println(" error");
  Serial.print(left_rpm); Serial.println(" left rpm");
  Serial.print(right_rpm); Serial.println(" right rpm");
  Serial.println(" ");
  delay(10);
}

void updateEncoderA() {
  // Increment value for each pulse from encoder
  encoderValueA++;
}
void updateEncoderB() {
  // Increment value for each pulse from encoder
  encoderValueB++;
}
