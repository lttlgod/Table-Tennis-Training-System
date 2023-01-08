#define motorx_dirPin 2
#define motorx_stepPin 3
#define sensorPin_x 4

#define motory_dirPin 7
#define motory_stepPin 6
#define sensorPin_y 8

#define enA 9; //上
#define in1 16;
#define in2 17;

#define enB 10; //下
#define in3 14;
#define in4 15;

#define enC 11; //bottom
#define in5 12;
#define in6 13;

#define stepsPerRevolution 3200
#define delaymsec_x 85
#define delaymsec_y 75      //初速度设为，值越小，实际速度越大

#define max_loc_x 4700
#define max_loc_y 5800

int desired_pos_x, pos_x, dir_x, desired_pos_y, pos_y, dir_y;
int prev_time_x = millis(), prev_time_y = millis(), current_time_x, current_time_y;

bool feed_next_flag = 0, feed_motor_on = 0;

char cmd;
int datas;

void setup() {
  pinMode(motorx_stepPin, OUTPUT);
  pinMode(motorx_dirPin, OUTPUT);
  pinMode(motory_stepPin, OUTPUT);
  pinMode(motory_dirPin, OUTPUT);
  pinMode(sensorPin_x, INPUT);
  pinMode(sensorPin_y, INPUT);

  // Set all the DC motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);

  init_x();
  init_y();

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){ 
    cmd = Serial.read();
    datas = Serial.parseInt();
    runUserCmd();
  }
  //x movement
  if(desired_pos_x != pos_x){
    dir_x = calcDir(desired_pos_x, pos_x);
    current_time_x = millis();
    if(current_time_x - prev_time_x > delaymsec_x){
        run_motorx();
        pos_x = pos_x + dir_x;
        prev_time_x = current_time_x;
    }
    if(digitalRead(sensorPin_x) == 0){ //reach home
        pos_x = 0;
        desired_pos_x = 0;
    }
  }
  //y movement
  if(desired_pos_y != pos_y){
    dir_y = calcDir(desired_pos_y, pos_y);
    current_time_y = millis();
    if(current_time_y - prev_time_y > delaymsec_y){
        run_motory();
        pos_y = pos_y + dir_y;
        prev_time_y = current_time_y;
    }
    if(digitalRead(sensorPin_y) == 0){ //reach home
        pos_y = 0;
        desired_pos_y = 0;
    }
  }
  //feed
  if(desired_pos_x == pos_x && desired_pos_y == pos_y && feed_next_flag){
    //unfinished
  }
}

void runUserCmd(){
  switch(cmd){
    case 'i':
      init_x();
      break;
    case 'j':
      init_y();
      break;
    case 'x'://e.g. x2350
      if(datas <= max_loc_x) desired_pos_x = datas;
      break;
    case 'y':
      if(datas <= max_loc_y) desired_pos_y = datas;
      break;
    case 'f'://feed
      feed_next_flag = true;
      break;
  }
}

int calcDir(int desired_pos; int current_pos){
  return (desired_pos > current_pos) ? 1 : -1;
}

void init_x(){
  digitalWrite(motorx_dirPin, LOW);//角速度向上
  for (int i = 0; i < stepsPerRevolution*3; i++) {
    digitalWrite(motorx_stepPin, HIGH);
    delayMicroseconds(delaymsec_x);
    digitalWrite(motorx_stepPin, LOW);
    delayMicroseconds(delaymsec_x);
    if(digitalRead(sensorPin_x)==0) {pos_x = 0; break;}
  }
}

void init_y(){
  digitalWrite(motory_dirPin, HIGH);//向下低头
  for (int i = 0; i < stepsPerRevolution*3; i++) {
    digitalWrite(motory_stepPin, HIGH);
    delayMicroseconds(delaymsec_y);
    digitalWrite(motory_stepPin, LOW);
    delayMicroseconds(delaymsec_y);
    if(digitalRead(sensorPin_y)==0) {pos_y = 0; break;}
  }  
}

void run_motorx(){
    digitalWrite(motorx_stepPin, HIGH);
    delayMicroseconds(delaymsec_x);
    digitalWrite(motorx_stepPin, LOW);
    delayMicroseconds(delaymsec_x);
}

void run_motory(){
    digitalWrite(motory_stepPin, HIGH);
    delayMicroseconds(delaymsec_y);
    digitalWrite(motory_stepPin, LOW);
    delayMicroseconds(delaymsec_y);
}
