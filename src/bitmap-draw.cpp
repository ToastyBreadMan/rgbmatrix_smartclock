#include "../include/bitmap-draw.h"

using namespace rgb_matrix;

// Convert 3 bit color value to 8 bit
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

// Draw bitmap array from img_weather.h
void draw_bmp(FrameCanvas *offscreen, int width, int height, int x_origin, int y_origin, const unsigned char *bitmap) 
{
	int count = 0;


	for(int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
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
				offscreen->SetPixel(x_origin + j, y_origin + i, r, g, b);
			}
			
			count += 2;
		}
	}
}

int DrawBitmap(FrameCanvas *offscreen, int x_origin, int y_origin, int code, bool isNight)
{
	const unsigned char *bitmap;

	// If you see this one, something went wrong
	bitmap = IMG_a15;

	switch (code) {
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
			fprintf(stderr, "ERROR: Weather id %d unknown", code);
			return -1;
	}

	draw_bmp(offscreen, BMP_WIDTH, BMP_HEIGHT, x_origin, y_origin, bitmap);
	return 1;
}
