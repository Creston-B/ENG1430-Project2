#include <Adafruit_MotorShield.h>
#include "Adafruit_seesaw.h"
#include <Servo.h>

Adafruit_seesaw ss;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Servo servo1;

int WETVAL = 950; //temp wetval
int DRYVAL = 450; //temp dryval
//value determinig if servo was put in open or closed state
bool servo1open = false;



void setup() {
  Serial.println("First Line of Code");
  Serial.begin(115200);
  Serial.println("HELP");

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
  servo1.attach(10);

  // Get initial cap value
  uint16_t initread = ss.touchRead(0);

  if(initread >= 500) { // Rotate to closed potiiton if above or equal to arbitrary
    servo1.write(180);
    delay(800);
    servo1.write(90);
    servo1open = false;
  } else if (initread < 500) { // Rotate to open position if below arbitrary
    servo1.write(0);
    delay(800);
    servo1.write(90);
    servo1open = true;
  }
}

void loop() {
  //take capacitive value
  uint16_t capread = ss.touchRead(0);
  

  // test for wet / dry conditions
  if((capread > WETVAL) && (servo1open == true)) { 
  // Rotate to closed potiiton if wet and open
    servo1.write(180);
    delay(800);
    servo1.write(90);
  } else if ((capread < DRYVAL) && (servo1open == false)) { 
    // Rotate to open position if dry and closed
    servo1.write(0);
    delay(800);
    servo1.write(90);
  }
  
  float tempC = ss.getTemp();

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);

  // Pause for 2 seconds
  delay(2000);
}