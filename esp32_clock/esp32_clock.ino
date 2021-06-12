#define PxMATRIX_double_buffer true
#include <PxMatrix.h>
#include <WiFi.h>
#include <ezTime.h>
#include <HTTPClient.h> 
#define ARDUINOJSON_USE_DOUBLE 1
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "include/GFX_fonts/GFX_fonts/Font5x7Fixed.h"
#include "include/GFX_fonts/GFX_fonts/Font4x5Fixed.h"

//---Danger Zone---
#include "include/key.h"
//#define debug
//-----------------

//---Time---
#define MYTIMEZONE "America/New_York"
#define TWELVEHOUR true;
#define DAY_FONT Font5x7Fixed
#define TIME_FONT Font5x7Fixed
#define DATE_FONT Font4x5Fixed

//----------

//---Weather---
#include "include/image-weather.h"
#define BITMAP_HEIGHT 25
#define BITMAP_WIDTH 25

#define BITMAP_X_OFFSET 0
#define BITMAP_Y_OFFSET 0

#define TEMP_X_OFFSET 4
#define TEMP_Y_OFFSET 27
#define TEMP_FONT Font4x5Fixed

const char openweatherendpoint[] = "http://api.openweathermap.org/data/2.5/weather?q=";
const char appid[] = "&appid=";
const char openweatherunits[] = "&units=imperial";
const char testjson[] = "{\"coord\":{\"lon\":-83.9207,\"lat\":35.9606},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01n\"}],\"base\":\"stations\",\"main\":{\"temp\":42.46,\"feels_like\":42.46,\"temp_min\":39.2,\"temp_max\":44.6,\"pressure\":1022,\"humidity\":87},\"visibility\":10000,\"wind\":{\"speed\":2.98,\"deg\":80,\"gust\":4.52},\"clouds\":{\"all\":1},\"dt\":1619151751,\"sys\":{\"type\":1,\"id\":3818,\"country\":\"US\",\"sunrise\":1619175115,\"sunset\":1619223336},\"timezone\":-14400,\"id\":4634946,\"name\":\"Knoxville\",\"cod\":200}";
char *urls [5];

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
weatherStruct weatherStructs[5];
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

uint8_t display_draw_time=30; //30-70 is usually fine
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

// __        __         _   _                 _____                 _   _
// \ \      / /__  __ _| |_| |__   ___ _ __  |  ___|   _ _ __   ___| |_(_) ___  _ __  ___
//  \ \ /\ / / _ \/ _` | __| '_ \ / _ \ '__| | |_ | | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//   \ V  V /  __/ (_| | |_| | | |  __/ |    |  _|| |_| | | | | (__| |_| | (_) | | | \__ \
//    \_/\_/ \___|\__,_|\__|_| |_|\___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

void getWeather(int weatherIndex) {
  char *url;
  HTTPClient http;
  weatherStruct *weather;

  if(weatherIndex > 5 || weatherIndex < 0) {
    return;
  }

  url = urls[weatherIndex];
  
  if(url == NULL) {
    return;
  }
  
  weather = &weatherStructs[weatherIndex];
  weather->updated = false;

#ifdef debug
  String payload = testjson;
  int httpCode = 200;
#else
  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();
#endif

  if(httpCode != 200){
    Serial.print("HTTP GET failed with code:");
    Serial.println(httpCode);
    return;
  }

  DynamicJsonDocument doc(24576);
  DeserializationError error = deserializeJson(doc, payload.c_str(), payload.length());

  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  JsonObject weather_0 = doc["weather"][0];
  if(weather_0 != NULL) {
    int weather_0_id = weather_0["id"];
    if(weather_0_id != NULL) {
      weather->id = weather_0_id;
    }
  } else { 
    Serial.println("No weather...");
    return;
  }
  
  JsonObject main = doc["main"];
  if(main != NULL) {
    
    weather->cur_temp = main["temp"];
    weather->min_temp = main["temp_min"];
    weather->max_temp = main["temp_max"];
    weather->humidity = main["humidity"];
    
  } else { 
    Serial.println("No main...");
    return;
  }

  JsonObject sys = doc["sys"];
  if(sys != NULL){
    weather->sunrise = sys["sunrise"];
    weather->sunset = sys["sunset"];
  } else {
    Serial.println("No sys...");
    return;    
  }

  weather->updated = true;
  http.end();
  
  return;
}

void getWeatherEvent() {
  for(int i=0; i<5; i++) {
    char *url = urls[i];
    if(url == NULL) { break; }
    
    getWeather(i);
#ifdef debug
    Serial.println("---Weather_Data---");
    Serial.println("Got data from:");
    Serial.println(url);
    Serial.print("Weather id = ");
    Serial.println(weatherStructs[i].id);
    Serial.print("Weather cur_temp = ");
    Serial.println(weatherStructs[i].cur_temp);
    Serial.print("Weather max_temp = ");
    Serial.println(weatherStructs[i].max_temp);
    Serial.print("Weather min_temp = ");
    Serial.println(weatherStructs[i].min_temp);
    Serial.print("Weather humidity = ");
    Serial.println(weatherStructs[i].humidity);
    Serial.print("Weather sunrise = ");
    Serial.println((int)weatherStructs[i].sunrise);
    Serial.print("Weather sunset = ");
    Serial.println((int)weatherStructs[i].sunset);
    Serial.println("------------------");
#endif
  }

  time_t t_now = TZ.now();
  Serial.print("Now Event: ");
  Serial.println((unsigned long)t_now);
  // Get previous half hour, then add 30 minutes to it
  time_t next_event = 1800 + (t_now - (t_now % 1800));

  Serial.print("Next Event: ");
  Serial.println((unsigned long)next_event);
  TZ.setEvent(getWeatherEvent, next_event);
}

void displayWeather(int weatherIndex) {
  weatherStruct weather;
  char *url;

  if(weatherIndex > 5 || weatherIndex < 0) {
    return;
  }

  url = urls[weatherIndex];
  
  if(url == NULL) {
    return;
  }
  
  weather = weatherStructs[weatherIndex];

  //weather = getWeather(url);
  //if(!weather.updated){
  //  Serial.println("Weather Failed to update!");
  //  return;
  //}

  time_t t_now = TZ.now();
  bool is_night = (t_now > weather.sunset || t_now < weather.sunrise);
  
  if(displayWeatherIcon(weather.id, BITMAP_X_OFFSET, BITMAP_Y_OFFSET, is_night)){
    Serial.println("Displaying weather icon failed"); 
  }
  displayWeatherTemp(weather.cur_temp, TEMP_X_OFFSET, TEMP_Y_OFFSET);
}

int convert_color(int c)
{
  switch (c) {
    case 0:
      return 0;
    case 1:
      return 32;
    case 2:
      return 64;
    case 3:
      return 96;
    case 4:
      return 128;
    case 5:
      return 160;
    case 6:
      return 192;
    case 7:
      return 255;
  }

  return 0;
}

void draw_bmp(int xoffset, int yoffset, const unsigned char *bitmap) {

  int count = 0;

  for(int i=0; i<BITMAP_HEIGHT; i++) {
    for (int j=0; j<BITMAP_WIDTH; j++) {
      int r, g, b, a;
      char byte1, byte2;

      byte1 = bitmap[count];
      byte2 = bitmap[count+1];

      b = (byte1 >> 3) & 0x07;
      g = (byte1) & 0x07;
      r = (byte2 >> 3) & 0x07;
      a = (byte2) & 0x07;

      r = convert_color(r);
      g = convert_color(g);
      b = convert_color(b);

      if (a != 0) {
        display.drawPixel(xoffset + j, yoffset + i, display.color565(r,g,b));
      }
      
      count += 2;
    }
  }
}

int displayWeatherIcon(int id, int xoffset, int yoffset, bool isNight) {
  const unsigned char *bitmap;

  // If you see this one, something went wrong
  bitmap = IMG_a15;

  switch (id) {
    case 200: // thunderstorm with light rain
    case 201: // thunderstorm with rain
    case 202: // thunderstorm with heavy rain
      bitmap = IMG_a26;
      break;
    case 210: // light thunderstorm 
    case 211: // thunderstorm
    case 212: // heavy thunderstorm
    case 221: // ragged thunderstorm
    case 230: // thunderstorm with light drizzle
    case 231: // thunderstorm with drizzle
    case 232: // thunderstorm with heavy drizzle
      bitmap = isNight ? IMG_a29 : IMG_a30;
      break;

    case 300: // light intensity drizzle
    case 301: // drizzle
    case 302: // heavy intensity drizzle
    case 310: // light intensity drizzle rain
    case 311: // drizzle rain 
    case 312: // heavy intensity drizzle rain
    case 313: // shower rain and drizzle
    case 314: // heavy shower rain and drizzle
    case 321: // shower drizzle
      bitmap = IMG_a20;
      break;

    case 500: // light rain 
      bitmap = isNight ? IMG_a36 : IMG_a20;
      break;
    case 501: // moderate rain 
      bitmap = isNight ? IMG_a36 : IMG_a21;
      break;
    case 502: // heavy intensity rain
      bitmap = isNight ? IMG_a36 : IMG_a22;
      break;
    case 503: // very heavy rain
      bitmap = IMG_a23;
      break;
    case 504: // extreme rain
      bitmap = IMG_a24;
      break;
    case 511: // freezing rain
      bitmap = isNight ? IMG_a39 : IMG_a04;
      break;
    case 520: // light intensity shower rain 
    case 521: // shower rain
      bitmap = isNight ? IMG_a40 : IMG_a12;
      break;
    case 522: // heavy intensity shower rain
    case 531: // ragged shower rain
      bitmap = isNight ? IMG_a41 : IMG_a13;
      break;

    case 600: // light snow
      bitmap = isNight ? IMG_a37 : IMG_a31;
      break;
    case 601: // Snow
      bitmap = isNight ? IMG_a37 : IMG_a32;
      break;
    case 602: // Heavy snow
      bitmap = IMG_a33;
      break;
    case 611: // Sleet
    case 612: // Light shower sleet
    case 613: // Shower sleet
    case 615: // Light rain and snow 
    case 616: // Rain and snow
    case 620: // Light shower snow
    case 621: // Shower snow
    case 622: // Heavy shower snow
      bitmap = isNight ? IMG_a39 : IMG_a04;
      break;

    case 701: // mist
      bitmap = IMG_a06;
      break;
    case 711: // Smoke
    case 721: // Haze
    case 731: // sand/ dust whirls
      bitmap = IMG_a07;
      break;
    case 741: // fog
      bitmap = IMG_a16;
      break;
    case 751: // sand
    case 761: // dust
      bitmap = IMG_a07;
      break;
    case 762: // volcanic ash
    case 771: // squalls
    case 781: // tornado
      bitmap = IMG_a35;
      break;
      
    case 800: // clear sky
    case 801: // few clouds: 11-25%
      bitmap = isNight ? IMG_a08 : IMG_a01;
      break;
    case 802: // scattered clouds: 25-50%
      bitmap = isNight ? IMG_a09 : IMG_a02;
      break;
    case 803: // broken clouds: 51-84%
      bitmap = isNight ? IMG_a10 : IMG_a03;
      break;
    case 804: // overcast clouds: 85-100%
      bitmap = IMG_a19;
      break;

    default:
      fprintf(stderr, "ERROR: Weather id %d unknown", id);
      return 1;
  }

  draw_bmp(xoffset, yoffset, bitmap);
  return 0;
}

void displayWeatherTemp(double temp, int xoffset, int yoffset) {
  char buff[32];

  display.setFont(&TEMP_FONT);
  memset(buff, 0, sizeof(buff));
  snprintf(buff, sizeof(buff), "%.1fF", temp);

  display.setCursor(xoffset, yoffset);
  display.print(buff);
}

void setup() {
  // put your setup code here, to run once:
  for (int i=0; i<5; i++) { urls[i] = NULL; }
  
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
#ifdef debug
  ezt::setInterval(300);
  ezt::setDebug(INFO);
#endif

  int url_size = sizeof(openweatherendpoint) + sizeof(openweathercityid) + sizeof(appid) + sizeof(openweatherapi) + sizeof(openweatherunits)+1;
  char *url = (char*)calloc(1, url_size);
  snprintf(url, url_size, "%s%s%s%s%s", openweatherendpoint, openweathercityid, appid, openweatherapi, openweatherunits);
  urls[0] = url;

  waitForSync();
  getWeatherEvent();
  display_update_enable(true);
}


void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();

  String time_day = TZ.dateTime("D");
  String time_hour = TZ.dateTime("H~:i");
  String time_date = TZ.dateTime("d~/m~/y");

  display.setFont(&DAY_FONT);
  display.setCursor(39, 8);
  display.print(time_day);

  //display.setFont(&TIME_FONT);
  display.setCursor(34, 16);
  display.print(time_hour);

  display.setFont(&DATE_FONT);
  display.setCursor(32, 27);
  display.print(time_date);

  displayWeather(0);
  display.showBuffer();

  events();
  delay(1000);
}
