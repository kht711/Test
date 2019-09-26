#include <Stepper.h>

//2048->360 deg, 1024->180 deg
const int steps = 2048;

//밀리초 기준
unsigned int times = 2 * 3600 * 1000;

Stepper myStepper(steps, 11, 9, 10, 8);

void setup() {
  /*
  모터 스텝을 2048로 설정했을 때
  안정적으로 동작하는 최대 속도는 17.
   */
  myStepper.setSpeed(15);  //15 RPM
}

void loop() {
  myStepper.step(steps);
  delay(times);

  myStepper.step(-steps);
  delay(times);
}
