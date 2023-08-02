#include <math.h> 
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Wire.h>

// Define pin connections & motor's steps per revolution
AccelStepper stepper1(1, 3, 2); // (Type:driver, STEP, DIR)
AccelStepper stepper2(1, 5, 4);
AccelStepper stepper3(1, 7, 6); // (Type:driver, STEP, DIR)
MultiStepper StepperControl; 

int ls1 = 8;
int ls2 = 9;
int ls3 = 10;

const int fan1 = 20;
long time = 0;
long debounce = 500;

void setup()
{
  pinMode(ls1, INPUT);
  pinMode(ls2, INPUT);
  pinMode(ls3, INPUT);

  pinMode(fan1, OUTPUT);

 
  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(400);
  stepper2.setMaxSpeed(1000);
  stepper2.setSpeed(400);
  stepper3.setMaxSpeed(1000);
  stepper3.setSpeed(400);

  StepperControl.addStepper(stepper1);
  StepperControl.addStepper(stepper2);
  StepperControl.addStepper(stepper3);
  Wire.begin();
  Serial.begin(115200);
  

  while (!Serial){
    ;//wait for serial to connect
  }
  Serial.println("Serial On");

  home();
}

void home() {
  
  while(digitalRead(ls1) == 1){
    stepper1.setSpeed(400);
    stepper1.runSpeed();       
    }
  stepper1.setCurrentPosition(0);
  for(int i=0;i < 200; i++){
    stepper1.setSpeed(-400);
    stepper1.runSpeed();
    delayMicroseconds(2000);
  }

  while(digitalRead(ls2) == 1){
    stepper2.setSpeed(400);
    stepper2.runSpeed();       
  }
  stepper2.setCurrentPosition(0);
  for(int i=0;i < 200; i++){
    stepper2.setSpeed(-400);
    stepper2.runSpeed();
    delayMicroseconds(2000);
  }

  while(digitalRead(ls3) == 1){
    stepper3.setSpeed(-400);
    stepper3.runSpeed();       
  }
  stepper3.setCurrentPosition(0);
  for(int i=0;i < 200; i++){
    stepper3.setSpeed(400);
    stepper3.runSpeed();
    delayMicroseconds(2000);
  }
 
}


void loop(){
  char buffer[16];
  Serial.println("");
  Serial.print("Zoom:");//max14600
  Serial.print(stepper1.currentPosition());
  Serial.print(" Iris:");//max 6200
  Serial.print(stepper2.currentPosition());
  Serial.print(" Focus:");//max14800
  Serial.print(stepper3.currentPosition());

   
  
  while (Serial.available()){

    //Wire.requestFrom(8, 255);
    //char input = Serial.read();
    int input = Serial.readBytesUntil('\n', buffer,12);
    
    
    if((buffer[0] == 'f') && (millis() - time > debounce)){
        for(int i=0;i < 800; i++){
          digitalWrite(fan1, HIGH);
          delayMicroseconds(2000);
          time = millis();
        }
        digitalWrite(fan1, LOW);
    }
    byte err, adr;
    if((buffer[0] == 'l') && (millis() - time > debounce)){
//        for(int i=0;i < 5000; i++){
//          Wire.requestFrom(8, 255);
//        }
        for(adr = 1; adr < 127; adr++){
          Wire.beginTransmission(adr);
          Wire.endTransmission();
        }
        
    }
    
    if((buffer[0] == 'w') && (stepper1.currentPosition() <= 14500)){
      Serial.println("ZOOM IN");
      for(int i=0;i < 200; i++){
        stepper1.setSpeed(-400);
        stepper1.runSpeed();
        delayMicroseconds(2000);
        }
    }

    if((buffer[0] == 's') && (stepper1.currentPosition() >= 150)){
      Serial.println("ZOOM OUT");
      for(int i=0;i < 200; i++){
        stepper1.setSpeed(400);
        stepper1.runSpeed();
        delayMicroseconds(2000);
      }
    }

    if((buffer[0] == 'a') && (stepper2.currentPosition() >= 6100)){
      Serial.println("IRIS +");
      for(int i=0;i < 200; i++){
        stepper2.setSpeed(400);
        stepper2.runSpeed();
        delayMicroseconds(2000);
      }
    }

    if((buffer[0] == 'q') && (stepper2.currentPosition() <= 150)){
      Serial.println("IRIS -");
      for(int i=0;i < 200; i++)
      {
        stepper2.setSpeed(-400);
        stepper2.runSpeed();
        delayMicroseconds(2000);
      }
    }

    if((buffer[0] == 'e') && (stepper3.currentPosition() >= -14600)){
      Serial.println("FOCUS +");
      for(int i=0;i < 200; i++){
        stepper3.setSpeed(400);
        stepper3.runSpeed();
        delayMicroseconds(2000);
      }
    }

    if((buffer[0] == 'd') && (stepper3.currentPosition() <= -100)){
      Serial.println("FOCUS -");
      for(int i=0;i < 200; i++){
        stepper3.setSpeed(-400);
        stepper3.runSpeed();
        delayMicroseconds(2000);
      }
    }

  }
}
	
