#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
// define your wifi credentials here
#define WIFI_SSID "motog62"
#define WIFI_PASSWORD "happiness"
#define API_KEY "AIzaSyBOlLTy-2cFMYIbcWD2RkAp3pmBpO_ogeU"
#define DATABASE_URL "https://awesomebelt-974da-default-rtdb.firebaseio.com" 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// int pushButtonState = 0;
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;





#include "pitches.h"

#define LEDC_CHANNEL 0
#define LEDC_TIMER 13
#define BUZZER_PIN  26


const int trig_front = 2;//
const int echo_front = 4;//
const int trig_back = 23;
const int echo_back = 22;
const int trig_left = 5;//
const int echo_left = 18;//
const int trig_right = 21;//
const int echo_right = 19;//

const int buttonPin = 34;//

int distance_front= 30;    //Threshold distance
int distance_back= 30;
int distance_left= 30;
int distance_right= 30;
int distance_depth= 30;


//Functions

void buzzer(int trigpin){
  int melody[] = {
    NOTE_C2,
    NOTE_G3,
    NOTE_E5,
    NOTE_A6
  };
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  int i;
  if(trigpin==trig_front){
    i=0;
    Serial.println("buzzer fornt");
  }else if(trigpin==trig_back){
    i=1;
    Serial.println("buzzer back");
  }else if(trigpin==trig_left){
    i=2;
    Serial.println("buzzer left");
  }else if(trigpin==trig_right){
    i=3;
    Serial.println("buzzer right");
  }
    for (int thisNote = 0; thisNote < 5; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      if (melody[thisNote] == 0) {
        // No note
        ledcWrite(LEDC_CHANNEL, 0);  // Turn off buzzer
      } else {
        // Play note
        ledcWriteTone(LEDC_CHANNEL, melody[i]);  // Set frequency
      }

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      ledcWrite(LEDC_CHANNEL, 0);  // Stop note
    }
}

void ultrasonic_func(int trigpin,int echopin,int dist,int hapticPin){
  int distance = 0;

  long duration;

  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);

  duration = pulseIn(echopin, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
  if(distance<dist){
    Serial.println("Obstacle");
    buzzer(trigpin);
  }
  else{
    Serial.println("NO Obstacle");
    digitalWrite(hapticPin,LOW);
    
  }
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  Serial.print(" ");
}




void setup() {
  // put your setup code here, to run once:
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = "AIzaSyBOlLTy-2cFMYIbcWD2RkAp3pmBpO_ogeU";
  config.database_url = "https://awesomebelt-974da-default-rtdb.firebaseio.com";

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
 

  pinMode(trig_front,OUTPUT);
  pinMode(echo_front,INPUT);
  pinMode(trig_back,OUTPUT);
  pinMode(echo_back,INPUT);
  pinMode(trig_left,OUTPUT);
  pinMode(echo_left,INPUT);
  pinMode(trig_right,OUTPUT);
  pinMode(echo_right,INPUT);

  pinMode(buttonPin,INPUT);

  ledcSetup(LEDC_CHANNEL, 2000, 8);  // Channel 0, 2 kHz frequency, 8-bit resolution
  ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL);  // Attach the buzzer to the channel

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  int button=digitalRead(buttonPin);
  if(button==1){
    Serial.println("Send emergency alert and location");
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        if (Firebase.RTDB.setInt(&fbdo, "test/int", 1)){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
        Firebase.RTDB.setInt(&fbdo, "test/int",0);
        }
    }
  }

  Serial.println("front");
  ultrasonic_func(trig_front,echo_front,distance_front,haptic_front);
  Serial.println("left");
  ultrasonic_func(trig_left,echo_left,distance_left,haptic_left);
  Serial.println("right");
  ultrasonic_func(trig_right,echo_right,distance_right,haptic_right);
  Serial.println("back");
  ultrasonic_func(trig_back,echo_back,distance_back,haptic_back);
  Serial.println("");
  delay(1000);
}
