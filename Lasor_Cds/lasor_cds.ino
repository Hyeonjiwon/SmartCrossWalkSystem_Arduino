const int LIMITVALUE = 300; //300보다 커지면 물체가 감지된것, serial.print 해주기

//센서별 핀번호 선언
int lasor = 8;    
int cds = A0;

//조도센서의 값을 저장하는 변수 선언
int val;

void setup() {               
  //핀모드 설정 
  pinMode(cds, INPUT);
  pinMode(lasor, OUTPUT);
  Serial.begin(9600); //시리얼 통신 초기화
}

void loop() {
  digitalWrite(lasor, HIGH); //레이저모듈핀에 항상 HIGH값을 입력
  val = analogRead(cds); //조도센서로 부터 값을 읽어들인다 
  
  if(val > LIMITVALUE) { //값이 일정값 이하로 떨어질 경우
    Serial.println(val); //시리얼 모니터에 출력
  }

  delay(500); 
}
