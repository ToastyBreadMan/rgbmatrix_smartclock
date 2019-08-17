#include "../rpi-rgb-led-matrix/include/led-matrix.h"
#include "../rpi-rgb-led-matrix/include/graphics.h"
#include "../include/clock.h"
#include "../include/weather.h"
#include "../include/config.h"

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
	Weather weather(OPEN_WEATHER_CITY, OPEN_WEATHER_KEY);
	Clock clock;

	// Give rgb_matrix it's options, we must feed the beast
	if(!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options))
		return usage(argv[0]);
	
	// Create matrix and offscreen buffer
	RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_options);
	FrameCanvas *offscreen = matrix->CreateFrameCanvas();
	
	// Hande our interrupts
	signal(SIGTERM, interruptHandler);
	signal(SIGINT, interruptHandler);

	// Stuff for handling time
	struct timespec next_time;
	next_time.tv_sec = time(NULL);
	next_time.tv_nsec = 0;

	weather.update();

	// Main draw loop
	while(!interrupt_signal)
	{
		offscreen->Clear();

		clock.draw(offscreen, next_time);
		weather.draw(offscreen);
	
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
