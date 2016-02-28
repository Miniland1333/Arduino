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
double Main = 90,
       Arm2  = 92,
       Arm3  = 93,
       Height  = 90,
       Rotate   = 90,
       End   = 85,
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
  Usb.Task();

  //Axis
  if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
    //Main -= Xbox.getAnalogHat(LeftHatX) / 32768.0 / rate;

    Arm2 = map(Xbox.getAnalogHat(LeftHatX), -32767, 32768, 97, 85);
  } else {
    Arm2 = 93;
  }
  if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {}
  if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
    Rotate += Xbox.getAnalogHat(RightHatX) / 32768.0 / rate;
  }
  if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
    Height = map(Xbox.getAnalogHat(RightHatY), -32767, 32768, 84, 101);
  } else {
    Height = 90;
  }

  //Buttons
  if (Xbox.getButtonPress(X)) {
    Arm3 = 97;
  }
  else if (Xbox.getButtonPress(B)) {
    Arm3 = 89;
  }
  else {
    Arm3 = 93;
  }

  //  if (Xbox.getButtonPress(LEFT)) {Arm2 = 97;}
  //  else if (Xbox.getButtonPress(RIGHT)) {Arm2 = 85;}
  //  else {Arm2 = 92;}

  if (Xbox.getButtonPress(L2)) {
    End += 1 / rate;
  }
  else if (Xbox.getButtonPress(R2)) {
    End -= 1 / rate;
  }
  Serial.println(Arm2);


  //Check Zone
  Main = check(Main, 0, 180);
  Arm2 = check(Arm2, 50, 125);
  Arm3 = check(Arm3, 84, 101);
  Height = check(Height, 85, 100);
  Rotate = check(Rotate, 27, 150);
  End = check(End, 11, 85);

  //Implement Zone
  setServo(0, Main);
  setServo(1, Arm2);
  setServo(2, Arm3);
  setServo(3, Height);
  setServo(4, Rotate);
  setServo(5, End);
}
//32768 max axis input

void setServo(int servonum, int degrees) {
  temp  = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  //Serial.println(degrees);
  pwm.setPWM(servonum, 0, temp);
}
double check(double input, int min, int max) {
  if (min > max) {
    Serial.println(String("Min of " + String(min) + " is greater than Max of " + String(max)));
  }
  //Serial.println(String(input)+" "+String(min)+" "+String(max));
  if (input > max) {
    input = max;
  }
  if (input < min) {
    input = min;
  }
  return input;
}
