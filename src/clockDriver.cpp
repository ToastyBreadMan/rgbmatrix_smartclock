#include <ctime>
#include "led-matrix.h"
#include "graphics.h"

public class clockDriver()
{
	public void setClockX(int x) { this.clock_x = x; }
	public int getClockX() { return this.clock_x }
	public void setClockY(int y) { this.clock_y = y; }
	public int getClockY() { return this.clock_y }

	public void setDayX(int x) { this.day_x = x; } 
	public int getDayX() { return this.day_x; } 
	public void setDayY(int y) { this.day_y = y; }
	public int getDayY() { return this.day_y; }

	public void setMonthX(int x) { this.month_x = x; }
	public int getMonthX() { return this.month_x; }
	public void setMonthY(int y) { this.month_y = y; }
	public int getMonthY() { return this.month_y; }

	public void setYearX(int x) { this.year_x = x; }
	public int getYearX() { return this.year_x; }
	public void setYearY(int y) { this.year_y = y; }
	public int getYearY() { this.year_y; }

	public bool setFont(std::string f){
		bool ret = true;
		ret = ret && this.minute_font.LoadFont(f.c_str());
		ret = ret && this.hour_font.LoadFont(f.c_str());
		ret = ret && this.day_font.LoadFont(f.c_str());
		ret = ret && this.month_font.LoadFont(f.c_str());
		ret = ret && this.year_font.LoadFont(f.c_str());
		return ret;
	}

	public bool setMinuteFont(std::string f){
		return this.minute_font.Load_Font(f.c_str());
	}
	public bool setMinuteFont(std::string f, bool outline){
		bool ret = true;
		ret = ret && this.minute_font.Load_Font(f.c_str());
		if(outline && ret)
			this.minute_font_outline = minute_font.CreateOutlineFont();
		return ret;
	}

	public bool setHourFont(std::string f){
		return this.hour_font.Load_Font(f.c_str());
	}
	public bool setHourFont(std::string f, bool outline){
		bool ret = true;
		ret = ret && this.hour_font.Load_Font(f.c_str());
		if(outline && ret)
			this.hour_font_outline = hour_font.CreateOutlineFont();
		return ret;
	}

	public bool setDayFont(std::string f){
		return this.day_font.Load_Font(f.c_str())
	}
	public bool setDayFont(std::string f, bool outline){
		bool ret = true;
		ret = ret && this.day_font.Load_Font(f.c_str());
		if(outline)
			this.day_font_outline = day_font.CreateOutlineFont();
		return ret;
	}

	public bool setMonthFont(std::string f){
		return this.month_font.Load_Font(f.c_str());
	}
	public bool setMonthFont(std::string f, bool outline){
		bool ret = true;
		ret = ret && this.month_font.Load_Font(f.c_str());
		if(outline)
			this.month_font_outline = month_font.CreateOutlineFont();
		return ret;
	}

	public bool setYearFont(std::string f){
		return this.year_font.Load_Font(f.c_str());
	}
	public bool setYearFont(std::string f, bool outline){
		bool ret = true;
		ret = ret && this.year_font.Load_Font(f.c_str());
		if(outline)
			this.year_font_outline = year_font.CreateOutlineFont();
		return ret;
	}

	public bool setFont(std::string f){
		bool ret = true;
		ret = ret && this.setMinuteFont(f);
		ret = ret && this.setHourFont(f);
		ret = ret && this.setDayFont(f);
		ret = ret && this.setMonthFont(f);
		ret = ret && this.setYearFont(f);
	}

	public void setAutoUpdate(bool option) { this.auto_update = option; }

	public clockDriver()
	{
		rgb_matrix::Font *hour_font_outline = NULL;
		rgb_matrix::Font *minute_font_outline = NULL;
		rgb_matrix::Font *day_font_outline = NULL;
		rgb_matrix::Font *month_font_outline = NULL;
		rgb_matrix::Font *year_font_outline = NULL;
	}

	public int drawClock(FrameCanvas *offscreen){
		
	}
}
