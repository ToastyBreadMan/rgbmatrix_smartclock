#include <ctime>
#include "led-matrix.h"
#include "graphics.h"

class clockDriver
{
	public:
		void setClockX(int x) { this.clock_x = x; }
		int getClockX() { return this.clock_x }
		void setClockY(int y) { this.clock_y = y; }
		int getClockY() { return this.clock_y }

		void setDayX(int x) { this.day_x = x; } 
		int getDayX() { return this.day_x; } 
		void setDayY(int y) { this.day_y = y; }
		int getDayY() { return this.day_y; }

		void setMonthX(int x) { this.month_x = x; }
		int getMonthX() { return this.month_x; }
		void setMonthY(int y) { this.month_y = y; }
		int getMonthY() { return this.month_y; }

		void setYearX(int x) { this.year_x = x; }
		int getYearX() { return this.year_x; }
		void setYearY(int y) { this.year_y = y; }
		int getYearY() { this.year_y; }

		bool setFont(std::string f){
			bool ret = true;
			ret = ret && this.minute_font.LoadFont(f.c_str());
			ret = ret && this.hour_font.LoadFont(f.c_str());
			ret = ret && this.day_font.LoadFont(f.c_str());
			ret = ret && this.month_font.LoadFont(f.c_str());
			ret = ret && this.year_font.LoadFont(f.c_str());
			return ret;
		}

		bool setMinuteFont(std::string f){
			return this.minute_font.Load_Font(f.c_str());
		}
		bool setMinuteFont(std::string f, bool outline){
			bool ret = true;
			ret = ret && this.minute_font.Load_Font(f.c_str());
			if(outline && ret)
				this.minute_font_outline = minute_font.CreateOutlineFont();
			return ret;
		}

		bool setHourFont(std::string f){
			return this.hour_font.Load_Font(f.c_str());
		}
		bool setHourFont(std::string f, bool outline){
			bool ret = true;
			ret = ret && this.hour_font.Load_Font(f.c_str());
			if(outline && ret)
				this.hour_font_outline = hour_font.CreateOutlineFont();
			return ret;
		}

		bool setDayFont(std::string f){
			return this.day_font.Load_Font(f.c_str())
		}
		bool setDayFont(std::string f, bool outline){
			bool ret = true;
			ret = ret && this.day_font.Load_Font(f.c_str());
			if(outline)
				this.day_font_outline = day_font.CreateOutlineFont();
			return ret;
		}

		bool setMonthFont(std::string f){
			return this.month_font.Load_Font(f.c_str());
		}
		bool setMonthFont(std::string f, bool outline){
			bool ret = true;
			ret = ret && this.month_font.Load_Font(f.c_str());
			if(outline)
				this.month_font_outline = month_font.CreateOutlineFont();
			return ret;
		}

		bool setYearFont(std::string f){
			return this.year_font.Load_Font(f.c_str());
		}
		bool setYearFont(std::string f, bool outline){
			bool ret = true;
			ret = ret && this.year_font.Load_Font(f.c_str());
			if(outline)
				this.year_font_outline = year_font.CreateOutlineFont();
			return ret;
		}

		bool setFont(std::string f){
			bool ret = true;
			ret = ret && this.setMinuteFont(f);
			ret = ret && this.setHourFont(f);
			ret = ret && this.setDayFont(f);
			ret = ret && this.setMonthFont(f);
			ret = ret && this.setYearFont(f);
		}

		clockDriver()
		{
			rgb_matrix::Font *hour_font_outline = NULL;
			rgb_matrix::Font *minute_font_outline = NULL;
			rgb_matrix::Font *day_font_outline = NULL;
			rgb_matrix::Font *month_font_outline = NULL;
			rgb_matrix::Font *year_font_outline = NULL;
		}

		int drawClock(FrameCanvas *offscreen, std::string format){
			struct tm nowTime;

			localtime_r(time(NULL), nowTime);
			
		}
}
