int buzzer = 16;
long ranNum;
int buzzTone;
String answerArray[3];
String guessArray[3];
boolean victoryFlag = false; // flag to check victory condition
int flag = 0;
int Counter = 0;
// end of item setup
String buttonAnsArray[] = {"blue", "red", "green"};


void setup() {
  // put your setup code here, to run once:
  //LEDs
pinMode(12, OUTPUT); 
pinMode(13, OUTPUT);
pinMode(14, OUTPUT);
//Buttons 
pinMode(15, INPUT);  
pinMode(5, INPUT); 
pinMode(4, INPUT);
//Buzzer
pinMode(buzzer, OUTPUT); 
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  
  if(flag == 0){
   for(int i=0; i<3;i++){
        ranNum = random(12,15); 
        switch(ranNum){
          case 12:
            answerArray[i] = "green";
            buzzTone = 3000;
            break;
          case 13:
            buzzTone = 5000;
            answerArray[i] = "red";
            break;
          case 14:
            buzzTone = 1000;  
            answerArray[i] = "blue";
            break;
        }
        digitalWrite(ranNum, HIGH); 
        tone(buzzer, buzzTone); 
        delay(500); 
        tone(buzzer, 0);
        digitalWrite(ranNum, LOW); 
        delay(200); 
    }
    for(int i=0;i<3;i++){
        Serial.println(answerArray[i]); 
      }
      flag = 1;
  }

    if (digitalRead(15) == HIGH) {
      guessArray[Counter]= "blue";
       digitalWrite(15, HIGH); 
       tone(buzzer, 1000, 500);
       delay(300);
       Counter++;
      Serial.println(Counter); 
       Serial.println("blue");
       delay(300);
    } 
    if (digitalRead(5) == HIGH) {
      guessArray[Counter]= "red";
       tone(buzzer, 1000, 500);
       delay(300);
       // delay(1000);
      Counter++;
      Serial.println(Counter); 
       Serial.println("red");
        delay(300);
    } 
    if (digitalRead(4) == HIGH) {
      guessArray[Counter]= "green";
      tone(buzzer, 1000, 500);
      delay(300);
      Counter++;
      Serial.println(Counter); 
       Serial.println("green"); 
        delay(300);
    } 

if(Counter == 3){
for (int x = 0; x < 3; x++) {
     if (answerArray[x]==guessArray[x]) { 
       victoryFlag=true;
     } else {
        Serial.println("you lose!");
        tone(buzzer, 1000, 500);
        delay(4000);
        tone(buzzer,0);
        delay(1000);
       victoryFlag=false;
       break;
     } 
} 

if (victoryFlag) {
  
  Serial.println("You win!");
  tone(buzzer,500,200);
 delay(200);
 tone(buzzer,500,200);
 delay(200);
 tone(buzzer,500,200);
 delay(200);
 tone(buzzer,800,150);
 delay(150);
 tone(buzzer,500,500);
 delay(500);
 tone(buzzer,600,1000);
 delay(4000);

}
flag = 0;
Counter = 0;
}



     
 
}
