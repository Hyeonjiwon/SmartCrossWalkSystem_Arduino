rl
#define DIST_S 200*58.2 // 200cm로 제한
#define DISTANCE 10 //보행자가 기다릴때 서있는 곳의 초음파 센서 간격
#define CW_DISTANCE 30 //보행자가 기다릴때 서있는 곳의 초음파 센서 간격

//int trig[3]={30,32,34};
//int echo[3]={22,24,26};

//300보다 커지면 물체가 감지된것, serial.print 해주기
const int LIMITVALUE = 300;

//레이저 센서
int lasor = 8;    
//조도 센서
int cds = A0;

//조도센서의 값을 저장하는 변수 선언
int val;

//차량 신호등 LED
int CAR_Y_LED = 2; // 노란불
int CAR_R_LED = 3; // 빨간불
int CAR_G_LED = 4; // 초록불

//횡단보도 LED
//int CW_R_LED = 11; // 빨간불
//int CW_G_LED = 10; // 초록불
//int CW_B_LED = 9; 

//횡단보도 LED
int CW_R_LED = 11; // 빨간불
int CW_Y_LED = 10; // 주황불

//기다릴때 초음파
int trig[2]={30,32};
int echo[2]={22,24};

//횡단보도 초음파
int cw_trig[2]={44,46};
int cw_echo[2]={36,38};

int flag_num = 0; //5가 되는 순간 빨간불이 켜짐 (5초 뒤라는 의미)
int flag = 0; //임시 변수, 노란불 제어용 (빨간불이 들어왔을 경우 노란불을 꺼줘야함)
int count = 0; //빨간불로 변경될때마다 ++ (라즈베리파이에서 시리얼 통신으로 변수 제어 가능한가...?)
int car_flag = 0;

void setup() {
  Serial.begin (9600);
  SENSOR_INIT();
}

void SENSOR_INIT() {
  pinMode(cds, INPUT);
  pinMode(lasor, OUTPUT);
  
  pinMode(CAR_Y_LED, OUTPUT);
  pinMode(CAR_R_LED, OUTPUT);
  pinMode(CAR_G_LED, OUTPUT);
  
  pinMode(CW_R_LED, OUTPUT);
  pinMode(CW_Y_LED, OUTPUT);
  //pinMode(CW_B_LED, OUTPUT);
  
  for (int i=0; i<2; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);

    pinMode(cw_trig[i],OUTPUT);
    pinMode(cw_echo[i],INPUT);
  }
}
 
void loop() {
  long dist[2]; 
  long cw_dist[2]; 
  
  for(int i=0; i<2; i++){
      dist[i] = trig_ultra(trig[i],echo[i]);
      cw_dist[i] =  trig_ultra(cw_trig[i],cw_echo[i]); //횡단보도 초음파 센서 값 초기화
      //cw_dist[i] = 100;
  }

   //아무런 상태도 아닐경우
  if((dist[0] >= DISTANCE) || (dist[1] >= DISTANCE) && (flag == 0)){
    //차량 신호등만 초록불
    digitalWrite(CAR_G_LED, HIGH);
    
     //나머지는 Off
    digitalWrite(CAR_R_LED, LOW);
    digitalWrite(CAR_Y_LED, LOW);
    
    digitalWrite(CW_R_LED, LOW);
    digitalWrite(CW_Y_LED, LOW);
    //setColor(0, 0, 0);
    
    flag_num =0;    //기다리다 갔을 경우
  }
  
  //보행자가 횡단보도 앞에서 기다릴 경우
  if((dist[0] < DISTANCE) && (dist[1] < DISTANCE) && (flag == 0)) {
    
    //차량 신호등 노란불로 설정
    digitalWrite(CAR_Y_LED, HIGH);

    //나머지는 off
    digitalWrite(CAR_R_LED, LOW);
    digitalWrite(CAR_G_LED, LOW);
    
    //횡단보도 신호등 노란불로 설정
    //setColor(255, 75, 0); // yellow

    digitalWrite(CW_R_LED, LOW);
    digitalWrite(CW_Y_LED, HIGH);
    
    flag_num++;  //횡단보도 초음파 센서 flag 변수 (5가 되면 빨간색 LED가 켜짐)
  }
  
  //보행자가 5초 이상 기다리면, 횡단보도 양쪽의 LED가 빨간색으로 변경 
  //보행자가 횡단보도를 건너는 경우
  if((flag_num == 5) && (flag == 0)) {    
     //차량 신호등 빨간불로 설정
    digitalWrite(CAR_R_LED, HIGH);

    //나머지는 Off
    digitalWrite(CAR_G_LED, LOW);
    digitalWrite(CAR_Y_LED, LOW);
    
    //횡단보도 신호등 빨간불로 설정
    //setColor(255, 0, 0); // red
    digitalWrite(CW_R_LED, HIGH);
    digitalWrite(CW_Y_LED, LOW);
    
    flag = 1; 
    /*
    Serial.print("!!!!!!!!!!!!!");
    Serial.println(flag);
    */
    if(flag == 1) {
      
      
      //Serial.print("건너기");
      for(;;) {
        detectCar();
        
         for(int i=0; i<2; i++){
          cw_dist[i] =  trig_ultra(cw_trig[i],cw_echo[i]); //횡단보도 초음파 센서 값 초기화

          Serial.print(cw_dist[0]);
          Serial.print("cm");
          Serial.write(9); //tab
          Serial.print(cw_dist[1]);
          Serial.println("cm");

        }
  
        //만약 건너고 있다면 
        if((cw_dist[0] < CW_DISTANCE) && (cw_dist[1] < CW_DISTANCE)) {
          //Serial.print("건너는 중");
          //차량 신호등 빨간불로 설정
          digitalWrite(CAR_R_LED, HIGH);
      
          //나머지는 off
          digitalWrite(CAR_G_LED, LOW);
          digitalWrite(CAR_Y_LED, LOW);
          
          //횡단보도 신호등 빨간불로 설정
          //setColor(255, 0, 0); // yellow
          digitalWrite(CW_R_LED, HIGH);
          digitalWrite(CW_Y_LED, LOW);
        }
    
        //다 건넜다면
        else {
          //Serial.print("다 건넘");  
          flag_num = 0;
          flag = 0;
          break;
        } 
        delay(1000); //1초                
      }       
      flag_num = 0;
      flag = 0;

      digitalWrite(lasor, LOW);
      
      count++;
      Serial.println(count);
    }
  }

  /*
  //보행자가 횡단보도를 건널 경우
  if(flag == 1){
    Serial.print("건너기");
    for(;;) {   
      //횡단보도 초음파 센서 측정 시작 
      for(int i=0; i<2; i++){
        cw_dist[i] = trig_ultra(cw_trig[i], cw_echo[i]);
      }
      
      //만약 건너고 있다면 
      if(cw_dist[0] < CW_DISTANCE && cw_dist[1] < CW_DISTANCE) {
        //차량 신호등 빨간불로 설정
        digitalWrite(CAR_R_LED, HIGH);
    
        //나머지는 off
        digitalWrite(CAR_G_LED, LOW);
        digitalWrite(CAR_Y_LED, LOW);
        
      //횡단보도 신호등 빨간불로 설정
        setColor(255, 0, 0); // yellow
      }
  
      //다 건넜다면
      else {
        break;
      }
      flag_num = 0;
      flag = 0;
    }
  }
  */
  print_data(dist, cw_dist, flag_num, flag);
  delay(1000); //1초
}

void detectCar() {
  //자동차 감지 시작
  digitalWrite(lasor, HIGH); //레이저모듈핀에 항상 HIGH값을 입력
  val = analogRead(cds); //조도센서로 부터 값을 읽어들인다 
  
  if(val > LIMITVALUE) { //값이 일정값 이하로 떨어질 경우
   Serial.print("car");
    Serial.println(val); //시리얼 모니터에 출력
  }
  
  else { //그 이외의 경우
    //analogWrite(piezo, 0); //소리가 울리지 않는다 
  }
}

void print_data(long dist[], long cw_dist[], int flag_num, int flag) {

  Serial.print(dist[0]);
  Serial.print("cm");
  Serial.write(9); //tab
  Serial.print(dist[1]);
  Serial.println("cm");

  Serial.print(cw_dist[0]);
  Serial.print("cm");
  Serial.write(9); //tab
  Serial.print(cw_dist[1]);
  Serial.println("cm");
  
  Serial.print("Num:");
  Serial.print(flag_num);
  Serial.write(9); //tab
  Serial.print("Flag:");
  Serial.println(flag);

}

long trig_ultra(int TRIG,int ECHO)
{
  long dist;
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  dist = pulseIn(ECHO, HIGH,DIST_S)/58.2;
  return(dist);
}

// RGB 값을 받아 analogWrite를 통해 각 핀에 연결된 LED에 전달 함수
/*void setColor(int red, int green, int blue)
{
  analogWrite(CW_R_LED, red);
  analogWrite(CW_G_LED, green);
  analogWrite(CW_B_LED, blue);
}
*/
