#include <Adafruit_MotorShield.h>
#include "Adafruit_seesaw.h"
#include <Servo.h>

Adafruit_seesaw ss;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

int WETVAL = 950; //temp wetval
int DRYVAL = 500; //temp dryval
int SERVOTIME = 700; //ms for servo to spin for a valve cycle
//value determinig if servo was put in open or closed state
bool valveopen;


int MAINOPENVAL = 0;
int MAINCLOSEDVAL = 180;

int SECONDOPENVAL = 0;
int SECONDCLOSEDVAL = 180;

Servo secondservo;
Servo mainservo;

void setup() {
  Serial.begin(115200);

  bool seesawinit = false;
  while(!seesawinit) {
    if (!ss.begin(0x36)) {
      Serial.println("ERROR! seesaw not found");
      delay(2000);
    } else {
      Serial.print("seesaw started! version: ");
      Serial.println(ss.getVersion(), HEX);
      seesawinit = true;
    }
  }
  
  bool afmsinit = false;
  while(!afmsinit) {
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
      delay(2000);
    } else { 
      Serial.println("Motor Shield found.");
      afmsinit = true;
    }
  }

  // Attach a servo to pin #10
  secondservo.attach(10);
  mainservo.attach(9);


  // Get initial cap value
  uint16_t initread = ss.touchRead(0);

  if(initread >= 500) { // Rotate to closed potiiton if above or equal to arbitrary
    secondservo.write(SECONDCLOSEDVAL);
    mainservo.write(MAINCLOSEDVAL);
    delay(SERVOTIME);
    secondservo.write(90);
    mainservo.write(90);
    valveopen = false;
  } else if (initread < 500) { // Rotate to open position if below arbitrary
    secondservo.write(SECONDOPENVAL);
    mainservo.write(MAINOPENVAL);
    delay(SERVOTIME);
    secondservo.write(90);
    mainservo.write(90);
    valveopen = true;
  }
}

void loop() {
  //take capacitive value
  uint16_t capread = ss.touchRead(0);
  

  // test for wet / dry conditions
  if((capread > WETVAL) && (valveopen == true)) { 
  // Rotate to closed potiiton if wet and open
    secondservo.write(SECONDCLOSEDVAL);
    delay(SERVOTIME);
    secondservo.write(90);
    delay(2500);
    mainservo.write(MAINCLOSEDVAL);
    delay(SERVOTIME);
    mainservo.write(90);
    valveopen = false;
  } else if ((capread < DRYVAL) && (valveopen == false)) { 
    // Rotate to open position if dry and closed
    secondservo.write(SECONDOPENVAL);
    mainservo.write(MAINOPENVAL);
    delay(SERVOTIME);
    secondservo.write(90);
    mainservo.write(90);
    valveopen = true;
  }
  
  float tempC = ss.getTemp();

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);
  if (valveopen) {
  Serial.println("Water valves are open");
  } else {
  Serial.println("Water valves are closed");
  }
  Serial.println("");
  Serial.println("-----------------------");
  Serial.println("");
  // Pause for 4 seconds
  delay(4000);
}