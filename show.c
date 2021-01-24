#include "show.h"
#include <math.h>

void show(struct fourier *f, unsigned int length)
{
    initscr();

    for (unsigned int i = 0; i < COLS; i++)
    {
	unsigned int index = i * length / COLS;
	unsigned int x = LINES / 2 + LINES / 2 * cos(f[i].freq * f[i].bias) * f[i].ampl * 100;
	if (x >= LINES)
	{
	    x = LINES - 1;
	}
	else if (x < 0)
	{
	    x = 0;
	}

	move(x, i);
	addch('#');
    }
    refresh();
    
    getch();
    endwin();
}

