#define DIST_S 200*58.2 // 200cm로 제한
#define DISTANCE 10 //보행자가 기다릴때 서있는 곳의 초음파 센서 간격
#define CW_DISTANCE 50 //보행자가 기다릴때 서있는 곳의 초음파 센서 간격

//기다릴때 초음파
int trig[2]={31,33};
int echo[2]={39,41};

void setup() {
  Serial.begin (9600);
  SENSOR_INIT();
}

void SENSOR_INIT() {
  
  for (int i=0; i<2; i++){
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
  }
}
 
void loop() {
  long dist[2]; 

    //횡단보도 초음파 센서 측정 시작 
    for(int i=0; i<2; i++){
      dist[i] = trig_ultra(trig[i], echo[i]);  
      Serial.print(dist[0]);
      Serial.print("cm");
      Serial.write(9); //tab
      Serial.print(dist[1]);
      Serial.println("cm");
    }

  delay(1000); //1초
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
