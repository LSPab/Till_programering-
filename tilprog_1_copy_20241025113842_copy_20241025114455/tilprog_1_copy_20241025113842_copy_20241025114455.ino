/*
* Name: clock and temp project
* Author: Leo Santaella Prenne
* Date: 2024-10-25
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/


//Library
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include "U8glib.h"

//Constants 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Global objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); // U8glib OLED object
Servo myServo;

void setup() {
  Serial.begin(9600);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC could not start!");
    while (1);
  }
  
  //Servo pin
  myServo.attach(9);
}

void loop() {
  String currentTime = getTime();
  float temperature = getTemp();

  //Update OLED display
  oledWrite(currentTime, temperature);

  //Updatera servo degrees based on temperature
  servoWrite(temperature);

  delay(1000); //Wait 1 second before updating again
}

//Function to get the time from clock module in string
String getTime() {
  DateTime now = rtc.now();
  String timeString = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  return timeString;
}

//Get temperature in Celcius from DSD1323
float getTemp() {
  return rtc.getTemperature();
}

//Print temperature and time on oled display
void oledWrite(String time, float temperature) {
  // U8glib display setup
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);         //Font
    u8g.setPrintPos(0, 20);                //Position for time display
    u8g.print("Time: " + time);            //Print time on display

    u8g.setPrintPos(0, 40);                //Position for temperature display
    u8g.print("Temp: " + String(temperature, 1) + " C"); //Print temperature
  } while (u8g.nextPage());
}

//Function to controll servo degree based on temperature
void servoWrite(float temperature) {
  int angle = map(temperature, -40, 85, 0, 180); //Adjust DS3231 range: -40 to 85°C
  angle = constrain(angle, 0, 180);
  myServo.write(angle);
}