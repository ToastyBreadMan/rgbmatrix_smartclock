#include <string>
#include <curl/curl.h>

#include "../rpi-rgb-led-matrix/include/led-matrix.h"                                                          
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "json.hpp"
#include "bitmap-draw.h"

class Weather
{
	private:
		bool update_success;
		bool isNight;

		int weather_id;
		int min_temp;
		int cur_temp;
		int max_temp;
		
		int city_id;
		std::string api_key;

		const std::string font = "rpi-rgb-led-matrix/fonts/5x7.bdf";
		const int temp_x_orig = 6;
		const int temp_y_orig = 24;
		rgb_matrix::Font temp_font;
	public:
		Weather(int, std::string);
		bool update();
		bool draw(FrameCanvas*);
};
