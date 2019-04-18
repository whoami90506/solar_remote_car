void sendMessage(){ //send message to the controller.
  Serial1.write(254); // start code
  Serial1.write(mSendDataForm);
  switch(mSendDataForm){
    case 0: // init
      sendBoolMessage(mPIDOpen);
      sendBoolMessage(mPingOpen);
      sendBoolMessage(mAutoStop);
      sendBoolMessage(mAvoidStupid);
      break;
      
    case 1: // for arduibo controller
      sendBoolMessage(mPing1.Close);
      sendBoolMessage(mPing2.Close);
      sendBoolMessage(mCrashing);
      sendBoolMessage(mCarConnection);
      break;
      
    case 2: // for developer
     sendBoolMessage(mPing1.Close);
     sendBoolMessage(mPing2.Close);
     sendBoolMessage(mCrashing);
     sendBoolMessage(mCarConnection);
     sendOmega(mEncoder1.omega);
     sendOmega(mEncoder2.omega);
     sendNum(mMotor1.outPut);
     sendNum(mMotor2.outPut);
     break;
      
    default: // in case error
      mSendDataForm = 1;
      break;
  }
  Serial1.write(255); // end code
}

void getMessage(){ // receive the message
  mBlueGetTime = millis();
  int input = Serial1.read();
  switch(input){
    case 254:
      mBlueRW = true;
      mBlueAddr = 0;
      break;
    case 255:
      mBlueRW = false;
      dataAnalyze();
      break;
    default:
      if(mBlueRW){
        mData[mBlueAddr] = input;
        mBlueAddr++;
      }
  }
}

void dataAnalyze(){ // package = {command, Data form that controller need, new command or not, if contorller get message or not}
  if(mBlueAddr ==4){
    mNewCommand = mData[2];
    setMotor(mData[0]);
    mSendDataForm = mData[1];
    mControllerConnection = mData[3];
  }
}

boolean isConnecting(){
  if(millis() - mBlueGetTime >= mBlueDelay * mBlueLost)return false;
  return true;
}

void sendBoolMessage(boolean b){
  if(b){
    Serial1.write(1);
  }else{
    int a = 0;
    Serial1.write(a);
  }
}

void sendOmega(double wd){ // developer form only. notice: I assume that the omega won't above 40.
  int w = (int)wd;
  if(w>=0){
    Serial1.write(w);
  }else{
    Serial1.write(abs(w) + 40);
  }
}

void sendNum(int i){ // developer form only. To the the PID voltage.
  if(i>250){
    Serial1.write(1);
    Serial1.write(i%250);
  }
  if(i>=0 && i<=250){
    Serial1.write(2);
    Serial1.write(i);
  }
  if(i<-250){
    Serial1.write(3);
    Serial1.write(abs(i)%250);
  }
  if(i>=-250 && i<0){
    Serial1.write(4);
    Serial1.write(abs(i));
  }
}
