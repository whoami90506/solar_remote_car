/*
This is what runs on the car's Arduino.
I'm not sure if there are bugs below.
Also, I'm sorry that this code is very hard to understand. XD

--- detail message ---
This page is only for constant number and global variable.
The main function is on "Main" page.

The orange side is the front of the car.
Encoder1 is connected with Motor1, and they are on the left side of the car.
Encoder2 is connected with Motor2, and they are on the right side of the car.
Ping1 is the ping on the front of the car.
Ping2 is the ping on the back of the car.

*/
const short int mEncoder1A = 22, mEncoder1B = 23;  //for encoder1
const short int mEncoder2A = 24, mEncoder2B = 25;  //for encoder2
const short int mMotorDirection1 = 4, mMotorPWM1 = 5;  //Confined by motor sheild.
const short int mMotorDirection2 = 7, mMotorPWM2 = 6;  //Confined by motor sheild.
const short int mPingTrig1 = 40, mPingEcho1 = 41;  //for Ping1
const short int mPingTrig2 = 42, mPingEcho2 = 43;  //for Ping2
const short int mLowV[] = {27}; //set GND Pin


const boolean mPIDOpen = true; //true:use PID control   false: directly control by given voltage.
const boolean mP = true;  //if P-contorl is on or not.
const boolean mI = true;  //if I-contorl is on or not.
const boolean mD = false; //if D-contorl is on or not.
const boolean mPingOpen = true; // if pings are using or not.
const boolean mBlueOpen = true; // if blurtooth control is on or not.
const boolean mEncoderPrint = false; // print the encoder data or not
const boolean mPIDPrint = true;  // print the encoder data or not
const boolean mPingPrint = false; // print the encoder data or not
const boolean mAvoidStupid = false; // 防呆功能


const short int mEncoderPulse = 600; // how many scale on the disk
const short int mEncoderFrequency = 4; // 1 or 2 or 4
const short int mEncoderForward1 = -1, mEncoderForward2 = 1; // define that go forward is positive in theta

const double mMotorK1[] = {-8.32d, -705.0d, 0.0}; // Kp, Ki, Kd
const double mMotorK2[] = {-8.32d, -705.0d, 0.0};
const short int mMotorForward1 = LOW, mMotorForward2 = LOW; // define that go forward is high in theta
const double mPIDMaxV = 12.0d, mMinV = 0; // the limit in voltage
const double mOmega = 10.0d;
const double mOmegaH = 6.0d;
const double mOmegaL = 3.0d; // The expection omega for motor
const int mV1 = 255*7/10;
const short int mPIDClock = 10; // how long do Arduino calculate PID control(ms)

const short int mPingDelay = 60; // how long is the trig high(ms)
const short int mPingClock = 100 - mPingDelay; // how long do Pings detect the distance(ms)
const short int mPingLast = mPIDClock *1000 / 2; // the max time to wait for the echo signal
const short int mCloseDistance = 30; // the distance between safe and danger.

const boolean mAutoStop = true; // the car will car when the bluetooth disconnect
const short int mBlueDelay = 20;// how long does the car send message to the controller.(ms)
const short int mBlueLost = 20; // how many messages doesn't get will be said that out of connecting.

struct Encoder{
  long scale, lastScale;
  unsigned long time;
  double omega, lastOmega;
  double alpha;
  struct Motor* motor; // the motor connected to this encoder
  short int STATE; // haven't used ever.  0:stop  1:accelerating  2:constant speed(equal setpoint)  3: constant speed(not equal setpoint)
};
struct Motor{
  short int forward; // the singal that require the car go forward
  short int directionPin, PWMPin;
  double input, lastInput, setPoint; // input = omega from encoder   setpoint: the expection we want
  short int outPut; // the voltage to the motor
  double Kp, Ki, Kd;
  double errorSumKi;
  unsigned long time;
};
struct Ping{
  short int trigPin, echoPin;
  unsigned long time;
  boolean change; //if the distance changes or not 
  double distance, lastDistance;
  short int number;// 1 : front, 2 : back
  short int state; // 1 : trig is high, 2 : compute the distance and delay
  boolean Close; // if the car is to close to barriers or not.
};

Encoder mEncoder1, mEncoder2;
Motor mMotor1, mMotor2;
Ping mPing1, mPing2;

short int mMovingState = 0;
// 0:stop, 1:W(High Speed), 2:W(Low Speed), 3:S(High Speed), 4:S(Low Speed)
// 5:A(confined by hardware), 6:D(confined by hardware), 7:WA, 8:WD, 9:SA, 10:SD

int mData[10]; // put the message from bluetooth
int mBlueAddr = 0; // calculate how many messages in a package
boolean mBlueRW = false; // reading a package or not
boolean mConnecting = false, mCarConnection = false, mControllerConnection = false;
short int mSendDataForm = 0; // 0:initmessage, 1:for user, 2:for developer
boolean mIsCrashed = false; // if the car crash in the past or not.
boolean mCrashing = false;  // if the car crash right now or not.
boolean mNewCommand = false; // if the user send the new command, use when AvoidStupid is true. 

unsigned long mCrashTime = millis(); // calculate how long does the car crash.
unsigned long mPIDTime = millis(); // calculate how long did Arduino do PID since last time.
unsigned long mBlueTime = millis(); // calculate how long did Arduino send message to controller since last time
long mBlueGetTime = millis() - mBlueDelay * mBlueLost; // calculate how long did
