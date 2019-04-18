void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();
  mControllerConnection = now - mGetTime < mDelay * mLost;
  
  if(mControllerConnection && mCarConnection && !isConnecting){ // both car and controller get the package.
    show("Notice     : Connected with the car.");
    if(mAutoStop)STATE = 0;
    mIsInit = false;
    isConnecting = true;
  }else if((!mControllerConnection || !mCarConnection) && isConnecting){ // either car or controller doesn't get package.
    show("Warning    : The car is out of connection.");
    isConnecting = false;
  }
  
  if(Serial.available()){ // user write message
    int input = Serial.read();
    if(!isConnecting)show("Warning    : The car is out of connection.");
    switch(input){
      case '5': show("Command    : Set Stop Mode.");             mNewCommand = true; STATE = 0;  break;
      case 'w': show("Command    : Set Forward(SLOW) Mode.");    mNewCommand = true; if(isForwardClose)show("Warning    : There are barriers in front of the car."); STATE = 1;  break;
      case '8': show("Command    : Set Forward(FAST) Mode.");    mNewCommand = true; if(isForwardClose)show("Warning    : There are barriers in front of the car."); STATE = 2;  break;
      case 's': show("Command    : Set Backward(SLOW) Mode.");   mNewCommand = true; if(isBackwardClose)show("Warning    : There are barriers in back of the car."); STATE = 3;  break;
      case '2': show("Command    : Set Backward(FAST) Mode.");   mNewCommand = true; if(isBackwardClose)show("Warning    : There are barriers in back of the car."); STATE = 4;  break;
      case '4': show("Command    : Set Left Mode.");             mNewCommand = true; STATE = 5;  break;
      case '6': show("Command    : Set Right Mode.");            mNewCommand = true; STATE = 6;  break;
      case '7': show("Command    : Set Forward & Left Mode.");   mNewCommand = true; if(isForwardClose)show("Warning    : There are barriers in front of the car."); STATE = 7;  break;
      case '9': show("Command    : Set Forward & Right Mode.");  mNewCommand = true; if(isForwardClose)show("Warning    : There are barriers in front of the car."); STATE = 8;  break;
      case '1': show("Command    : Set Backward & Left Mode.");  mNewCommand = true; if(isBackwardClose)show("Warning    : There are barriers in back of the car."); STATE = 9;  break;
      case '3': show("Command    : Set Backward & Right Mode."); mNewCommand = true; if(isBackwardClose)show("Warning    : There are barriers in back of the car."); STATE = 10; break;
      case 'i':
        switch(STATE){
          case 0 : show("Information: Now is in Stop Mode.");              break;
          case 1 : show("Information: Now is in Forward(SLOW) Mode.");     break;
          case 2 : show("Information: Now is in Forward(FAST) Mode.");     break;
          case 3 : show("Information: Now is in Backward(SLOW) Mode.");    break;
          case 4 : show("Information: Now is in Backward(FAST) Mode.");    break;
          case 5 : show("Information: Now is in Left Mode.");              break;
          case 6 : show("Information: Now is in Right Mode.");             break;
          case 7 : show("Information: Now is in Forward & Left Mode.");    break;
          case 8 : show("Information: Now is in Forward & Right Mode.");   break;
          case 9 : show("Information: Now is in Backward & Left Mode.");   break;
          case 10: show("Information: Now is in Backward & Right Mode.");  break;
        }
        if(isForwardClose)show("Warning    : There are barriers in front of the car.");
        if(isBackwardClose)show("Warning    : There are barriers in back of the car.");
        if(mCrashing)show("Warning    : The car has some problem.");
        if(mGetDataForm == 2){ // developer mode only
          Serial.print("Omega : ");
          Serial.print(mOmega1);
          Serial.print("  ");
          Serial.println(mOmega2);
          Serial.print("Voltage:                    "); 
          Serial.print(mOutput1);
          Serial.print("  ");
          Serial.println(mOutput2);
        }
        break;
      default: show("Notice     : Invalid Command.");
    }
  }
  if(now - mSendTime > mDelay){ // send package
    Serial1.write(254);  // start code
    Serial1.write(STATE);
    Serial1.write(mWonderForm);
    sendBoolMessage(mNewCommand);
    sendBoolMessage(mControllerConnection);
    Serial1.write(255); // end code
    mNewCommand = false;
    mSendTime = now;
  }
  if(Serial1.available()){ // get package from car
    mGetTime = now;
    int input = Serial1.read();
    switch(input){
    case 254: // start code
      mRW = true;
      mAddr = 0;
      break;
    case 255: // end code
      mRW = false;
      analyze();
      break;
    default: // read messages
      if(mRW){
        mData[mAddr] = input;
        mAddr++;
      }
    }
  }
}

void sendBoolMessage(boolean b){
  if(b){
    Serial1.write(1);
  }else{
    int a = 0;
    Serial1.write(a);
  }
}
