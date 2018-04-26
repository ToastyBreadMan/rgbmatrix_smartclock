#include <ctime>
#include "led-matrix.h"
#include "graphics.h"

public class clockDriver()
{
	public void setClockX(int x){ this.clock_x = x; }
	public int getClockX(){ return this.clock_x }
	public void setClockY(int y){ this.clock_y = y; }
	public int getClockY(){ return this.clock_y }

	public void setDayX(int x){ this.day_x = x; } 
	public int getDayX(){ return this.day_x; } 
	public void setDayY(int y){ this.day_y = y; }
	public int getDayY(){ return this.day_y; }

	public void setMonthX(int x){ this.month_x = x; }
	public int getMonthX(){ return this.month_x; }
	public void setMonthY(int y){ this.month_y = y; }
	public int getMonthY(){ return this.month_y; }

	public void setYearX(int x){ this.year_x = x; }
	public int getYearX(){ return this.year_x; }
	public void setYearY(int y){ this.year_y = y; }
	public int getYearY(){ this.year_y; }

	public void setAutoUpdate(bool option) { this.auto_update = option; }

	public clockDriver();

	public int drawClock(FrameCanvas *offscreen);
}
