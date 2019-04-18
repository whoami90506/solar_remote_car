/*
package form 0 (init):      {0, isPIDOpen, isPingOpen, isAutoStopOpen, isAvoidStupidOpen}
package form 1 (user):      {1, isPing1CLose, isPing2Close, isCarCrashing, isCarGetPackage}
package form 2 (developer): {2, isPing1CLose, isPing2Close, isCarCrashing, isCarGetPackage, Omega1, Omega2, Voltage1(2 bytes), Voltage2(2bytes)}
*/
void analyze(){
  if(mAddr >0){
    switch(mData[0]){
      case 0: // init
      if(mAddr ==  5 && !mIsInit){
        show("Initial    :");
        showBool(mData[1], "PID control");
        showBool(mData[2], "Ping");
        showBool(mData[3], "Auto stop function");
        showBool(mData[4], "Avoid crash function");
        mAutoStop = mData[3];
        mWonderForm = mGetDataForm;
        mIsInit = true;
        break;
        
      case 1: // for arduino controller
        if(mAddr == 5){
          if(mData[1] == 0 || mData[1] == 1){
            if(mData[1] && !isForwardClose)show("Warning    : There are barriers in front of the car.");
            if(!mData[1] && isForwardClose)show("Notice     : There is no barrier anymore in front of the car.");
            isForwardClose = mData[1];
          }
          if(mData[2] == 0 || mData[2] == 1){
            if(mData[2] && !isBackwardClose)show("Warning    : There are barriers in back of the car.");
            if(!mData[2] && isBackwardClose)show("Notice     : There is no barrier anymore in back of the car.");
            isBackwardClose = mData[2];
          }
          if(mData[3] == 0 || mData[3] == 1){
            if(mData[3] && !mCrashing){
              show("Warning    : The car has some problem.");
              STATE = 0;
            }
            mCrashing = mData[3];
          }
          if(mData[4] == 0 || mData[4] == 1)mCarConnection = mData[4];
        }
        break;
        
      case 2: // for developer
        if(mAddr == 11){
          if(mData[1] == 0 || mData[1] == 1){
            if(mData[1] && !isForwardClose)show("Warning    : There are barriers in front of the car.");
            if(!mData[1] && isForwardClose)show("Notice     : There is no barrier anymore in front of the car.");
            isForwardClose = mData[1];
          }
          if(mData[2] == 0 || mData[2] == 1){
            if(mData[2] && !isBackwardClose)show("Warning    : There are barriers in back of the car.");
            if(!mData[2] && isBackwardClose)show("Notice     : There is no barrier anymore in back of the car.");
            isBackwardClose = mData[2];
          }
          if(mData[3] == 0 || mData[3] == 1){
            if(mData[3] && !mCrashing){
              show("Warning    : The car has some problem.");
              STATE = 0;
            }
            mCrashing = mData[3];
          }
          if(mData[4] == 0 || mData[4] == 1)mCarConnection = mData[4];
          int w1,w2;
          if(mData[5] > 40){
            w1 = -1 * (mData[5] - 40);
          }else{
            w1 = mData[5];
          }
          if(mData[6] > 40){
            w2 = -1 * (mData[6] - 40);
          }else{
            w2 = mData[6];
          }
          if(mOmega1 != w1 || mOmega2 != w2){ // update the data
            mOmega1 = w1;
            mOmega2 = w2;
            Serial.print("Omega : "); 
            Serial.print(mOmega1);
            Serial.print("  ");
            Serial.println(mOmega2);
          }
          int o1 = decodeNum(mData[7], mData[8]);
          int o2 = decodeNum(mData[9], mData[10]);
          if(mOutput1 != o1 || mOutput2 != o2){ // update the data
            mOutput1 = o1;
            mOutput2 = o2;
            Serial.print("Voltage:                    "); 
            Serial.print(mOutput1);
            Serial.print("  ");
            Serial.println(mOutput2);
          }
        }
      }    
    }
  }
}

void show(String s){ // show the time and message.
  Serial.print("Time(s) : ");
  int time = millis()/1000;
  if(time<10)Serial.print("000");
  if(time>=10 && time <100)Serial.print("00");
  if(time>=100 && time < 1000)Serial.print("0");
  Serial.print(time);
  Serial.print(", ");
  Serial.println(s);
}
  
void showBool(boolean b, String s){ //show boolean message.
  Serial.print("                             ");
  Serial.print(s);
  if(b){
    Serial.println(" is on.");
  }else{
    Serial.println(" is off.");
  }
}

int decodeNum(int a, int b){ // decode voltage message. Only work on developer mode.
  switch(a){
    case 1: return 250 + b; break;
    case 2: return b;       break;
    case 3: return -250 - b;break;
    case 4: return -b;  
  }
}  
