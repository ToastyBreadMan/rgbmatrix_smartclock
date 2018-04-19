#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

using namespace rgb_matrix;

volatile bool interrupt_signal = false;
static void interruptHandler(int signo){
	interrupt_signal = true;
}

int usage(char *program_name)
{
	printf("Whoops!\n");
	return 1;
}

int main(int argc, char **argv)
{
	RGBMatrix::Options matrix_options;
	rgb_matrix::RuntimeOptions runtime_options;

	if(!rgb_matrix::ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options))
		return usage(argv[0]);
	
	RGBMatrix *matrix = rgb_matrix::CreateMatrixFromOptions(matrix_options, runtime_options);
	//FrameCanvas *offscreen = matrix->CreateFrameCanvas();
	
	signal(SIGTERM, interruptHandler);
	signal(SIGINT, interruptHandler);

	int max_x = matrix->width();
	int max_y = matrix->height();

	while(!interrupt_signal)
	{
		int x = rand() % max_x;	
		int y = rand() % max_y;
		Color color(rand() % 256, rand() % 256, rand() % 256);

		int radius = rand() % 5 + 1;
		
		while(radius != 0)
		{
			DrawCircle(matrix, x, y, radius, color);
			radius--;
		}
		usleep(100000);
	}

	matrix->Clear();
	delete matrix;

	printf("\nYeet\n");
	return 0;
}
