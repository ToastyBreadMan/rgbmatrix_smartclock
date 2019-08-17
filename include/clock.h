#include <string>
#include <curl/curl.h>
#include <time.h>

#include "../rpi-rgb-led-matrix/include/led-matrix.h"
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "config.h"

class Clock {
	private:
		const std::string clock_time_fmt = CLOCK_FMT;
		const int clock_x_orig = CLOCK_X_ORIGIN;
		const int clock_y_orig = CLOCK_Y_ORIGIN;

	    const std::string clock_font_str = CLOCK_FONT;
		rgb_matrix::Font clock_font;

		const std::string date_time_fmt_weekday = DATE_WEEKDAY_FMT;
		const int date_x_orig_weekday = DATE_WEEKDAY_X_ORIGIN;
		const int date_y_orig_weekday = DATE_WEEKDAY_Y_ORIGIN;

		const std::string date_time_fmt_month = DATE_MONTH_FMT;
		const int date_x_orig_month = DATE_MONTH_X_ORIGIN;
		const int date_y_orig_month = DATE_MONTH_Y_ORIGIN;

		const std::string date_time_fmt_day = DATE_DAY_FMT;
		const int date_x_orig_day = DATE_DAY_X_ORIGIN;
		const int date_y_orig_day = DATE_DAY_Y_ORIGIN;

		const std::string date_font_str = DATE_FONT;
		rgb_matrix::Font date_font;

	public:
		Clock();
		bool draw(rgb_matrix::FrameCanvas*, struct timespec);
};
