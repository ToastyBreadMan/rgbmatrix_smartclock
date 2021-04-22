#include <PxMatrix.h>
#include <WiFi.h>
#include <ezTime.h>
#include "include/GFX_fonts/GFX_fonts/Font5x7Fixed.h"

//---Danger Zone---
#include "include/key.h"
//-----------------

#define MYTIMEZONE "America/New_York"
bool TWELVEHOUR = true;

// Pins for LED MATRIX
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 21
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define matrix_width 64
#define matrix_height 32

uint8_t display_draw_time=60; //30-70 is usually fine
Timezone TZ;

PxMATRIX display(64,32,P_LAT,P_OE,P_A,P_B,P_C,P_D);

uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable)
{
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 4000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(16);

  delay(500);

  Serial.print("Serial Begin\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  TZ.setLocation(F(MYTIMEZONE));
  ezt::setInterval(300);
  ezt::setDebug(INFO);

  waitForSync();
  updateNTP();
  display_update_enable(true);
}


void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();

  String t = TZ.dateTime("D H~:i");
  display.setCursor(2,10);
  display.setFont(&Font5x7Fixed);
  display.print(t);

  events();
  delay(500);
}
