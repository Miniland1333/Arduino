#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  110 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  550 // this is the 'maximum' pulse length count (out of 4096)

#include <XBOXUSB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXUSB Xbox(&Usb);

//Define Starting positions
uint16_t bottom = 85,
two    = 80,
three  = 30,
four   = 90,
five   = 180,
temp   = 0;


void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.println(F("\r\nXBOX USB Library Started"));
  setServo(0,bottom);
  Serial.println(temp);
  setServo(1,two);
  setServo(2,three);
  setServo(3,four);
  setServo(4,five);
}
void loop() {

}
//32227Max axis input

void setServo(uint8_t servonum, uint32_t degrees){
  temp  = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum,0,temp);
}
