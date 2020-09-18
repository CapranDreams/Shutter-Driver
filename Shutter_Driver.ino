#include <TimerOne.h>

#define LED0        LED_BUILTIN         // 13
#define LED1        21
#define LED2        20
#define MOTOR_A     4
#define MOTOR_B     6
#define ENABLE      3
#define TRIGGER     9

volatile int LED_cntr = 0;
volatile int LED_dir = 1;

void setup() {
     pinMode(LED0, OUTPUT);        digitalWrite(LED0, LOW);
     pinMode(LED1, OUTPUT);        digitalWrite(LED1, LOW);
     pinMode(LED2, OUTPUT);        digitalWrite(LED2, LOW);
     
     pinMode(MOTOR_A, OUTPUT);     digitalWrite(MOTOR_A, LOW);
     pinMode(MOTOR_B, OUTPUT);     digitalWrite(MOTOR_B, LOW);
     pinMode(ENABLE, OUTPUT);      digitalWrite(ENABLE, LOW);
     analogWriteFrequency(MOTOR_A, 10000);
     analogWriteFrequency(MOTOR_B, 10000);
     analogWriteResolution(8);
     
     pinMode(TRIGGER, INPUT_PULLUP);     

     Timer1.initialize(25000);
     Timer1.attachInterrupt(blinkLED); // blinkLED to run every 1 seconds
     
     Serial.begin(9600);
     
     digitalWrite(ENABLE, HIGH);
}

void loop() {

     while(Serial.available() > 0) {
          int inByte = Serial.read();
          Serial.println(inByte, DEC);
          setMotor(inByte);
     }
     

}

void blinkLED(void)
{
     
     if(LED_dir) LED_cntr+=5;
     else LED_cntr-=5;
     
     if(LED_cntr>=255 || LED_cntr == 0) LED_dir = !LED_dir;
     
     analogWrite(LED2, LED_cntr);
}

void setMotor(int state) {

     digitalWrite(ENABLE, HIGH);
     if(state == 49) { // "1"
          motorPWM(0, 255);
          delayMicroseconds(500);
          motorPWM(1, 50);
          delayMicroseconds(15);
          motorPWM(0, 80);
          delayMicroseconds(5);
          motorPWM(0, 20);
     }
     else if(state == 50) { // "2"
          motorPWM(1, 255);
          delayMicroseconds(500);
          motorPWM(0, 50);
          delayMicroseconds(15);
          motorPWM(1, 80);
          delayMicroseconds(5);
          motorPWM(1, 20);
     }
     else {
          digitalWrite(ENABLE, LOW);
          digitalWrite(MOTOR_A, LOW);
          digitalWrite(MOTOR_B, LOW);
     }
}

void motorPWM(int state, int duty) {
     if(state) {
          analogWrite(MOTOR_A, duty);
          analogWrite(MOTOR_B, 0);
          digitalWrite(LED1, HIGH);
     }
     else {
          analogWrite(MOTOR_B, duty);
          analogWrite(MOTOR_A, 0);
          digitalWrite(LED1, LOW);
     }
}
