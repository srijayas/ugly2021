#include <DFPlayerMini_Fast.h>
#include "SR04.h"
static const uint8_t PIN_MP3_TX = 11; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 10; // Connects to module's TX 

#if !defined(UBRR1H)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(PIN_MP3_RX, PIN_MP3_TX); // RX, TX
#endif

#define TRIG_PIN 3
#define ECHO_PIN 2
SR04 sr04 = SR04 (ECHO_PIN,TRIG_PIN);

DFPlayerMini_Fast myMP3;

const long scrollDelay = 15;   // adjust scrolling speed
long a;
int minSensorDist = 20 ;
long duration, distance;
bool songPlaying;
void setup() {
  
  Serial.begin(9600);
  
  #if !defined(UBRR1H)
    mySerial.begin(9600);
    myMP3.begin(mySerial, true);
  #else
    Serial1.begin(9600);
    myMP3.begin(Serial1, true);
  #endif  
  // put your setup code here, to run once:
   pinMode(TRIG_PIN, OUTPUT);
   pinMode(ECHO_PIN, INPUT);
   
   
  delay(1000);
  Serial.println("Setting volume to max");
  myMP3.reset();
  myMP3.volume(30);
  
  songPlaying = false;

}

void loop() {
  // put your main code here, to run repeatedly:
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  String s = String(distance);
  Serial.println(" cm");
  if (distance < minSensorDist) // && !songPlaying) 
  {
   
    Serial.println("SCROLL NOW: Distance was " + s + " cm, Playing song ...");
    Serial.println("Looping track 1");
    myMP3.playFromMP3Folder(random(1,6));
    songPlaying = true;
  }
  else
  {
    Serial.println("Song already playing");
  }

  delay(1000);
    
}
