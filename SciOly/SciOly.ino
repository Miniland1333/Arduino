#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <XBOXUSB.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  110 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  550 // this is the 'maximum' pulse length count (out of 4096)

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
XBOXUSB Xbox(&Usb);

//Define Starting positions
double Bottom = 85,
LLift  = 100,
RLift  = 80,
Top  = 30,
four   = 90,
five   = 180,
temp   = 0,
rate   = 5;//Equivilent to MS delay


void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.println(F("\r\nXBOX USB Library Started"));
}
void loop() {
  //Modify Zone
  Usb.Task();
  if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
      Bottom-=Xbox.getAnalogHat(LeftHatX)/32768.0/rate;
    }
    if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
      LLift-=Xbox.getAnalogHat(LeftHatY)/32768.0/rate;
      RLift+=Xbox.getAnalogHat(LeftHatY)/32768.0/rate;
      Top+=Xbox.getAnalogHat(LeftHatY)/32768.0/rate;
    }
    if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
      four+=Xbox.getAnalogHat(RightHatX)/32768.0/rate;
    }
    if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      LLift+=Xbox.getAnalogHat(RightHatY)/32768.0/rate;
      RLift-=Xbox.getAnalogHat(RightHatY)/32768.0/rate;
    }
    Serial.println(Xbox.getAnalogHat(RightHatX)/32768.0/rate);
  }


  //Check Zone
  Bottom = check(Bottom,0,180);
  LLift = check(LLift,0,180);
  RLift = check(RLift,0,180);
  Top = check(Top,0,180);
  four = check(four,0,180);
  five = check(five,0,180);

  //Implement Zone
  setServo(0,Bottom);
  setServo(1,LLift);
  setServo(2,RLift);
  setServo(3,Top);
  setServo(4,four);
  Serial.println(String(LLift)+", "+String(RLift));
}
//32768 max axis input

void setServo(int servonum, int degrees){
  temp  = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  //Serial.println(degrees);
  pwm.setPWM(servonum,0,temp);
}
double check(double input, int min, int max){
  if(min>max){
    Serial.println(String("Min of "+String(min)+" is greater than Max of "+String(max)));
  }
  //Serial.println(String(input)+" "+String(min)+" "+String(max));
  if(input>max){input=max;}
  if(input<min){input=min;}
  return input;
}
