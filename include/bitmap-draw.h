#include "../rpi-rgb-led-matrix/include/led-matrix.h"
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "img-weather.h"

#define BMP_HEIGHT 25
#define BMP_WIDTH 25

using namespace rgb_matrix;

// Convert compressed bmp color to 8 bit color
int convert_color(int);

// Draw bitmap in img_weather.h file
void draw_bmp(FrameCanvas *offscreen, int width, int height, int x_origin, int y_origin, const unsigned char *bitmap);

// Draw icon based on OpenWeatherMap weather id
int DrawBitmap(FrameCanvas *offscreen, int x_origin, int y_origin, int code, bool isNight);
