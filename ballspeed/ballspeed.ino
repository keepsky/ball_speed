#define TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 8 //ECHO 핀 설정 (초음파 받는 핀)

void setup() 
{
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop()
{
  long duration, tbase, tnow;
  double speed, tdiff, dist_diff, distance, dist_now;
  int forward;

  while(1)
  {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn (ECHO, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.
    //34000*초음파가 물체로 부터 반사되어 돌아오는시간 /1000000 / 2(왕복값이아니라 편도값이기때문에 나누기2를 해줍니다.)
    //초음파센서의 거리값이 위 계산값과 동일하게 Cm로 환산되는 계산공식 입니다. 수식이 간단해지도록 적용했습니다.

    //tbase = millis()/500;
    tnow = micros();
    tdiff = (double)(tnow - tbase)/1000000;   // sec
    tbase = tnow;

    dist_now = ((double)(340*duration)/1000)/2; 
    //PC모니터로 초음파 거리값을 확인 하는 코드 입니다.
    dist_diff = (dist_now - distance)/1000;    
    distance = dist_now;

    speed = dist_diff/tdiff;    // m/s

    if(speed < 0)
    {
      speed = speed * (-1);
      forward = 0;
    }
    else
    {
      forward = 1;
    }

//#define DEBUG    
#ifdef DEBUG
    //Serial.print("Duration : ");
    //Serial.println(duration); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
    Serial.print("time : ");
    Serial.println(tdiff); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
    Serial.print("Distance : ");
    Serial.println(dist_diff); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
    Serial.print("speed : ");
    Serial.print(speed);
    Serial.println(" m/s");
#endif
    if(speed > 2)
    {
      Serial.print("speed : ");
      if(forward)
        Serial.print("(+) ");
      else
        Serial.print("(-) ");
      Serial.print(speed);
      Serial.println(" m/s");      
    }
    //delay(10); //1초마다 측정값을 보여줍니다.
  }
}
