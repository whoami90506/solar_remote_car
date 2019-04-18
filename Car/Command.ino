void setMotor(int state){
  if(mNewCommand || !mCrashing){
    switch(state){
      case '0': case 0 : Stop(true); break;
      case '1': case 1 : W1();       break;
      case '2': case 2 : W2();       break;
      case '3': case 3 : S1();       break;
      case '4': case 4 : S2();       break;
      case '5': case 5 : A();        break;
      case '6': case 6 : D();        break;
      case '7': case 7 : WA();       break;
      case '8': case 8 : WD();       break;
      case '9': case 9 : SA();       break;
      case 'A': case 10: Sd();       break;
    }
  }
  if(mNewCommand){ // new command says that the car doesn;t crash anymore.
    mCrashing = false;
    mIsCrashed = false;
  }
}

void W1(){
  if(!mPingOpen || !isClose(&mPing1)){  
    if(mPIDOpen){
      mMotor1.setPoint = mOmegaL;
      mMotor2.setPoint = mOmegaL;
    }else{
      mMotor1.setPoint = mV1;
      mMotor2.setPoint = mV1;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 1;
}

void W2(){     
  if(!mPingOpen || !isClose(&mPing1)){  
    if(mPIDOpen){
      mMotor1.setPoint = mOmega;
      mMotor2.setPoint = mOmega;
    }else{
      mMotor1.setPoint = 255;
      mMotor2.setPoint = 255;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 2;
}

void S1(){    
  if(!mPingOpen || !isClose(&mPing2)){  
    if(mPIDOpen){
      mMotor1.setPoint = -1 * mOmegaL;
      mMotor2.setPoint = -1 * mOmegaL;
    }else{
      mMotor1.setPoint = -1 * mV1;
      mMotor2.setPoint = -1 * mV1;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 3;
}

void S2(){     
  if(!mPingOpen || !isClose(&mPing2)){  
    if(mPIDOpen){
      mMotor1.setPoint = -1 * mOmega;
      mMotor2.setPoint = -1 * mOmega;
    }else{
      mMotor1.setPoint = -255;
      mMotor2.setPoint = -255;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 4;
}

void A(){    
  if(mPIDOpen){
    mMotor1.setPoint = 0;
    mMotor2.setPoint = mOmegaL;
  }else{
    mMotor1.setPoint = mV1;
    mMotor2.setPoint = 255;
    motorSend(&mMotor1);
    motorSend(&mMotor2);
  }
  mMovingState = 5;
}

void D(){    
  if(mPIDOpen){
    mMotor1.setPoint = mOmegaL;
    mMotor2.setPoint = 0;
  }else{
    mMotor1.setPoint = 255;
    mMotor2.setPoint = 0;
    motorSend(&mMotor1);
    motorSend(&mMotor2);
  }
  mMovingState = 6;
}

void WA(){
  if(!mPingOpen || !isClose(&mPing1)){  
    if(mPIDOpen){
      mMotor1.setPoint = mOmegaL;
      mMotor2.setPoint = mOmegaH;
    }else{
      mMotor1.setPoint = mV1;
      mMotor2.setPoint = 255;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 7;
}

void WD(){ 
if(!mPingOpen || !isClose(&mPing1)){  
    if(mPIDOpen){
      mMotor1.setPoint = mOmegaH;
      mMotor2.setPoint = mOmegaL;
    }else{
      mMotor1.setPoint = 255;
      mMotor2.setPoint = mV1;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 8;
}

void SA(){
  if(!mPingOpen || !isClose(&mPing2)){  
    if(mPIDOpen){
      mMotor1.setPoint = -1 * mOmegaL;
      mMotor2.setPoint = -1 * mOmegaH;
    }else{
      mMotor1.setPoint = -1 * mV1;
      mMotor2.setPoint = -255;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 9;
}

void Sd(){ 
if(!mPingOpen || !isClose(&mPing2)){  
    if(mPIDOpen){
      mMotor1.setPoint = -1 * mOmegaH;
      mMotor2.setPoint = -1 * mOmegaL;
    }else{
      mMotor1.setPoint = -255;
      mMotor2.setPoint = -1 * mV1;
      motorSend(&mMotor1);
      motorSend(&mMotor2);
    }
  }else{Stop(false);}
  mMovingState = 10;
}

void Stop(boolean setState){ // setState:  true: stop until get other commands. false: stop until the pings say that there are no barriers anymore.
if(mPIDOpen){
    mMotor1.setPoint = 0;
    mMotor2.setPoint = 0;
  }else{
    mMotor1.setPoint = 0;
    mMotor2.setPoint = 0;
    motorSend(&mMotor1);
    motorSend(&mMotor2);
  }
  if(setState)mMovingState = 0;
}
