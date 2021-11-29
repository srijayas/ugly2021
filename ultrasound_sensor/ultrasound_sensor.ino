#include <DFPlayerMini_Fast.h>
#include <Adafruit_NeoPixel.h>
#include <NewPing.h>

static const bool DEBUG=false;

// Neopixel inits --------------->
#define PIN        6
#define NUMPIXELS 16
#define DELAYVAL 500
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// MP3 module DFPlayer inits --------------->
static const uint8_t PIN_MP3_TX = 11; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 10; // Connects to module's TX 
#if !defined(UBRR1H)
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(PIN_MP3_RX, PIN_MP3_TX); // RX, TX
#endif
DFPlayerMini_Fast myMP3;

// Ultrasonic sensor inits --------------->
#define TRIG_PIN 3
#define ECHO_PIN 2
//SR04 sr04 = SR04 (ECHO_PIN,TRIG_PIN);
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Variables --------------->
int minSensorDist = 20 ;
int uS;
long duration, distance;
bool songPlaying;
bool handSensedTurnOn;
bool handSensedTurnOff;

typedef struct {
  int r;
  int g;
  int b;
} rgb;

const rgb fireColors[] 
{
  {128,17,0},
  {182,34,3},
  {215,53,2},
  {252,100,0},
  {255,117,0},
  {250,192,0},
  {161,1,0},
  {254,101,13},
  {242,60,4}
};
int rnd;

void Print(String message)
{
  if (!DEBUG)
    return;
  Serial.println(message);
}
void playRandomSong(bool play)
{
  if (play)
  {    
    Print("Looping random track");
    myMP3.playFromMP3Folder(random(1,7));
    songPlaying = true;
    
  }
  else
  {
    songPlaying = false;    
    myMP3.stop();
  }

}


void playLights(bool play)
{
 if (play)
 {
    pixels.clear();
    for (int i=0;i < NUMPIXELS;i++)
    {
      Print("\n starting lights " + String(i) + "," + String(NUMPIXELS) + "," + String(fireColors[rnd].r));
      rnd = random(0,8);    
      pixels.setPixelColor(i, fireColors[rnd].r,fireColors[rnd].g,fireColors[rnd].b);
      pixels.show();
    }
   delay(random(100,500));
 }
 else
 {
    Print("\n STOPPING lights");
    pixels.fill((0,0,0));
    pixels.show();  
    delay(1000);
 }
}

void setup() {
  
  Serial.begin(9600);
  
  #if !defined(UBRR1H)
    mySerial.begin(9600);
    myMP3.begin(mySerial, true);
  #else
    myMP3.begin(Serial1, true);
  #endif  


  pixels.begin();
   
  delay(1000);
  Serial.println("Setting volume to max");
  myMP3.reset();
  myMP3.volume(30);
  
  
  songPlaying = false;
  handSensedTurnOn = false;
  handSensedTurnOff = false;
  playLights(false);
  playRandomSong(false);

  
}

void loop() {
  //delay(50);
  //pixels.clear();
  uS = sonar.ping_cm();  
  pinMode(ECHO_PIN,OUTPUT);

  digitalWrite(ECHO_PIN,LOW);

  pinMode(ECHO_PIN,INPUT);

  Serial.print("Ping: ");
  distance = uS;
  Serial.print(distance);
  Serial.println("cm");  
//  if (distance == 0 && digitalRead(ECHO_PIN) == LOW) {
//    pinMode(ECHO_PIN, OUTPUT);
//    digitalWrite(ECHO_PIN, LOW);
//    delay(100);
//    Serial.println("error");
//    pinMode(ECHO_PIN, INPUT);    
//  }  
  
  if (distance > 5 && distance <= minSensorDist)
  {
    handSensedTurnOn = true;
    handSensedTurnOff = false;
    
  }
  else if (handSensedTurnOn && distance <=5 && distance > 0)
  {
    handSensedTurnOff = true;
    handSensedTurnOn = false;
    
  }
 
  

  if (handSensedTurnOn)
  {
    
    if (!songPlaying)
    {
      Print("Distance was " + String(distance) + " cm, Playing song ...");
      playRandomSong(true);
    }
    playLights(true);
  }

  if (handSensedTurnOff)
  {
    Print("Distance was " + String(distance) + " cm, STOPPING song ...");
    playRandomSong(false); 
    playLights(false);
  }


}
