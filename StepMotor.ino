#include <Stepper.h>
#include <Arduino.h>
#include <TM1637Display.h>
#include <DS3231M.h>

//2048->360 deg, 1024->180 deg
const int steps = 2048;

/* IN1->pin8
 * IN2->pin9
 * IN3->pin10
 * IN4->pin11
 */
Stepper myStepper1(steps, 11, 9, 10, 8);
Stepper myStepper2(steps, 7, 5, 6, 4);

void Step(int count){
  int i, j;
  int s = 1;
  int c = count;
  if (c < 0){
    s = -1;
    c = -count;
  }
  for (i = 0; i < c; i++){
    for (j = 0; j < steps; j++){
      myStepper1.step(s);
      myStepper2.step(s);
    }
  }
}
//-------------------------------------------------

//TM1637 모듈에 있는 핀 CLK, DIO를 디지털 핀 3, 2로 설정
#define CLK 3
#define DIO 2

//TM1637 관련 변수들
unsigned long s;
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };

//타이머 변수. 시, 분, 초 관리.
int hour;
int minute;
int second;

//설정하고 싶은 시간 설정
#define HOUR 1
#define MINUTE 58
#define SECOND 52

TM1637Display display(CLK, DIO);

//-------------------------------------------------

/*
SDA -> A4
SCL -> A5
*/
DS3231M_Class DS3231M;

//-------------------------------------------------

void TimerInit(){
  hour = HOUR;
  minute = MINUTE;
  second = SECOND;
  data[0] = display.encodeDigit(hour / 10);
  data[1] = 0x80 + display.encodeDigit(hour % 10);
  data[2] = display.encodeDigit(minute / 10);
  data[3] = display.encodeDigit(minute % 10);

  //밝기 조정. 1부터 7까지 설정 가능. 7이 최대 밝기
  display.setBrightness(0x07);
  display.setSegments(data);
  s = millis();
}

void TimerDisplay(){
  //디스플레이 초기화
  TimerInit();

  //코드 경과 시간 측정 시작
  unsigned int start = millis();
  
  //무한 반복
  while(1){
    static uint8_t secs;
    DateTime now = DS3231M.now();
    //hour가 0, 그리고 minute가 0이 되는 조건이면
    //타이머 화면 꺼지면서 함수 빠져나오기
    if (hour == 0 && minute == 0){
      display.setBrightness(0x07, false);
      display.setSegments(data);
      return;
    }
    
    //콜론 OFF
    if (millis() - s > 500){
      data[1] = display.encodeDigit(hour % 10);
    }

    //1초 되었을 때
    if (secs != now.second()){
      secs = now.second();
      Serial.println(String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
      data[1] = 0x80 + display.encodeDigit(hour % 10);
      second--;
      s = millis();
    }
    
    // 초가 0이하가 되었을 때
    if (second < 0){
      second = 59;
      if (minute == 0){
        minute = 59;
        hour--;
      }else{
        minute--;
      }
      data[0] = display.encodeDigit(hour / 10);
      data[1] = 0x80 + display.encodeDigit(hour % 10);
      data[2] = display.encodeDigit(minute / 10);
      data[3] = display.encodeDigit(minute % 10);

      //코드 경과 측정 종료 (Ctrl + Shift + M으로 확인 가능)
      Serial.println(millis() - start);
    }
    display.setSegments(data);
  }
}

//-------------------------------------------------

void setup() {
  delay(15000UL);
  //step() 함수를 실행할 때 스피드 설정
  myStepper1.setSpeed(15);  //15 RPM
  myStepper2.setSpeed(15);
  Serial.begin(9600);
  DS3231M.begin();
  DS3231M.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  // 반시계방향
  Step(15);
  TimerDisplay();

  // 시계방향
  Step(-15);
  TimerDisplay();
}
