#include "../rpi-rgb-led-matrix/include/led-matrix.h"
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "../include/bitmap-draw.h"

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

using namespace rgb_matrix;

// Handle interrupts well
volatile bool interrupt_signal = false;
static void interruptHandler(int signo){
	interrupt_signal = true;
}

// TODO: Make a better usage function
int usage(char *program_name)
{
	printf("Whoops!\n");
	return 1;
}

int main(int argc, char **argv)
{
	RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_options;

	// Give rgb_matrix it's options, we must feed the beast
	if(!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options))
		return usage(argv[0]);
	
	// Create matrix and offscreen buffer
	RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_options);
	FrameCanvas *offscreen = matrix->CreateFrameCanvas();
	
	// Hande our interrupts
	signal(SIGTERM, interruptHandler);
	signal(SIGINT, interruptHandler);

	// Clock variables
	const char *clock_time_fmt = "%H:%M";
	const Color clock_color(189,147,249);
	const int clock_x_orig = 7;
	const int clock_y_orig = 5;
	rgb_matrix::Font clock_font;
	if (!clock_font.LoadFont("rpi-rgb-led-matrix/fonts/10x20.bdf")) {
		fprintf(stderr, "Font file failed to load");
		return 1;
	}

	// Date variables
	const char *date_time_fmt = "%a %b %d";
	const Color date_color(241,250,140);
	const int date_x_orig = 7;
	const int date_y_orig = 0;
	rgb_matrix::Font date_font;
	if (!date_font.LoadFont("rpi-rgb-led-matrix/fonts/5x8.bdf")) {
		fprintf(stderr, "Font file failed to load");
		return 1;
	}

	// Stuff for handling time
	char text_buf[256];	
	struct timespec next_time;
	next_time.tv_sec = time(NULL);
	next_time.tv_nsec = 0;
	struct tm tm;

	while(!interrupt_signal)
	{
		localtime_r(&next_time.tv_sec, &tm);
		
		offscreen->Clear();

		// Draw Time
		strftime(text_buf, sizeof(text_buf), clock_time_fmt, &tm);
		rgb_matrix::DrawText(offscreen, clock_font, clock_x_orig, clock_y_orig + clock_font.baseline(),
							 clock_color, NULL, text_buf, 0);

		// Draw Date
		strftime(text_buf, sizeof(text_buf), date_time_fmt, &tm);
		rgb_matrix::DrawText(offscreen, date_font, date_x_orig, date_y_orig + date_font.baseline() - 1,
							 date_color, NULL, text_buf, 0);
		
		DrawBitmap(offscreen, 0, -1, 800, false);

		// Wait till showing new clock
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_time, NULL);

		// Swap what is being displayed
		offscreen = matrix->SwapOnVSync(offscreen);

		next_time.tv_sec += 1;
							 
	}

	matrix->Clear();
	delete matrix;

	printf("\nYeet\n");
	return 0;
}
