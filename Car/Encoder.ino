void compute(struct Encoder *e, unsigned long time){ // calculate the omega and angular accelation of the motor.
  double dTheta = (double)(e->scale - e->lastScale)* 2.0d*(double)PI / (double)(mEncoderPulse * mEncoderFrequency);
  double dTime = (double)(time - e->time) / 1000.0d;
  e->alpha = (e->omega - e->lastOmega) / dTime;
  e->lastOmega = e->omega;
  e->omega = dTheta / dTime;
  e->scale = 0;
  e->lastScale = e->scale;
  e->time = time;
  changeState(e); // no use
}

void changeState(struct Encoder *e){ // no use.
  double a = abs(e->alpha);
  double w = abs(e->omega);
  if(a < 200){
    if(w < 2){
      e->STATE = 0;
    }else{
      if(abs(e->motor->setPoint - e->omega) <2){
        e->STATE = 2;
      }else{e->STATE = 3;}
    }
  }else{e->STATE = 1;}
}

void encoder11(){    // for mEncoderFrequency = 1 && A phase, Encoder1
  short int B = digitalRead(mEncoder1B);
  if(B) mEncoder1.scale += mEncoderForward1;
  if(!B) mEncoder1.scale -= mEncoderForward1;
}

void encoder1A(){    // for mEncoderFrequency = 2 or 4 && A phase, Encoder1
  int A = digitalRead(mEncoder1A);
  int B = digitalRead(mEncoder1B);
  if(A){
    if(!B) mEncoder1.scale += mEncoderForward1;
    if(B) mEncoder1.scale -= mEncoderForward1;
  }
  if(!A){
    if(B) mEncoder1.scale += mEncoderForward1;
    if(!B) mEncoder1.scale -= mEncoderForward1;
  }
}

void encoder1B(){  // for mEncoderFrequency = 4      && B phase, Encoder1
  int A = digitalRead(mEncoder1A);
  int B = digitalRead(mEncoder1B);
  if(B){
    if(A) mEncoder1.scale += mEncoderForward1;
    if(!A) mEncoder1.scale -= mEncoderForward1;
  }
  if(!B){
    if(!A) mEncoder1.scale += mEncoderForward1;
    if(A) mEncoder1.scale -= mEncoderForward1;
  }
}

void encoder21(){    // for mEncoderFrequency = 1 && A phase, Encoder2
  short int B = digitalRead(mEncoder2B);
  if(B) mEncoder2.scale += mEncoderForward2;
  if(!B) mEncoder2.scale -= mEncoderForward2;
}

void encoder2A(){    // for mEncoderFrequency = 2 or 4 && A phase, Encoder2
  int A = digitalRead(mEncoder2A);
  int B = digitalRead(mEncoder2B);
  if(A){
    if(!B) mEncoder2.scale += mEncoderForward2;
    if(B) mEncoder2.scale -= mEncoderForward2;
  }
  if(!A){
    if(B) mEncoder2.scale += mEncoderForward2;
    if(!B) mEncoder2.scale -= mEncoderForward2;
  }
}

void encoder2B(){  // for mEncoderFrequency = 4      && B phase, Encoder2
  int A = digitalRead(mEncoder2A);
  int B = digitalRead(mEncoder2B);
  if(B){
    if(A) mEncoder2.scale += mEncoderForward2;
    if(!A) mEncoder2.scale -= mEncoderForward2;
  }
  if(!B){
    if(!A) mEncoder2.scale += mEncoderForward2;
    if(A) mEncoder2.scale -= mEncoderForward2;
  }
}

void initEncoder(struct Encoder *e, short int a, short int b, struct Motor *m){
  e->scale = 0;
  e->lastScale = 0;
  e->omega = 0;
  e->lastOmega = 0;
  e->alpha = 0;
  e->time = millis();
  pinMode(a, INPUT);
  pinMode(b, INPUT);
  e->motor = m;
  e-> STATE = 0;
}
