/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#define enA 5
#define in1 6
#define in2 7

#define angle 6353 // 90도
#define init_motor_speed 80
#define motor_speed 100

int door_state = 0; // init value

long newPosition = 0;
long oldPosition  = -999;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
//   avoid using pins with LEDs attached


void motor_dir(int i){
  if(i==0){ // CCW, Closing
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
  }else if(i==1){ // CW, Opening
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Booting");
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  analogWrite(enA, 0);
}



void loop() {
  newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  
  if(door_state == 1){ // Opening
     motor_dir(1);
     analogWrite(enA, motor_speed);
     if(newPosition>angle)
     {
      Serial.println("Door Opened");
      analogWrite(enA, 0);
      myEnc.write(0);
      door_state = 0;
     }
  }else if(door_state == 2) { // Closing
     motor_dir(0);
     analogWrite(enA, motor_speed);
     if(newPosition<(-angle))
     {
      Serial.println("Door Closed");
      analogWrite(enA, 0);
      myEnc.write(0);
      door_state = 0;
     }
  }
  
  if(Serial.available()){
    char c = Serial.read();
    if(c=='O'){
      door_state = 1;
    }else if(c=='C'){
      door_state = 2;
    }else if(c=='s'){
      analogWrite(enA, 0);
      myEnc.write(0);
    }else if(c=='c'){
      analogWrite(enA, init_motor_speed);
      motor_dir(0);   // CCW Closing dir
    }else if(c=='o'){
      analogWrite(enA, init_motor_speed);
      motor_dir(1);   // CW Opening dir
    }
  }
}
