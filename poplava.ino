#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "flood-d74b7-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "p6eyzRc5Fvq7J6me4tharFiM3l3stkc7AbcV3UWp"

#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "12345678"

#define redLed 16
#define yellowLed 13
#define greenLed 0
#define buzzPin 5


int pin = A0;
int sensorValue = 0;
bool buzzerPower = true;
//int buzzPin = 14;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("..");
    delay(200);
  }
  Serial.println();
  Serial.println("NodeMCU is Connected!");
  Serial.println(WiFi.localIP());
    
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  pinMode(buzzPin, OUTPUT); 
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

  sensorValue = analogRead(pin);
  String waterMsg = CheckWaterLevel(sensorValue);
  
  Serial.println(sensorValue);
  
  Firebase.setString("flood", waterMsg);
   if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
  } 

  buzzerPower = Firebase.getBool("power");
  delay(1000);
  
}

String CheckWaterLevel(int level) {
 String msg = "No flood";


 if(level >= 100) {
   digitalWrite(greenLed, LOW);
    Buzz();
    
    if (level > 100 && level <= 300) {
      msg = "Water level: low";
      digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else if( level > 300 && level <= 330) {
      msg = "Water level: medium";
       digitalWrite(yellowLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else if (level > 330) {
      msg = "Water level: high";
       digitalWrite(yellowLed, LOW);
      digitalWrite(redLed, HIGH);
    }

 } else {
    digitalWrite(greenLed, HIGH);
    digitalWrite(yellowLed, LOW);
    digitalWrite(redLed, LOW);
  }
  return msg;
}

void Buzz() {
  if(buzzerPower) {
    digitalWrite(buzzPin, HIGH);
    delay(500);
    digitalWrite(buzzPin, LOW);
  }
}
