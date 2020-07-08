# include <Encoder.h>
# include <ros.h>
# include <std_msgs/Int16.h>
# include <geometry_msgs/Twist.h>
# include <std_msgs/Float32.h>
# include <std_msgs/String.h>
# include <TimerOne.h>   

Encoder knobLeft(2,4) ;
Encoder knobRight(3,5);   

#define LOOP_TIME 100000
#define ENA 6
#define ENB 9
#define IN1 24
#define IN2 26
#define IN3 7
#define IN4 8
int relay_pin= 22;
float left_pwm, right_pwm;

ros :: NodeHandle nh;
std_msgs::Int16 left_wheel_enc;
std_msgs::Int16 right_wheel_enc;
std_msgs::Int16 right_wheel_values;
std_msgs::Int16 left_wheel_values;
std_msgs::Float32 open_lock_value;
std_msgs::Float32 check_open_lock;

ros :: Publisher check_open_lock_pub("open_lock",&check_open_lock);
ros :: Publisher left_wheel_enc_pub("left_encoder",&left_wheel_enc);
ros :: Publisher right_wheel_enc_pub("right_encoder",&right_wheel_enc);
void timerIsr()
{
  Timer1.detachInterrupt();//stopthetimer
  right_wheel_enc.data=knobRight.read();
  left_wheel_enc.data=knobLeft.read();
  right_wheel_enc_pub.publish(&right_wheel_enc);
  left_wheel_enc_pub.publish(&left_wheel_enc);
  Timer1.attachInterrupt(timerIsr);//enablethetimer
}

void cmdLeftWheelCB(const std_msgs::Float32& msg)
{
    left_wheel_values.data = msg.data;
}

void cmdRightWheelCB(const std_msgs::Float32& msg)
{
  right_wheel_values.data = msg.data;
}

void open_lock(const std_msgs::Float32& msg)
{ 
  // int result;
  // result=0;
  // open_lock_value.data=msg.data;
  // result = msg.data
  if(msg.data > 0){
    check_open_lock.data=msg.data;
    check_open_lock_pub.publish(&check_open_lock);
    digitalWrite(relay_pin, HIGH);
    delay(5000);
    check_open_lock.data=0;
    check_open_lock_pub.publish(&check_open_lock);
    digitalWrite(relay_pin, LOW);
  }
}

ros::Subscriber<std_msgs::Float32> subCmdLeft("left_pwm",&cmdLeftWheelCB);
ros::Subscriber<std_msgs::Float32> subCmdRight("right_pwm",&cmdRightWheelCB);
ros::Subscriber<std_msgs::Float32> openLock("locker",&open_lock);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode (ENA, OUTPUT) ;
  pinMode (ENB, OUTPUT) ;
  pinMode ( IN1 , OUTPUT) ;
  pinMode ( IN2 , OUTPUT) ;
  pinMode ( IN3 , OUTPUT) ;
  pinMode ( IN4 , OUTPUT) ;
  pinMode ( relay_pin, OUTPUT) ;
  analogWrite(ENA,0);
  analogWrite(ENB,0);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  Timer1.initialize(LOOP_TIME);
  nh.initNode();
  nh.subscribe(subCmdLeft);
  nh.subscribe(subCmdRight);
  nh.subscribe(openLock);
  nh.advertise(left_wheel_enc_pub);
  nh.advertise(right_wheel_enc_pub);
  nh.advertise(check_open_lock_pub);
  
  Timer1.attachInterrupt(timerIsr);
  left_wheel_values.data=0;
  right_wheel_values.data=0;
}

void loop() {

  nh.spinOnce();
  /**
  if (left_wheel_values.data > 0){
    digitalWrite(IN1,HIGH); 
    digitalWrite(IN2,LOW);
  }
  if (left_wheel_values.data < 0){
    digitalWrite(IN1,LOW);  
    digitalWrite(IN2,HIGH);
  }
  if (right_wheel_values.data > 0){
    digitalWrite(IN3,HIGH);  
    digitalWrite(IN4,LOW);
  }
  if (right_wheel_values.data < 0){
    digitalWrite(IN3,LOW);  
    digitalWrite(IN4,HIGH);
  }
  else{
    digitalWrite(IN1,LOW);  
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);  
    digitalWrite(IN4,LOW);
  }**/
  
  if (left_wheel_values.data > 0 && right_wheel_values.data > 0)  //FWD Condition
  {
    digitalWrite(IN1,HIGH);  //right forward
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);   //left forward
    digitalWrite(IN4,LOW);
  }
  else if (left_wheel_values.data < 0 && right_wheel_values.data < 0)
  {
    digitalWrite(IN1,LOW);  //right backwards
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);   //left backwards
    digitalWrite(IN4,HIGH);
  }

  else if (left_wheel_values.data > 0 && right_wheel_values.data < 0)
  {
    digitalWrite(IN1,LOW);  //right backwards
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,HIGH);   //left forward
    digitalWrite(IN4,LOW);
  }
  
  else if (left_wheel_values.data == 0 && right_wheel_values.data == 0)
  {
    digitalWrite(IN3,LOW);   //left backward
    digitalWrite(IN4,LOW);
    digitalWrite(IN1,LOW);  //right forward
    digitalWrite(IN2,LOW);
  }

  else
  {
    digitalWrite(IN3,LOW);   //left backward
    digitalWrite(IN4,HIGH);
    digitalWrite(IN1,HIGH);  //right forward
    digitalWrite(IN2,LOW);
  }
  
    
    analogWrite(ENB, abs(left_wheel_values.data));
    analogWrite(ENA, abs(right_wheel_values.data));
}
