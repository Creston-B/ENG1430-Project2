#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper1 = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(200, 2);

// Wrappers for the first motor
void forwardstep1() {
  myStepper1->onestep(FORWARD, SINGLE);
}
void backwardstep1() {
  myStepper1->onestep(BACKWARD, SINGLE);
}

// Wrappers for the second motor
void forwardstep2() {
  myStepper2->onestep(FORWARD, SINGLE);
}
void backwardstep2() {
  myStepper2->onestep(BACKWARD, SINGLE);
}

// Create AccelStepper objects for the two steppers
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

int wetval = 500; //temp wetval
int dryval = 500; //temp dryval

void setup() {
  Serial.begin(115200);
  AFMStop.begin(); // Start the top shield

  stepper1.setMaxSpeed(100.0);
  stepper1.setAcceleration(100.0);

  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(100.0);
  bool seesawinit = false;
  while(!seesawinit)
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    delay(2000);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
    seesawinit = true;
  }
}


void loop() {

  //wait to run
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
  }
 
  //take capacitive value
  uint16_t capread = ss.touchRead(0);

  // test for wet / dry conditions
  if(capread > wetval) { // Rotate 90 degrees clockwise if wet 
  stepper1.moveTo(45); // 200 steps for 90 degrees
  stepper2.moveTo(45); // 200 steps for 90 degrees
  } else if (capread < dryval) { // Rotate 90 degrees counterclockwise if dry
  stepper1.moveTo(0); // -200 steps for 90 degrees (opposite direction)
  stepper2.moveTo(0); // -200 steps for 90 degrees (opposite direction)
  }

  
  float tempC = ss.getTemp();

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);

    // Pause for 2 seconds
  delay(2000);
}
