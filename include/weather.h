#include <string>
#include <curl/curl.h>

#include "../rpi-rgb-led-matrix/include/led-matrix.h"                                                          
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "json.hpp"
#include "bitmap-draw.h"

class Weather
{
	private:
		int weather_id;
		int min_temp;
		int cur_temp;
		int max_temp;
		
		std::string city_id;
		std::string api_key;
	public:
		Weather(std::string, std::string);
		bool update();
		bool draw();
};
