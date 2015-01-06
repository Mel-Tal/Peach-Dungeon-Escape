// A library of functions to use in GBA Programming

#include "mylib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;


/*---------------------Functions---------------------*/

//MANDATORY
void setPixel(int r, int c, unsigned short color)
{
	*(videoBuffer + OFFSET(r, c, 240)) = color;
}

//MANDATORY
void drawRect(int row, int col, int height, int width, unsigned short color)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

//MANDATORY
void drawHollowRect(int row, int col, int width, int height, unsigned short color)
{
	int r,c;
	for (r = row; r < (row + height); r++) {
		setPixel(r, col, color);
		setPixel(r, col + width, color);
	}
	for (c = col; c < (col + width); c++) {
		setPixel(row, c, color);
		setPixel(row + height, c, color);
	}
	setPixel((row + height), (col + width), color);
}

//MANDATORY
void plotLine(int row0, int col0, int row1, int col1, unsigned short color)
{
	int dx, dy, d, x, y;
	
	dy = row1 - row0;
	dx = col1 - col0;
	d = 2 * dy - dx;
	y = row0;
	setPixel(row0, col0, color);

	for (x = (col0 + 1); x <= col1; x++) {
		if (d > 0) {
			y++;
			setPixel(y, x, color);
			d = d + (2 * dy - 2 * dx);
		} else {
			setPixel(y, x, color);
			d = d + (2 * dy);
		}
	}

}

int collision(gameObj a, gameObj b)
{
	if (((a.c + a.w) > b.c) && ((a.c + a.w) < (b.c + b.w))) {
		return 1;
	}

	return 0;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void delay(int x) 
{
	volatile int i = 0;
	while (i < x) {
		i++;
	}
}

void drawImage3(int row, int col, int width, int height, const unsigned short* pic)
{
	int r;
	for(r=0; r<height; r++)
	{
		DMA[3].src = pic + (r * width);
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_ON;
	}
}

void erase(gameObj o)
{
	drawRect(o.r, o.c, o.h, o.w, BLACK);
}


