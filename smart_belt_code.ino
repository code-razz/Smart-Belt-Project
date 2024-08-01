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
  delay(500);
}
