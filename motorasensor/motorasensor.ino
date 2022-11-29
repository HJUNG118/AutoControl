/* 45 RPM HD Premium Planetary Gear Motor w/Encoder */

// POT
int potpin = A0; // analog pin used to connect the potentiometer
int val; // variable to read the value from the analog pin

// motor control pin
const int motorDirPin = 4; // Input 1
const int motorPWMPin = 5; // Input 2
const int EnablePin = 6;  // Enable

// encoder pin
const int encoderPinA = 2;
const int encoderPinB = 3;
int encoderPos = 0; //fist start position
// encoder value change motor turn angles
const float ratio = 360./188.611/48.;//Encoder value to motor rotation value
// 360. -> 1 turn
// 188.611 -> Gear Ratio
// 48. -> Encoder: Countable Events Per Revolution (Motor Shaft)
// testing
// P control
float Kp = 24;
float targetDeg = 0;

void doEncoderA()
{  
  encoderPos += (digitalRead(encoderPinA)==digitalRead(encoderPinB))?1:-1;
}
void doEncoderB()
{  
  encoderPos += (digitalRead(encoderPinA)==digitalRead(encoderPinB))?-1:1;
}

void doMotor(bool dir, int vel)
{
  digitalWrite(motorDirPin, dir);
  analogWrite(motorPWMPin, dir?(255 - vel):vel);
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(encoderPinA, INPUT_PULLUP);
  attachInterrupt(0, doEncoderA, CHANGE);
  
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(1, doEncoderB, CHANGE);
  
  pinMode(motorDirPin, OUTPUT);
  pinMode(EnablePin, OUTPUT);
}

void loop()
{
  
  
  // POT CALLIBRATION
 val = 500 - analogRead(potpin); // reads the value of the potentiometer (value between 0 and 1023)
                   // Angle starting from left to right side.(each segment=45 deg)  
                                   
  val = map(val, 0, 1023, 0, 360); // scale it to use it with the servo (value between 0 and 180)
  if(val < 268)
  {
    val == 1;
  }
  else if(val > 268 && val < 360)
  {
    val == -1;
  }
 
  targetDeg= val ;
  Serial.print("    Angle : ");
  Serial.print(val);
  float motorDeg = float(encoderPos)*ratio;
  //Control signal = P gain * (target value-present value)
  float error = targetDeg - motorDeg;
  float control = Kp*error;
  
  digitalWrite(EnablePin, 30);

  doMotor((control>=0)?HIGH:LOW, min(abs(control), 130)); //130 good
  
  Serial.print(" encoderPos : ");
  Serial.print(encoderPos);
  Serial.print(" ratio : ");
  Serial.print(ratio);
  Serial.print("   motorDeg : ");
  Serial.print(float(encoderPos)*ratio);
  Serial.print("   error : ");
  Serial.print(error);
  Serial.print("    control : ");
  Serial.print(control);
  Serial.print("    motorVel : ");
  Serial.println(min(abs(control), 130)); 
  //Serial.print("    dfdsf : ");
  //Serial.println(analogRead(potpin)); 
  

}
