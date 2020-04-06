#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Gaitonde"
#define STAPSK  "Akash1995"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);
const int button1Pin = 14;     // the number of the pushbutton pin  
const int led1Pin = 2;      // the number of the LED pin        

const int button2Pin = 16;     // the number of the pushbutton pin  
const int led2Pin =  4;      // the number of the LED pin        

const int button3Pin = 12;     // the number of the pushbutton pin    2
const int led3Pin = 15 ;      // the number of the LED pin  15        

const int button4Pin = 13;     // the number of the pushbutton pin    
const int led4Pin = 5;      // the number of the LED pin  
const int buzzer = 0;

int button1State = 0;         // variable for reading the pushbutton status
int button2State = 0;         // variable for reading the pushbutton status
int button3State = 0;         // variable for reading the pushbutton status
int button4State = 0;         // variable for reading the pushbutton status


int count = 0;// variables will change:
int guessArray[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // array to hold our (the players) guesses
int pattern[] = {2,1,3,0,0,3,0,2,0,1,1,1,0,3,2};
int turn = 0; //0 for computers 1 for humans

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
 
  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>Simon</title>\
    <style>\
      body { background-color: #dddddd; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Welcome to Simon!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>You are currently playing Level %02d</p>\
  </body>\
</html>",

           hr, min % 60, sec % 60, turn
          );  server.send(200, "text/html", temp);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}


void setup(void) {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

 
  // initialize the LED pin as an output:
  pinMode(led1Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT);
 
  // initialize the LED pin as an output:
  pinMode(led2Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button2Pin, INPUT);

  // initialize the LED pin as an output:
  pinMode(led3Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button3Pin, INPUT);

    // initialize the LED pin as an output:
  pinMode(led4Pin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button4Pin, INPUT);

  pinMode(buzzer, OUTPUT);

}

void initializeGame(){
  count = 0;
  if (button1State == HIGH && button2State == HIGH && button3State == HIGH && button4State == HIGH){
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, HIGH);
    digitalWrite(led4Pin, HIGH);
    delay(1000);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
    digitalWrite(led4Pin, LOW);
    delay(1000);
  }else{
    for(int i = 0; i < 15; i++){
    // server.handleClient();
    // MDNS.update();
     displayPattern(i);
     getUserInput(i);
     Serial.print("End Round ");
     Serial.println(i);
     turn++;
     //count = turn;
     server.handleClient();
     MDNS.update();
    }
     displayVictory();
     delay(5000);
     initializeGame();
  }
}

void displayPattern(int rnd){
 int count = 0;
 while (count < rnd){
  int x = pattern[count];
  delay(400);
  if(x == 0){
      digitalWrite(led1Pin, HIGH);
      tone(buzzer, 1000);
      delay(300);
      digitalWrite(led1Pin, LOW);
      noTone(buzzer);
      count++;
  }else if(x == 1){
      digitalWrite(led2Pin, HIGH);
      tone(buzzer, 900);
      delay(300);
      digitalWrite(led2Pin, LOW);
      noTone(buzzer);
      count++;
  }else if(x == 2){
      digitalWrite(led3Pin, HIGH);
      tone(buzzer, 1800);
      delay(300);
      digitalWrite(led3Pin, LOW);
      noTone(buzzer);
      count++;
  }else if(x == 3){
      digitalWrite(led4Pin, HIGH);
      tone(buzzer, 700);
      delay(300);
      digitalWrite(led4Pin, LOW);
      noTone(buzzer);
      count++;
  }
 }
}

void getUserInput(int rnd){
 count = 0;
 while (count < rnd){
  if  (digitalRead(button1Pin) == 1 || digitalRead(button2Pin) == 1 || digitalRead(button3Pin) == 1 || digitalRead(button4Pin) == 1){
   bool pressed = false;
   while(pressed == false ){
    delay(100);
      if (digitalRead(button1Pin) == 0) {
        Serial.println("Button 1 Pressed");
        tone(buzzer, 1000);
        digitalWrite(led1Pin, HIGH);
        delay(300);        
        noTone(buzzer);
        guessArray[count] = 0;
        digitalWrite(led1Pin, LOW);
        pressed = true;
      break;    
      }else if(digitalRead(button2Pin) == 0){
        Serial.println("Button 2 Pressed");
        tone(buzzer, 900);
        digitalWrite(led2Pin, HIGH);
        delay(300);        
        noTone(buzzer);
        guessArray[count] = 1;
        digitalWrite(led2Pin, LOW);
        pressed = true;
        break;
      }else if (digitalRead(button3Pin) == 0) {
        Serial.println("Button 3 Pressed");
        tone(buzzer, 800);
        digitalWrite(led3Pin, HIGH);
        delay(300);        
        noTone(buzzer);
        guessArray[count] = 2;  
        digitalWrite(led3Pin, LOW);
        pressed = true;
        break;    
      }else if(digitalRead(button4Pin) == 0){
          Serial.println("Button 4 Pressed");
          tone(buzzer, 700);
          digitalWrite(led4Pin, HIGH);
          delay(300);        
          noTone(buzzer);
          guessArray[count] = 3;
          digitalWrite(led4Pin, LOW);
          pressed = true;            
          break;
      }
     }
   }
    if(guessArray[count] !=  pattern[count]){
      turn = 0;
      Serial.println("Sorry you lose.");
      tone(buzzer, 100);
      delay(1000);        
      noTone(buzzer);
      initializeGame();
    }
    count++;
  }
}

void displayVictory() {
   
  tone(buzzer, 1000);
  delay(300);        
  noTone(buzzer);    
  delay(300);        
  tone(buzzer, 1000);
  delay(300);        
  noTone(buzzer);    
  delay(300);        
  tone(buzzer, 1000);
  delay(300);        
  noTone(buzzer);    
  delay(300);      
  tone(buzzer, 1000);
  delay(800);        
  noTone(buzzer);    
  delay(300);    
  Serial.println("Congratulations, you win!!!");    
}

void loop(void) {
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  button4State = digitalRead(button4Pin);
  tone(buzzer, 1000);
  delay(300);        
  noTone(buzzer);    
  delay(300);
  server.handleClient();
  MDNS.update();
  initializeGame();
}
