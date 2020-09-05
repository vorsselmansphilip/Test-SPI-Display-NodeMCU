/*
 *
 * connect with your board (please reference board compatibility)
 * cloned from: 
 * https://www.youtube.com/watch?v=KAN4k_k4wLE&list=UU0p938IT6gwqTjoO0nWQeIA
 * https://imgur.com/a/zIYda
 * https://github.com/seanauff/classic-car-sensor-interface
 * 
 * 
 * NODEMCU    ESP8266       controler (uno)                 DFRobot 2.2 inch tft lcd display
 
 * GPIO14/D5                   D5   GPIO14    SPI_SCK       ---->  SPI_SCL
 * GPIO13/D7                   D7   GPIO13    SPI_MOSI      ---->  SPI_MOSI
 *                                                          ---->  Not Used
 * GPIO15/D8                   D8   GPIO15    HCS           ---->  cs ( do not connect during developing does not allow flashing)
 *
 * GPIO5/D1                    5                           ---->  rs (LCD Internal Registers Select)
 * GPIO2/D4                    6                           ---->  wr
 * GPIO4/D2                    7                           ---->  lck (LCD Internal Registers Clock Line)
 * 
 * show many graphics on board
 *
2.2-inch LCD display screen (round screen), dot matrix 128 * 128 degree HD analysis, commonly used on mobile phones, watches, instruments, medical setting, digital, industrial products in. Compatible with TFT 8K3963FPC, IC is ST7687S, without TP , 30PIN welded.
LCD type TFT TRANSMISSIVE
Viewing direction 12:00 O 'Clock 6:00 O' Clock
(Good Viewing)
Module area (WXHXT) 40.80X47.70 X 2.50 mm3
Viewing area (Φ) 37.50 mm
Active area (Φ) 35.10 mm
Number of Dots 220RGB X 220
Pixel pitch (W 'H) 0.156 X 0.156 mm2
Driver IC ST7687S
Interface Type System parallel interface
Input voltage 2.8
 *
 */

#include <Arduino.h>
#include "DFRobot_ST7687S_Latch.h"
#include "DFRobot_Display_Clock.h"
#include "gauge.h"
#include "weixinpic.c"
#include "test.c"
#include "math.h"
//#include "Seven_Segment20pt7b.h"


#include "SPI.h"
static const uint8_t D11  = 9;
static const uint8_t D12  = 10;

#ifdef ARDUINO_ESP8266_NODEMCU
uint8_t pin_cs = D8, pin_rs = D1, pin_wr = D4, pin_lck = D2;
#endif

DFRobot_ST7687S_Latch tft(pin_cs, pin_rs, pin_wr, pin_lck);
DFRobot_Display_Gauge gauge(&tft);



double lower_limit = 0.0;
double upper_limit = 9999.0;
String eng_units = "RPM";
double gauge_div = 4;
double gauge_range = 270.0;
double gauge_start = 135.0;
int green_div = 18;
int orange_div = 2;
int LL_div = 0;
uint16_t LL_color = DISPLAY_RED;
int L_div = 0;
uint16_t L_color = DISPLAY_ORANGE;
int inrange_div = 21;
uint16_t inRange_color = DISPLAY_BLUE;
int H_div = 0;
uint16_t H_color = DISPLAY_ORANGE;
int HH_div = 0;
uint16_t HH_color = DISPLAY_RED;
int red_div = 1;
int gauge_w = 64;
int gauge_h = 64;

float sinVal;                                                   // variable which can hold the sine value
static int x = 0;

float deg2rad(long degrees){
  return degrees * PI / 180;
}



void draw_segment(double angle){
  //https://www.calculator.net/triangle-calculator.html?vc=&vx=64&vy=&va=14&vz=64&vb=&angleunits=d&x=29&y=33
 
  tft.setTextColor(DISPLAY_BLUE);
 



  int total_div = green_div + orange_div + red_div;



  double div_angle = (gauge_range  / (green_div + orange_div + red_div) - gauge_div); 
  double gauge_angle = (gauge_range - (total_div-2 * gauge_div))/total_div;

  //calculate half of the triangle to get the r
  //https://www.mathsisfun.com/polar-cartesian-coordinates.html
  double A = gauge_angle / 2;
  A = 12.8;
  double B = 90.0;
  double C = 180.0 - B - A;
  double c = 64.0;
  
  tft.setCursor(40,20);
  tft.println(div_angle);
  double a = c * sin(radians(A))/sin(radians(C));
  double b = c * sin(radians(B))/sin(radians(C));
  double ax,ay,bx,by,cx,cy;
  //calulate side b

  ax = 0;
  ay = 0;

  //calculate coordinates
  // b coordinates
  bx = gauge_w * cos(radians(135));
  by = gauge_w * sin(radians(135));


  //c coordinates
   //div_angle = 2.0;
  cx = gauge_w * cos(radians(45));
  cy = gauge_w * sin(radians(45));

  //Serial.println(ax,ay);
  //Serial.println(bx,by);
  //Serial.println(cx,cy);

  
  tft.setCursor(40,30);
  //tft.println(round(ax ), round(ay));
  tft.setCursor(40,40);
  //tft.println(round(bx),round(by));
  tft.setCursor(30,40);
  //tft.println(round(cx),round(cy));

  //tft.fillTriangle(ax,ay,bx,by,cx,cy,DISPLAY_GREEN);

  double currentangle = 135;
  tft.setLineWidth(2);
  tft.fillCircle(0,0,64,inRange_color);
  tft.fillCircle(0,0,54,DISPLAY_BLACK);
  currentangle = gauge_start;

    //calculate coordinates
  // b coordinates
  bx = gauge_w *2* cos(radians(135));
  by = gauge_w *2* sin(radians(135));


  //c coordinates
   //div_angle = 2.0;
  cx = gauge_w *2* cos(radians(45));
  cy = gauge_w *2* sin(radians(45));

  tft.fillTriangle(ax,ay,bx,by,cx,cy,DISPLAY_BLACK);

  
  cx = gauge_w * cos(radians(gauge_start));
  cy = gauge_w * sin(radians(gauge_start));

  if(LL_div > 0){
  
    // Calculate the LL
    bx = cx;
    by = cy;
    // recalculate c point
    currentangle = currentangle + (LL_div *A);
    //currentangle = 165;
    tft.println(currentangle);

    cx = gauge_w  * cos(radians(currentangle));
    cy = gauge_w  * sin(radians(currentangle));

    tft.fillTriangle(ax,ay,bx,by,cx,cy,LL_color);


  }

    if(L_div > 0){
  
    // Calculate the L
    bx = cx;
    by = cy;
    // recalculate c point
    currentangle = currentangle + (L_div *A);
    //currentangle = 165;
    //tft.println(currentangle);

    cx = gauge_w  * cos(radians(currentangle));
    cy = gauge_w  * sin(radians(currentangle));

    tft.fillTriangle(ax,ay,bx,by,cx,cy,L_color);


  }

    if(inrange_div > 0){
  
    // Calculate the L
    bx = cx;
    by = cy;
    // recalculate c point
    currentangle = currentangle + (inrange_div *A);
    //currentangle = 165;
    //tft.println(currentangle);

    cx = gauge_w  * cos(radians(currentangle));
    cy = gauge_w  * sin(radians(currentangle));

    //tft.fillTriangle(ax,ay,bx,by,cx,cy,L_color);


  }

  if(H_div > 0){
  
    // Calculate the L
    bx = cx;
    by = cy;
    // recalculate c point
    currentangle = currentangle + (H_div *A);
    //currentangle = 165;
    //tft.println(currentangle);

    cx = gauge_w  * cos(radians(currentangle));
    cy = gauge_w  * sin(radians(currentangle));

    tft.fillTriangle(ax,ay,bx,by,cx,cy,H_color);


  }

  if(HH_div > 0){
  
    // Calculate the L
    bx = cx;
    by = cy;
    // recalculate c point
    currentangle = currentangle + (HH_div *A);
    //currentangle = 165;
    //tft.println(currentangle);

    cx = gauge_w  * cos(radians(currentangle));
    cy = gauge_w  * sin(radians(currentangle));

    tft.fillTriangle(ax,ay,bx,by,cx,cy,HH_color);


  }



  
  currentangle = gauge_start;
  cx = gauge_w  * cos(radians(currentangle));
  cy = gauge_w * sin(radians(currentangle));
  tft.drawLine(cx*7/8,cy*7/8,cx,cy,DISPLAY_BLACK);

  for(int x= 0; x < 20+1; x++){

  currentangle = currentangle + (A);

  //shift coordinates to the next point
  //tft.setLineWidth (3);
  bx = cx;
  by = cy;
  // recalculate c point
  
  cx = round(gauge_w * cos(radians(currentangle)));
  cy = round(gauge_w * sin(radians(currentangle)));
  tft.drawLine(cx*7/8,cy*7/8,cx,cy,DISPLAY_BLACK);
  
  //draw the triangle
  //tft.fillTriangle(ax,ay,bx,by,cx,cy,DISPLAY_GREEN);
  //tft.fillTriangle(bx,by,ax,ay,cx,cy,DISPLAY_GREEN);

  //currentangle = currentangle + (gauge_div);

  //shift coordinates to the next point
  //bx = cx;
  //by = cy;
  // recalculate c point
  
  //cx = gauge_w * cos(radians(currentangle));
  //cy = gauge_w * sin(radians(currentangle));

 


  }





tft.fillCircle(0,0,40,DISPLAY_BLACK);
tft.fillCircle(0,0,4,inRange_color);

tft.fillCircle(-20,11,4,DISPLAY_ORANGE);
tft.fillCircle(0,11,4,DISPLAY_ORANGE);
tft.fillCircle(20,11,4,DISPLAY_RED);

tft.setTextColor(inRange_color);
//tft.setCursor(45,85);
////tft.setTextSize(2);
//tft.print("9999");

tft.setCursor(60,100);
tft.setTextSize(1);
tft.print(eng_units);
tft.setCursor(45,110);
tft.setTextSize(1);
tft.print("00:00:00");

  //tft.fillTriangle(0,0,-64,16,-64,30,DISPLAY_GREEN);
  //tft.fillTriangle(0,2,-64,0,-64,16,DISPLAY_GREEN);

  //tft.fillTriangle(0,0,-64,0,-64,-14,DISPLAY_GREEN);
  //tft.fillTriangle(0,0,-64,-16,-64,-30,DISPLAY_GREEN);

  //tft.fillTriangle(0,0,64,0,64,14,DISPLAY_YELLOW);
  //tft.fillTriangle(0,0,64,16,64,30,DISPLAY_YELLOW);


  //tft.fillCircle(0,0,40,DISPLAY_BLACK);
  


}


void setup(void)
{
  Serial.begin(115200);
  tft.begin();
  tft.fillScreen(DISPLAY_BLACK);
  gauge.setClockDial(0,0,60);
  gauge.setDialColor(DISPLAY_GREEN,DISPLAY_BLACK);
  gauge.setPointerLength(32, 40, 48);  //pointer length: hour, minute, second
  gauge.setPointerWidth(3, 2, 2);  //pointer width: hour, minute, second
  gauge.setPointerColor(DISPLAY_RED, DISPLAY_GREEN, DISPLAY_BLUE);  //pointer color: hour, min, second
  //gauge.showDial();

  tft.fillScreen(DISPLAY_BLACK);
  //tft.drawBmp((uint8_t*)red_20gauge_bits, -50, -50, 118, 118,);
  //tft.drawBmp((uint8_t*)gImage_128_i,-64,-64,128,128,1,DISPLAY_WHITE);
  //radians = deg * PI / 180

  float angle = 14.0;
  // angle b & c 
  float rad = deg2rad(angle);
  float cosi = cos(rad);
  String stringOne =  String(rad, DEC); 
  tft.setCursor(40,40);
  tft.setTextSize(1);
  tft.println(stringOne);
  draw_segment(angle);

  
 

  

}

void sinwave(){
  
  x ++;
    //== generatingh the next sine value ==// 

    sinVal = (sin(radians(x)));  
                                       
    if (x == 180){
      x = 0;
    }
    


      



}

void gauge_value(double actual_value){
double cx,cy;
tft.setLineWidth(2);
//tft.fillCircle(0,0,40,DISPLAY_BLACK);
//tft.drawLine(cx*6/8,cy*6/8,cx*5/8,cy*5/8,DISPLAY_BLACK);
double range = gauge_range/(upper_limit + abs(lower_limit));
double current_value = 135 + (range * actual_value);

tft.setCursor(64,20);
tft.setTextSize(1);
cx = round(gauge_w * cos(radians(current_value)));
cy = round(gauge_w * sin(radians(current_value)));
tft.setLineWidth(1);
tft.drawLine(cx*6/8,cy*6/8,cx*5/8,cy*5/8,DISPLAY_ORANGE);

tft.print(current_value);


}


void loop(void)
{

  sinwave();

  tft.setCursor(45,84);
  tft.setTextSize(2);

  String stringOne =  String(round(sinVal * upper_limit));                        // using a long and a base
  //
  tft.print(stringOne);
  gauge_value(sinVal * upper_limit);
  delay(10);

  
}



