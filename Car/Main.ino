void setup() {
  // put your setup code here, to run once:
  for(int i=0; i< sizeof(mLowV)/sizeof(short int);i++){ //set GND. no use now.
    pinMode(mLowV[i], OUTPUT);
    digitalWrite(mLowV[i], LOW);
  }
  Serial.begin(115200);
  if(mBlueOpen)Serial1.begin(115200);
  initEncoder(&mEncoder1, mEncoder1A, mEncoder1B, &mMotor1);
  initEncoder(&mEncoder2, mEncoder2A, mEncoder2B, &mMotor1);
  if(mEncoderFrequency == 1){                                             // for mEncoderFrequency = 1 && A phase
    attachInterrupt(digitalPinToInterrupt(mEncoder1A),encoder11,FALLING);
    attachInterrupt(digitalPinToInterrupt(mEncoder2A),encoder21,FALLING);
  } 
  if(mEncoderFrequency == 2 || mEncoderFrequency == 4){                   // for mEncoderFrequency = 2 or 4 && A phase
    attachInterrupt(digitalPinToInterrupt(mEncoder1A),encoder1A,CHANGE);
    attachInterrupt(digitalPinToInterrupt(mEncoder2A),encoder2A,CHANGE);
  }  
  if(mEncoderFrequency == 4){                                             // for mEncoderFrequency = 4      && B phase
    attachInterrupt(digitalPinToInterrupt(mEncoder1B),encoder1B,CHANGE);
    attachInterrupt(digitalPinToInterrupt(mEncoder2B),encoder2B,CHANGE);
  }
  if(mEncoderPrint)Serial.println("Encoder init");

  initMotor(&mMotor1, mMotorDirection1, mMotorPWM1, mMotorK1[0], mMotorK1[1], mMotorK1[2], mMotorForward1);
  initMotor(&mMotor2, mMotorDirection2, mMotorPWM2, mMotorK2[0], mMotorK2[1], mMotorK2[2], mMotorForward2);
  if(mPingOpen){
    initPing(&mPing1, mPingTrig1, mPingEcho1, 1);
    initPing(&mPing2, mPingTrig2, mPingEcho2, 2);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();
  boolean dataPrint =false; // need to print the data again or not.
  if(mBlueOpen){
    if(now - mBlueTime >=mBlueDelay){ // send message every (mBlueDelay) ms.
      sendMessage();
      mBlueTime = now;
    }
    if(Serial1.available())getMessage(); // receive message.
    mCarConnection = isConnecting();
    if((!mCarConnection || !mControllerConnection) && mConnecting && mAutoStop){ //either controller or car doesn't get message
      Stop(true);
      mConnecting = false;
    }else if(mCarConnection && mControllerConnection && !mConnecting){ // both controller and car are connecting.
      mSendDataForm = 0;
      sendMessage();
      mConnecting = true;
    }
  }
   
  if(Serial.available()>0){ 
    int inByte = Serial.read();
    switch(inByte){// not working when bluetooth is open( I don't know why).
      case 'W':  W2();  break;       
      case 'S':  S2();  break;        
      case 'A':  A();   break;        
      case 'D':  D();   break;        
      case 'E':  Stop(true);break;
    }
  }
  if(now - mPIDTime >= mPIDClock){ // calculate PID every (mPIDClock) ms.
    compute( &mEncoder1, now);
    compute( &mEncoder2, now);
    if(mEncoderPrint) dataPrint = true;
    if(mPIDOpen){
      mMotor1.input = mEncoder1.omega;
      mMotor2.input = mEncoder2.omega;
      computePID(&mMotor1, now);
      computePID(&mMotor2, now);
      if(mPIDPrint)dataPrint = true;
      motorSend(&mMotor1); //send the voltage to the motor.
      motorSend(&mMotor2);
    }
    mPIDTime = now;
  }
  
  if(mPingOpen){ // detact the distance to barriers
    pingActivity(&mPing1, now);
    pingActivity(&mPing2, now);
    if(mPingPrint){
      if(mPing1.change){
        mPing1.change = false;
        dataPrint = true;
      }
      if(mPing2.change){
        mPing2.change = false;
        dataPrint = true;
      }
    }
  }
  
  if(mAvoidStupid){
    boolean flag = false;
    if(mPIDOpen){
      if(abs(mMotor1.setPoint)>0 && abs(mEncoder1.omega)<3)flag = true;
      if(abs(mMotor2.setPoint)>0 && abs(mEncoder2.omega)<3)flag = true;
    }else{
      if(abs(mMotor1.setPoint)>0 && abs(mEncoder1.omega)<3)flag = true;
      if(abs(mMotor2.setPoint)>0 && abs(mEncoder1.omega)<3)flag = true;
    }
    if(flag){
      if(!mIsCrashed){
        mIsCrashed = true;
        mCrashTime = millis();
      }
      if(mIsCrashed && millis() - mCrashTime > 200){ // if car crashes over 200ms.
        mCrashing = true;
        Stop(true);
      }
    }else{
      if(mIsCrashed && millis() - mCrashTime <200)mIsCrashed = false;
    }
  }
  
  if(dataPrint){ //if data need to update.
      Serial.print("Time(ms) : ");
      Serial.print(now);
      if(mEncoderPrint){
        Serial.print("  Encoder1: ");
        Serial.print(mEncoder1.scale);
        Serial.print(" ");
        Serial.print(mEncoder1.omega);
        Serial.print("  Encoder2: ");
        Serial.print(mEncoder2.scale);
        Serial.print(" ");
        Serial.print(mEncoder2.omega);
      }
      if(mPIDPrint){
        Serial.print("  PID V1: ");
        Serial.print(mMotor1.outPut);
        Serial.print("  PID V2: ");
        Serial.print(mMotor2.outPut);
      }
      if(mPingPrint){
        Serial.print("  Ping1(cm) : ");
        Serial.print(mPing1.distance);
        Serial.print("  Ping2(cm) : ");
        Serial.print(mPing2.distance);
      }
      Serial.print("\n");
    }
}
