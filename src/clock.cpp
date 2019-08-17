#include "../include/clock.h"

Clock::Clock() {
	
    if (!this->date_font.LoadFont(this->date_font_str.c_str())) {
        fprintf(stderr, "Font file failed to load");
    }

	rgb_matrix::Font clock_font;
	if (!this->clock_font.LoadFont(this->clock_font_str.c_str())) {
        fprintf(stderr, "Font file failed to load");
    }

}

bool Clock::draw(rgb_matrix::FrameCanvas *offscreen, struct timespec next_time) {

	char text_buf[256];
	struct tm tm;

	rgb_matrix::Color clock_color(CLOCK_COLOR_R, CLOCK_COLOR_G, CLOCK_COLOR_B);
	rgb_matrix::Color date_color(DATE_COLOR_R, DATE_COLOR_G, DATE_COLOR_B);

	localtime_r(&next_time.tv_sec, &tm);

	try {
		// Draw Time
		strftime(text_buf, sizeof(text_buf), this->clock_time_fmt.c_str(), &tm);
		rgb_matrix::DrawText(offscreen, clock_font, clock_x_orig, clock_y_orig + clock_font.baseline(),
				clock_color, NULL, text_buf, 0);

		// Draw weekday
		strftime(text_buf, sizeof(text_buf), this->date_time_fmt_weekday.c_str(), &tm);
		rgb_matrix::DrawText(offscreen, this->date_font, this->date_x_orig_weekday,
				this->date_y_orig_weekday + this->date_font.baseline(), date_color, NULL, text_buf, 0);

		// Draw month
		strftime(text_buf, sizeof(text_buf), this->date_time_fmt_month.c_str(), &tm);
		rgb_matrix::DrawText(offscreen,this->date_font, this->date_x_orig_month,
				this->date_y_orig_month + this->date_font.baseline(), date_color, NULL, text_buf, 0);

		// Draw day
		strftime(text_buf, sizeof(text_buf), this->date_time_fmt_day.c_str(), &tm);
		rgb_matrix::DrawText(offscreen,this->date_font, this->date_x_orig_day,
				this->date_y_orig_day + this->date_font.baseline(), date_color, NULL, text_buf, 0);

	}
	catch (const std::exception& e) {
		return false;
	}

	return true;
}
