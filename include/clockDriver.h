#include <ctime>
#include "led-matrix.h"
#include "graphics.h"

public class clockDriver()
{

	// __     __         _       _     _
	// \ \   / /_ _ _ __(_) __ _| |__ | | ___  ___
	//  \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __|
	//   \ V / (_| | |  | | (_| | |_) | |  __/\__ \
	//    \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
	//
	private int clock_x; // x start location for the clock display
	private int clock_y; // y start location for the clock display

	private int day_x; // 
	private int day_y; // 

	private int month_x; // 
	private int month_y; // 

	private int year_x; //
	private int year_y; //

	private bool auto_update;

	private time_t last_update;

	rgb_matrix::Font hour_font;
	rgb_matrix::Font minute_font;
	rgb_matrix::Font day_font;
	rgb_matrix::Font month_font;
	rgb_matrix::Font year_font;

	//  _____                 _   _                 ____        __
	// |  ___|   _ _ __   ___| |_(_) ___  _ __     |  _ \  ___ / _|___
	// | |_ | | | | '_ \ / __| __| |/ _ \| '_ \    | | | |/ _ \ |_/ __|
	// |  _|| |_| | | | | (__| |_| | (_) | | | |   | |_| |  __/  _\__ \
	// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___|____/ \___|_| |___/
	//                                        |_____|
	public void setClockX(int x);
	public int getClockX();
	public void setClockY(int y);
	public int getClockY();

	public void setDayX(int x);
	public int getDayX();
	public void setDayY(int y);
	public int getDayY();

	public void setMonthX(int x);
	public int getMonthX();
	public void setMonthY(int y);
	public int getMonthY();

	public void setYearX(int x);
	public int getYearX();
	public void setYearY(int y);
	public int getYearY();

	public void setAutoUpdate(bool option);
	public bool getAutoUpdate();

	public void setFont(rgb_matrix::Font f);
	public void setHourFont(rgb_matrix::Font f);
	public void setMinuteFont(rgb_matrix::Font f);
	public void setDayFont(rgb_matrix::Font f);
	public void setMonthFont(rgb_matrix::Font f);
	public void setYearFont(rgb_matrix::Font f);

	public clockDriver();
	public int drawClock(FrameCanvas *offscreen);

}
