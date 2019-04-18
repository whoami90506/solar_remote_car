void pingActivity(struct Ping *p, unsigned long now){
  unsigned long dTime = now - p->time;
  switch(p->state){
    case 1: // compute distance and delay.
      if(dTime >= mPingDelay){
        digitalWrite(p->trigPin, LOW);
        p-> lastDistance = p-> distance;
        double duration = pulseIn(p->echoPin, HIGH, mPingLast);
        p-> distance = (duration/2.0d)/29.0d;
        p-> change = true;
        CloseActivity(p);
        p-> state = 2;
        p-> time = now;
      }
      break;
      
    case 2: // trig is high.
      if(dTime >= mPingClock){
        digitalWrite(p->trigPin, HIGH);
        p-> state = 1;
        p-> time = now;
      }
      break;
  }
}

void CloseActivity(struct Ping *p){
  if(isClose(p) && !p->Close){
    if(p->number==1 && (mMovingState == 1 || mMovingState == 2 || mMovingState == 7 || mMovingState == 8 )) Stop(false); // forward is to close && W1,W2,WA,WD
    if(p->number==2 && (mMovingState == 3 || mMovingState == 4 || mMovingState == 9 || mMovingState == 10)) Stop(false);  // back is to close && S1,S2,SA,SD
  }
  if(!isClose(p) && p->Close)setMotor(mMovingState); // return orginal command when no more barriers.
  p->Close = isClose(p);
}

void initPing(struct Ping *p, short int trig, short int echo , short int no){
  p->trigPin = trig;
  p->echoPin = echo;
  p-> state = 1;
  p-> distance = -1;
  p-> lastDistance = -1;
  p-> change = false;
  p->number = no;
  p->Close = false;
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  p-> time = millis();
  digitalWrite(trig, HIGH);
}

boolean isClose(struct Ping *p){
  if( (p->distance<=mCloseDistance && p->distance>0)  &&  (p->lastDistance<=mCloseDistance && p->lastDistance>0) )return true; // the distance is too close two times.
  return false;
}


