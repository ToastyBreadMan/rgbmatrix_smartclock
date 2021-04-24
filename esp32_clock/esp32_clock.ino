#include <PxMatrix.h>
#include <WiFi.h>
#include <ezTime.h>
#include <HTTPClient.h>
#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "include/GFX_fonts/GFX_fonts/Font5x7Fixed.h"

//---Danger Zone---
#include "include/key.h"
//-----------------

//---Time---
#define MYTIMEZONE "America/New_York"
const bool TWELVEHOUR = true;
//----------

//---Weather---
const char openweatherendpoint[] = "http://api.openweathermap.org/data/2.5/weather?q=";
const char appid[] = "&appid=";
const char openweatherunits[] = "&units=imperial";
const char testjson[] = "{\"coord\":{\"lon\":-83.9207,\"lat\":35.9606},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":42.46,\"feels_like\":42.46,\"temp_min\":39.2,\"temp_max\":44.6,\"pressure\":1022,\"humidity\":87},\"visibility\":10000,\"wind\":{\"speed\":2.98,\"deg\":80,\"gust\":4.52},\"clouds\":{\"all\":1},\"dt\":1619151751,\"sys\":{\"type\":1,\"id\":3818,\"country\":\"US\",\"sunrise\":1619175115,\"sunset\":1619223336},\"timezone\":-14400,\"id\":4634946,\"name\":\"Knoxville\",\"cod\":200}";
//-------------

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

struct weatherStruct {
  bool updated;
  int id;
  double cur_temp;
  double min_temp;
  double max_temp;
  int humidity;
  int64_t sunrise;
  int64_t sunset;
};

struct weatherStruct getWeather(char *url) {
  HTTPClient http;
  weatherStruct weather;

  weather.updated = false;

  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();

  if(httpCode != 200){
    Serial.print("HTTP GET failed with code:");
    Serial.println(httpCode);
    return weather;
  }
  
  DynamicJsonDocument doc(24576);
  DeserializationError error = deserializeJson(doc, payload.c_str(), payload.length());

  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return weather;
  }
  
  JsonObject weather_0 = doc["weather"][0];
  if(weather_0 != NULL) {
    int weather_0_id = weather_0["id"];
    if(weather_0_id != NULL) {
      weather.id = weather_0_id;
    }
  } else { 
    Serial.println("No weather...");
    return weather;
  }
  
  JsonObject main = doc["main"];
  if(main != NULL) {
    
    weather.cur_temp = main["temp"];
    weather.min_temp = main["temp_min"];
    weather.max_temp = main["temp_max"];
    weather.humidity = main["humidity"];
    
  } else { 
    Serial.println("No main...");
    return weather;
  }

  JsonObject sys = doc["sys"];
  if(sys != NULL){
    weather.sunrise = sys["sunrise"];
    weather.sunset = sys["sunset"];
  } else {
    Serial.println("No sys...");
    return weather;    
  }

  weather.updated = true;
  http.end();
  
  return weather;
}

void displayWeather(char *url) {
  weatherStruct weather;

  Serial.println("---Get Weather From---");
  Serial.println(url);
  Serial.println("----------------------");
  
  weather = getWeather(url);
  if(!weather.updated){
    Serial.println("Weather Failed to update!");
    return;
  }
  
  Serial.println("---Weather_Data---");
  Serial.print("Weather id = ");
  Serial.println(weather.id);
  Serial.print("Weather cur_temp = ");
  Serial.println(weather.cur_temp);
  Serial.print("Weather max_temp = ");
  Serial.println(weather.max_temp);
  Serial.print("Weather min_temp = ");
  Serial.println(weather.min_temp);
  Serial.print("Weather humidity = ");
  Serial.println(weather.humidity);
  Serial.print("Weather sunrise = ");
  Serial.println((int)weather.sunrise);
  Serial.print("Weather sunset = ");
  Serial.println((int)weather.sunset);
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

  char *url = (char*)calloc(1, sizeof(openweatherendpoint) + sizeof(openweathercityid) + sizeof(appid) + sizeof(openweatherapi) + sizeof(openweatherunits)+1);
  sprintf(url, "%s%s%s%s%s", openweatherendpoint, openweathercityid, appid, openweatherapi, openweatherunits);
  displayWeather(url);
  free(url);

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
