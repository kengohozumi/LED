#define USE_USBCON  // seeeduino xiao model needs this flag to be defined
#define SERIAL_PORT Serial

#include <Adafruit_NeoPixel.h>
#include <ros.h>
#include <std_msgs/Bool.h>
#define PIN 6
#define ANALOG_PIN A0
#define NUMPIN 30
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIN, PIN, NEO_GRB + NEO_KHZ800);

ros::NodeHandle nh;

volatile bool navigation = false;
volatile bool stop = false;
unsigned long time =0;

void navigation_callback(const std_msgs::Bool& navBool) {
  navigation = navBool.data;
  time = millis(); 
}

void estop_callback(const std_msgs::Bool& estop) {
  stop = estop.data;
}

ros::Subscriber<std_msgs::Bool> sub("/navBool", &navigation_callback);
ros::Subscriber<std_msgs::Bool> sub1("/estop", &estop_callback);

void whiteLight() {
  int i = 0;
  for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 50, 50, 50);//white
  strip.show();
  delay(5);
  for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 0, 0, 0);
  strip.show();
}
void blueLight() {
  int i=0;
  for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 0, 0, 50);
  strip.show();
}

void blueLight1_1() {
  int i, j;
  //for(j=0; j<256; j++) {
    for(i=0; i<NUMPIN; i++) {
      if(i%2 == 0){
        strip.setPixelColor(i, 0, 0, 50);
      }else{
        strip.setPixelColor(i, 0, 0, 0);
      }
    }
    strip.show();
    //delay(wait);
  //}  
}

void blueLight1_2() {
    for(int i=0; i<NUMPIN; i++) {
      if(i%2 == 1){
        strip.setPixelColor(i, 0, 0, 50);
      }else{
        strip.setPixelColor(i, 0, 0, 0);
      }
    }
    strip.show();
    //delay(wait);
  } 

void blueLight2(){
  blueLight1_1();
  blueLight1_2();
}

void blueLight3(){
  int i;
     for(i = 0 ; i < NUMPIN; i++ )
     {
        strip.setPixelColor(i-1,0,0,0);
        strip.setPixelColor(i, 0, 0, 50);
        strip.show();
        delay(30);
     }
}

void orangeLight() {
  int i=0;
  for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i,45,13,0);//255,69,0
  strip.show();
}

void orangeLight1_sequential() {
  int i=0;
  int j = 0;
   while (j < 5) {
     for(i=0;i<NUMPIN;i++)strip.setPixelColor(i,45,13,0);strip.show();delay(100);
     for(i=0;i<NUMPIN;i++) strip.setPixelColor(i, 0, 0, 0);strip.show();     
    //for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i,45,13,0);strip.show();strip.setPixelColor(i,0,0,0);strip.show();//255,69,0
    j++;
    if(navigation == false)break;
    if(stop)break; 
  }
}

void orangeLight2_sequential() {
  int i=0;
  int j = 0;
   while (j < 5) {
     for(i=NUMPIN;i>=0;i--)strip.setPixelColor(i,45,13,0);strip.show();delay(100);
     for(i=NUMPIN;i>=0;i--) strip.setPixelColor(i, 0, 0, 0);strip.show();     
    //for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i,45,13,0);strip.show();strip.setPixelColor(i,0,0,0);strip.show();//255,69,0
    j++;
    if(navigation == false)break;
    if(stop)break; 
  }
}

void orangeLight3(){
  int i;
     for(i = 0 ; i < NUMPIN; i++ )
     {
        strip.setPixelColor(i-1,0,0,0);
        strip.setPixelColor(i, 45,13,0);
        strip.show();
        delay(30);
     }
}

void redLight() {
  int i = 0;
  for (i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 50, 0, 0);
  strip.show();
}

void redLightflash() {
  int j = 0;
  int i = 0;
  while (j < 5) {
    for ( i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 50, 0, 0);
    strip.show();
    delay(100);
    for ( i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(100);
    j++;
    if(navigation == false)break;
    if(stop)break; 
  }
}

void downLight() {
  int i = 0;
  for ( i = 0; i < NUMPIN; i++) strip.setPixelColor(i, 0, 0, 0);
  strip.show();
}

void controlLoopnav3() {
   if(stop == true && navigation == true){redLightflash();}
   else if(stop == true && navigation == false){redLightflash();}
   else if(stop == false && navigation == true){blueLight2();} 
   else if(stop == false && navigation == false){orangeLight3();}
 }    

void setup() {
  SERIAL_PORT.begin(115200);
  nh.getHardware()->setBaud(115200); 
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(sub1);
  strip.begin();
  strip.show();  
  delay(100);
  navigation = false;
  while (SERIAL_PORT.available())
    SERIAL_PORT.read();
  delay(1000);
}

void loop() {
  if(millis() > time+700){navigation = false;}//WDT
  controlLoopnav3();
  nh.spinOnce();
  delay(10);
  delay(100);
}
