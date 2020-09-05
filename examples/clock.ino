/*
 *
 * connect with your board (please reference board compatibility)
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
 * Copyright   [DFRobot](http://www.dfrobot.com), 2016
 * Copyright   GNU Lesser General Public License
 *
 */

#include <Arduino.h>
#include "DFRobot_ST7687S_Latch.h"
#include "DFRobot_Display_Clock.h"
#include <TimeLib.h>
#include "SPI.h"
static const uint8_t D11  = 9;
static const uint8_t D12  = 10;

#ifdef ARDUINO_ESP8266_NODEMCU
uint8_t pin_cs = D8, pin_rs = D1, pin_wr = D4, pin_lck = D2;
#endif

DFRobot_ST7687S_Latch tft(pin_cs, pin_rs, pin_wr, pin_lck);
DFRobot_Display_Clock clk(&tft);

void setup(void)
{
  Serial.begin(115200);
  setTime(18,0,0,19,4,2020);
  tft.begin();
  tft.fillScreen(DISPLAY_BLACK);
  clk.setPointerLength(32, 40, 48);  //pointer length: hour, minute, second
  clk.setPointerWidth(3, 2, 2);  //pointer width: hour, minute, second
  clk.setPointerColor(DISPLAY_RED, DISPLAY_GREEN, DISPLAY_BLUE);  //pointer color: hour, min, second
  clk.setClockDial(0, 0, 60);  //clock position x, y, clock radius
  clk.setDialColor(DISPLAY_WHITE, DISPLAY_BLACK);  //clock border volor, background
  clk.showDial();  //draw dial
}


void loop(void)
{
  clk.updateClock(hour(), minute(), second());
}