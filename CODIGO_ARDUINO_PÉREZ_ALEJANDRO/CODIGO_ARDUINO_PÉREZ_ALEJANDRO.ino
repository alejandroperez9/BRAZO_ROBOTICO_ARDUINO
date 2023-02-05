#include <Stepper.h>
#include <Servo.h>

#define STEPS  2048

#define IN1  11
#define IN2  10
#define IN3   9
#define IN4   8

Stepper stepper(STEPS, IN4, IN2, IN3, IN1);

int8_t  quad = 0;
uint8_t previous_data;

Servo servoMotorB;
Servo servoMotorC;
Servo servoMotorP; 

int potenciometroPinB = 3; 
int potenciometroPinC = 1;
int potenciometroPinP = 2;

int valB;
int valC;
int valP;


void setup()
{
  stepper.setSpeed(15);

  previous_data = digitalRead(A5) << 1 | digitalRead(A4);
  PCICR  = 2;
  PCMSK1 = 0x30;

  servoMotorB.attach(7);
  servoMotorC.attach(4);
  servoMotorP.attach(2);

}


ISR (PCINT1_vect){
  uint8_t current_data = digitalRead(A5) << 1 | digitalRead(A4);

  if( current_data == previous_data )
    return;

  if( bitRead(current_data, 0) == bitRead(previous_data, 1) )
    quad -= 1;
  else
    quad += 1;
  previous_data = current_data;
}

int8_t encoder_update(void){
  
  int8_t val = 0;
  
  while(quad >= 4){
    val += 1;
    quad -= 4;
  }
  
  while(quad <= -4){
    val -= 1;
    quad += 4;
  }
  
  return val;
}


void loop(){
  
  int8_t stp = encoder_update();

  while(stp != 0){
    
    int8_t dir = (stp > 0) ? -1 : 1;
    stepper.step( 20 * dir );
    stp += dir;
    stp += encoder_update();

  }

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  delay(100);

  valB = analogRead(potenciometroPinB);
  valB = map(valB, 0, 1023, 0, 180); 
  servoMotorB.write(valB); 

  valC = analogRead(potenciometroPinC);
  valC = map(valC, 0, 1023, 0, 180); 
  servoMotorC.write(valC); 

  valP = analogRead(potenciometroPinP);
  valP = map(valP, 0, 1023, 0, 180); 
  servoMotorP.write(valP);   
  
  delay(15);   

}
