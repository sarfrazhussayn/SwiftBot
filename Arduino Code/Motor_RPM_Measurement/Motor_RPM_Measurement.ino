// RPM Measurement of Encoder Motors //
/*
*- encVal/pulse_per_rev revolutions in ------- interval/1000 seconds b/c interval given in milliseconds
*- ? revolutions in -------------------------- 60 s
*- ? = (enc/pulse_per_rev)*60 / (int/1000) RPM i.e. revolutions in 60s
*/
int pulse_per_rev = 1120;
int interval = 50;
unsigned long current_t = 0;
float t = 60*(1000/interval);
float rpmA, rpmB;
int encoderPinA = 2;
int encoderPinB = 3;
volatile long encoderValA = 0;
volatile long encoderValB = 0;
int enA1 = 8;
int enA2 = 7;
int pwmA = 6;
int enB1 = 13;
int enB2 = 12;
int pwmB = 11;
void updateA() {
  encoderValA++;
}
void updateB() {
  encoderValB++;
}
void setup() {
    Serial.begin(9600);
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);
    attachInterrupt(1, updateA, RISING);
    attachInterrupt(0, updateB, RISING);
    pinMode(enA1, OUTPUT);
    pinMode(enA2, OUTPUT);  
    pinMode(pwmA, OUTPUT);
    pinMode(enB1, OUTPUT);
    pinMode(enB2, OUTPUT);  
    pinMode(pwmB, OUTPUT);
    digitalWrite(enA1, HIGH);
    digitalWrite(enA2, LOW);
    digitalWrite(enB1, LOW);
    digitalWrite(enB2, HIGH);
}
void loop() {
  analogWrite(pwmA, 200);
  analogWrite(pwmB, 200);
  if (millis() >= current_t + interval) {
    detachInterrupt;
    current_t += interval;
    rpmA = encoderValA*(t/pulse_per_rev);
    rpmB = encoderValB*(t/pulse_per_rev);
    encoderValA = 0;
    encoderValB = 0;
    Serial.println(rpmA); Serial.print("  ");
    Serial.print(rpmB); Serial.print("  ");
    attachInterrupt;
  }
}
\end{lstlisting}

\section{Arduino Based Proportional-Integral Controller}
\label{app PI}

\begin{lstlisting}[language=Arduino] 
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
