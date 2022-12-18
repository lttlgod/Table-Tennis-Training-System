#define motorx_dirPin 2
#define motorx_stepPin 3
#define sensorPin_x 4
#define motory_dirPin 7
#define motory_stepPin 6
#define sensorPin_y 8

#define stepsPerRevolution 3200
#define delaymsec_x 85
#define delaymsec_y 75      //初速度设为100，值越小，实际速度越大
//速度为75时，最大值角度为4700

#define max_loc_x 4700
#define max_loc_y 5800

char cmd1;
int datas;
int loc_x = 0, dir_x = 1, loc_y = 0, dir_y = 1;
bool flag_x, flag_y;

void setup() {
  pinMode(motorx_stepPin, OUTPUT);
  pinMode(motorx_dirPin, OUTPUT);
  pinMode(motory_stepPin, OUTPUT);
  pinMode(motory_dirPin, OUTPUT);
  pinMode(sensorPin_x, INPUT);
  pinMode(sensorPin_y, INPUT);
  //init_x();
  Serial.begin(9600);
}

void loop(){
  if (Serial.available()){ 
    cmd1 = Serial.read();
    datas = Serial.parseInt();
    runUserCmd();
  }
}

void runUserCmd(){
  switch(cmd1){
    case 'i':
      init_x();
      break;
    case 'j':
      init_y();
      break;
    case 'x'://x2350
      run_motorx(datas);
      break;
    case 'y':
      run_motory(datas);
      break;
  }
}

void init_x(){
  digitalWrite(motorx_dirPin, LOW);//角速度向上
  for (int i = 0; i < stepsPerRevolution*3; i++) {
    digitalWrite(motorx_stepPin, HIGH);
    delayMicroseconds(delaymsec_x);
    digitalWrite(motorx_stepPin, LOW);
    delayMicroseconds(delaymsec_x);
    flag_x = digitalRead(sensorPin_x);
    if(flag_x==0) {loc_x = 0; break;}
  }
  //停下来之后慢慢回来一些
}

void run_motorx(int aim){
  if(loc_x <= aim){
    digitalWrite(motorx_dirPin, HIGH);
    dir_x = 1;
  }
  else{
    digitalWrite(motorx_dirPin, LOW);//角速度向下
    dir_x = -1;
  }
  while(loc_x != aim){
    loc_x += dir_x;
    digitalWrite(motorx_stepPin, HIGH);
    delayMicroseconds(delaymsec_x);
    digitalWrite(motorx_stepPin, LOW);
    delayMicroseconds(delaymsec_x);
    if(loc_x > max_loc_x || loc_x == 0) break;
  }
}

void init_y(){
  digitalWrite(motory_dirPin, HIGH);//向下低头
  for (int i = 0; i < stepsPerRevolution*3; i++) {
    digitalWrite(motory_stepPin, HIGH);
    delayMicroseconds(delaymsec_y);
    digitalWrite(motory_stepPin, LOW);
    delayMicroseconds(delaymsec_y);
    flag_y = digitalRead(sensorPin_y);
    if(flag_y==0) {loc_y = 0;/*Serial.println(i);*/ break;}
  }  
}

void run_motory(int aim){
  if(loc_y <= aim){
    digitalWrite(motory_dirPin, LOW);
    dir_y = 1;
  }
  else{
    digitalWrite(motory_dirPin, HIGH);
    dir_y = -1;
  }
  while(loc_y != aim){
    loc_y += dir_y;
    digitalWrite(motory_stepPin, HIGH);
    delayMicroseconds(delaymsec_y);
    digitalWrite(motory_stepPin, LOW);
    delayMicroseconds(delaymsec_y);
    if(loc_y > max_loc_y || loc_y == 0) break;
  }
}
