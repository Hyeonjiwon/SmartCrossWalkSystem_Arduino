int count = 1;

void setup(){
  Serial.begin(9600);   //컴퓨터화면에 data값을 확인할 수 있도록 시리얼 통신 설정
}
  
void loop(){  
  Serial.print(count);
  Serial.println();
  //count++;
  
  if(count >10) {
    count = 0;
  }
  
  delay(1000);
}
