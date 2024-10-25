//Bibliotek
#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include "U8glib.h"

//Konstanter 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Globala objekt
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
  
  //servo pin
  myServo.attach(9);
}

void loop() {
  String currentTime = getTime();
  float temperature = getTemp();

  //Uppdatera OLED display
  oledWrite(currentTime, temperature);

  //Udatera servo grader baserat på temperatur
  servoWrite(temperature);

  delay(1000); // Wait 1 second before updating again
}

//Function för att få tiden från klockmodulen i en string
String getTime() {
  DateTime now = rtc.now();
  String timeString = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  return timeString;
}

//Ta temperatur i Celcius från DSD1323
float getTemp() {
  return rtc.getTemperature();
}

//Skriv ut temperaturen och tiden på oled display
void oledWrite(String time, float temperature) {
  // U8glib display setup
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);         //Font
    u8g.setPrintPos(0, 20);                //Position för time display
    u8g.print("Time: " + time);            //skriv ut tid på display

    u8g.setPrintPos(0, 40);                //Position för temperatur display
    u8g.print("Temp: " + String(temperature, 1) + " C"); //Skriv ut temperatur
  } while (u8g.nextPage());
}

//Funktion för att kontrollera servo grad baserat på temperatur
void servoWrite(float temperature) {
  int angle = map(temperature, -40, 85, 0, 180); //Justera till DS3231 range: -40 to 85°C
  angle = constrain(angle, 0, 180);
  myServo.write(angle);
}