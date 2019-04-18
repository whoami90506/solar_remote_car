/*
This is what runs on the controller Arduino.
I'm not sure if there are bugs below.
Also, I'm sorry that this code is very hard to understand. XD

--- detail message ---
This page is only for constant number and global variable.
The main function is on "Main" page.

how to control the car:
7 8 9   WA  W1     WD 
4 5 6   A   Stop   D
1 2 3   SA  S1     SD
W: W2, S: S2, i: information
*/
const short int mDelay = 20; // how mamy ms did Arduino send message to the car.
const short int mLost = 20; // how many messages are lost will be defined that the car is out of connection.
const short int mGetDataForm = 1; //1: for user, 2:for developer

unsigned long mSendTime = millis(); // how long has Arduino send package to car since last time.
long mGetTime = millis() - mDelay * mLost;// how long has Arduino get package from car since last time.
boolean isConnecting = false, mCarConnection = false, mControllerConnection = false;
boolean mNewCommand = false; // if the command is new or not.

int mData[100]; // where place the package got from car.
boolean mRW = false; //if reading the package or not.
int mAddr = 0; // how many messages do Arduino get.

boolean isForwardClose = false, isBackwardClose = false;
boolean mAutoStop = true; // if the car stop when bluetooth is out of connection or not.
boolean mIsInit = false; // if Arduino has been initial or not
int mOmega1 = 0, mOmega2 = 0; // Only working when developer mode
int mOutput1 = 0,mOutput2 = 0; // voltage to the motor. Only working when developer mode
int mSetPoint1 = 0, mSetPoint2 = 0;// Only working when developer mode
boolean mCrashing = false;// if the car crash or not.

short int STATE = 0;
// 0:stop, 1:W(High Speed), 2:W(Low Speed), 3:S(High Speed), 4:S(Low Speed)
// 5:A(confined by hardware), 6:D(confined by hardware), 7:WA, 8:WD, 9:SA, 10:SD

short int mWonderForm = 0; // the package form we need now. After init, it will change to  mGetFormData.
