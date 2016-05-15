/*************************************************************
Motor Shield 1-Channel DC Motor Demo
by Randy Sarafan

For more information see:
http://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/


unsigned long Time;
unsigned long RUNTIME=3000;

void setup() {

  Serial.begin(115200);

  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  pinMode(8, OUTPUT); //Initiates Brake Channel B pin


  Serial.println("Forward");
  //forward @ full speed
  digitalWrite(12, HIGH); //Establishes forward direction of Channel A
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A
  analogWrite(3, 50);   //Spins the motor on Channel A at full speed

  digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B
  analogWrite(11, 50);   //Spins the motor on Channel B at full speed

  Time = millis();
  while ((millis() - Time) <= RUNTIME) {
    Serial.println((millis() - Time));
  }

  Serial.println("STOP");
  digitalWrite(9, HIGH); //Engage the Brake for Channel A
  digitalWrite(8, HIGH); //Engage the Brake for Channel B

}

void loop() {}
