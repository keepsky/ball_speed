#include <Wire.h> // arduino IDE에서 해당 library 추가 필요
//#include <LiquidCrystal_I2C_Hangul.h> // arduino IDE에서 해당 library 추가 필요
#include <LiquidCrystal_I2C.h> // arduino IDE에서 해당 library 추가 필요

#define DEBUG    
//#define TRACE

#define SLEEP_DURATION 5
#define CYCLE_DURATION  (1000/SLEEP_DURATION)

#define TRIG 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 8 //ECHO 핀 설정 (초음파 받는 핀)

//LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() 
{
  Serial.begin(115200);
  while(!Serial)
  {
    delay(10);
  }

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  lcd.init();
  lcd.backlight();

  Serial.println("setup ok");
  
  lcd.setCursor(0,0);
  lcd.print("START");
}

void loop()
{
  long duration, tbase, tnow;
  double tdiff, dist_diff, distance, dist_now;
  double speed, max_speed=0;
  int forward, first=1;

  int index=0;
  double sv[100], sum_speed, avr_speed;
  int empty=0;
  int cycle=0;

#if 1
  while(1)
  {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    tbase = pulseIn (ECHO, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.

    delayMicroseconds(2000);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    tnow = pulseIn (ECHO, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.

    distance = ((double)(340*tbase)/1000)/2; 
    dist_now = ((double)(340*tnow)/1000)/2; 

    tdiff = (double)(tnow - tbase)/(double)1000000;   // sec

    dist_diff = (dist_now - distance)/1000;  
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

    if(speed > 5)
    {
      lcd.setCursor(0,0);
      lcd.print("AVR : ");
      lcd.print(speed);
      lcd.print(" m/s  ");
    }
#ifdef DEBUG      
      Serial.print("dist_diff : ");
      Serial.println(dist_diff);
      Serial.print("tdiff : ");
      Serial.println(tdiff);

      Serial.print("speed : ");
      Serial.print(speed);
      Serial.println(" m/s");      
#endif

    delay(10);
    continue;

#ifdef TRACE
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
#ifdef DEBUG      
      Serial.print("speed : ");
      if(forward)
        Serial.print("(+) ");
      else
        Serial.print("(-) ");
      Serial.print(speed);
      Serial.println(" m/s");      
#endif
      if(index < 100)
        sv[index++]=speed;
    }
    else if(empty++ > 5)
    {
      empty = 0;
      if(index > 0)
      {
        sum_speed = 0;
        for(int i=0;i<index;i++)
        {
          sum_speed += sv[i];
        }        
        avr_speed = sum_speed / index;

        if(cycle > CYCLE_DURATION)
        {
          max_speed = 0;
          cycle = 0;
        }

        if(avr_speed > 2)
        {
#ifdef DEBUG      
          Serial.print("avr speed : ");
          Serial.print(avr_speed);
          Serial.println(" m/s");      
          //Serial.println(index);      
#endif
          if(avr_speed > max_speed)
          {
            max_speed = avr_speed;
          }
          lcd.setCursor(0,0);
          lcd.print("AVR : ");
          lcd.print(avr_speed);
          lcd.print(" m/s  ");
          lcd.setCursor(0,1);
          lcd.print("MAX : ");
          lcd.print(max_speed);
          lcd.print(" m/s  ");

        }
        index = 0;

      }
    }

    //delayMicroseconds(100);
    cycle++;
    delay(SLEEP_DURATION); //1초마다 측정값을 보여줍니다.

  }
#endif

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

    if(first)
    {
      first = 0;
      delay(SLEEP_DURATION);
      continue;
    }
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

#ifdef TRACE
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
#ifdef DEBUG      
      Serial.print("speed : ");
      if(forward)
        Serial.print("(+) ");
      else
        Serial.print("(-) ");
      Serial.print(speed);
      Serial.println(" m/s");      
#endif
      if(index < 100)
        sv[index++]=speed;
    }
    else if(empty++ > 5)
    {
      empty = 0;
      if(index > 0)
      {
        sum_speed = 0;
        for(int i=0;i<index;i++)
        {
          sum_speed += sv[i];
        }        
        avr_speed = sum_speed / index;

        if(cycle > CYCLE_DURATION)
        {
          max_speed = 0;
          cycle = 0;
        }

        if(avr_speed > 2)
        {
#ifdef DEBUG      
          Serial.print("avr speed : ");
          Serial.print(avr_speed);
          Serial.println(" m/s");      
          //Serial.println(index);      
#endif
          if(avr_speed > max_speed)
          {
            max_speed = avr_speed;
          }
          lcd.setCursor(0,0);
          lcd.print("AVR : ");
          lcd.print(avr_speed);
          lcd.print(" m/s  ");
          lcd.setCursor(0,1);
          lcd.print("MAX : ");
          lcd.print(max_speed);
          lcd.print(" m/s  ");

        }
        index = 0;

      }
    }

    //delayMicroseconds(100);
    cycle++;
    delay(SLEEP_DURATION); //1초마다 측정값을 보여줍니다.
  }
}
