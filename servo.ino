#include <Adafruit_MotorShield.h>
#include "Adafruit_seesaw.h"
#include <Servo.h>

Adafruit_seesaw ss;
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

Servo servo1;
Servo servo2;

int wetval = 500; //temp wetval
int dryval = 500; //temp dryval

void setup() {
  Serial.begin(115200);
  AFMStop.begin(); // Start the top shield
  
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
  
  servo1.attach(9); // attach servo to pin 9
  servo2.attach(10); // attach servo to pin 10
}

void loop() {
  //take capacitive value
  uint16_t capread = ss.touchRead(0);

  // test for wet / dry conditions
  if(capread > wetval) { // Rotate 90 degrees clockwise if wet 
    servo1.write(90); // Rotate servo1 to 90 degrees
    servo2.write(90); // Rotate servo2 to 90 degrees
  } else if (capread < dryval) { // Rotate 90 degrees counterclockwise if dry
    servo1.write(0); // Rotate servo1 to 0 degrees
    servo2.write(0); // Rotate servo2 to 0 degrees
  }
  
  float tempC = ss.getTemp();

  Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
  Serial.print("Capacitive: "); Serial.println(capread);

  // Pause for 2 seconds
  delay(2000);
}
