void motorSend(struct Motor *m){
  if(mPIDOpen){  //use PID
    if(m->setPoint == 0 && abs(m->input)<=2){ // avoid shaking.
      analogWrite(m->PWMPin, 0);
      return;
    }
    if(m->outPut >=0){
      digitalWrite(m->directionPin, m->forward);
    }else{
      digitalWrite(m->directionPin, !m->forward);
    }
    short int v = abs(m->outPut);
    if(v>=mMinV){ //this may be deleted.
      analogWrite(m->PWMPin, v);
    }else{
      analogWrite(m->PWMPin, 0);
    }
  }else{ // control directly.
    if(m->setPoint >=0){ 
      digitalWrite(m->directionPin, m->forward);
    }else{
      digitalWrite(m->directionPin, !m->forward);
    }
    analogWrite(m->PWMPin, abs(m->setPoint));
  }
}

double availableV(double v){
  double absV = abs(v);
  if(absV > mPIDMaxV){
    if(v >= 0)return mPIDMaxV;
    if(v < 0)return (-1.0d)* mPIDMaxV;
  }
  return v;
}

void computePID(struct Motor *m, unsigned long now){
  unsigned long dTime = now - m->time;
  double dInput = -1*(m->input - m->lastInput);
  double error = -1*(m->setPoint - m->input);
  m-> errorSumKi = availableV(m-> errorSumKi + m->Ki* error* dTime/1000.0d);
  m-> outPut = (int)(availableV(m->Kp* error + m->errorSumKi - m->Kd* dInput/ dTime*1000.0d)/12*255);

  m->time = now;
  m->lastInput = m->input;
}

void initMotor(struct Motor *m, short int dir, short int pwm, double p, double i, double d, short int forward){
  m->input = 0;
  m->lastInput = 0;
  m->outPut = 0;
  m->setPoint = 0;
  m->errorSumKi = 0;
  if(mP){m->Kp = p;}else{m->Kp = 0;}
  if(mI){m->Ki = i;}else{m->Ki = 0;}
  if(mD){m->Kd = d;}else{m->Kd = 0;}
  m->directionPin = dir;
  m->PWMPin = pwm;
  m->forward = forward;
  pinMode(dir, OUTPUT);
  pinMode(pwm, OUTPUT);
  analogWrite(pwm, 0);
  m->time = millis();
}
