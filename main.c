#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "text.h"
#include "Bricks.h"
#include "DungeonDoor.h"
#include "winner.h"
#include "peach.h"
#include "title.h"

int main() 
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	int gameStarted = 0;
	gameObj pch = {83, 48, PEACH_HEIGHT, PEACH_WIDTH, 0, 0};
	
	int jumping = 0;
	int jmpCnt = 0;
	int doorR = 80;
	int door1C = 96;
	int door2C = 144;
	int door3C = 192;
	room rooms[5];
	room current = {-1, {-1, pch}, {-1, pch}, {-1, pch}};
	gameObj firstO = {doorR, door1C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
	gameObj secondO = {doorR, door2C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
	gameObj thirdO = {doorR, door3C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
			

	while(1) {

		if (!gameStarted) {

			int x = 0;

			while (!KEY_DOWN_NOW(BUTTON_START))
			{
				drawImage3(0, 0, TITLE_WIDTH, TITLE_HEIGHT, title);
				x++;
			}

			drawRect(0, 0, 160, 250, BLACK);
			srand(x);

			int y;
			for (y = 0; y < 5; y++) {
				gameObj go1 = {doorR, door1C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
				gameObj go2 = {doorR, door2C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
				gameObj go3 = {doorR, door3C, DUNGEONDOOR_HEIGHT, DUNGEONDOOR_WIDTH, 0, 0};
				door d1 = {rand()%5, go1};
				door d2 = {rand()%5, go2};
				door d3 = {rand()%5, go3};
				room r;
				r.num = y;
				r.d1 = d1;
				r.d2 = d2;
				r.d3 = d3;
				rooms[y] = r;
			}

			rooms[4].d3.target = 6;

			current = rooms[0];
		}

		door first = current.d1;
		door second = current.d2;
		door third = current.d3;

		char buffer [4];
		sprintf(buffer, "%d", current.num);
		drawString(0, 0, "ROOM: ", WHITE);
		drawRect(0, 30, 7, 90, BLACK);
		drawString(0, 30, buffer, WHITE);
/*		sprintf(buffer, "%d", first.target);
		drawString(0, 50, buffer, WHITE);
		sprintf(buffer, "%d", second.target);
		drawString(0, 60, buffer, WHITE);
		sprintf(buffer, "%d", third.target);
		drawString(0, 70, buffer, WHITE);
*/
		gameStarted = 1;

		int i, j;
		for(i = 112; i <= 144; i += 16) {
			for (j = 0; j <= 224; j += 16) {
				drawImage3(i, j, BRICKS_WIDTH, BRICKS_HEIGHT, Bricks);
			}

		}

//		drawImage3(112, 0, BRICKS_WIDTH, BRICKS_HEIGHT, Bricks);

//		drawImage3(63, 75, GOOMBA_WIDTH, GOOMBA_HEIGHT, Goomba);
		drawImage3(doorR, door1C, DUNGEONDOOR_WIDTH, DUNGEONDOOR_HEIGHT, DungeonDoor);
		drawImage3(doorR, door2C, DUNGEONDOOR_WIDTH, DUNGEONDOOR_HEIGHT, DungeonDoor);
		drawImage3(doorR, door3C, DUNGEONDOOR_WIDTH, DUNGEONDOOR_HEIGHT, DungeonDoor);

		drawImage3(pch.r, pch.c, pch.w, pch.h, peach);


/*		drawRect(11, 51, 10, 40, BLACK);
		if (collision(pch, firstO)) {
			drawRect(12, 51, 8, 8, RED);
		}

		if (collision(pch, secondO)) {
			drawRect(12, 61, 8, 8, RED);
		}

		if (collision(pch, thirdO)) {
			drawRect(12, 71, 8, 8, RED);
		}
*/
		if (KEY_DOWN_NOW(BUTTON_RIGHT)){
			pch.dc = 1;
			if (!jumping) {
				pch.dr = -1;
				jmpCnt = 0;
				jumping = 1;
			}
		} else if (KEY_DOWN_NOW(BUTTON_LEFT)) {
			pch.dc = -1;
			if (!jumping) {
				pch.dr = -1;
				jmpCnt = 0;
				jumping = 1;
			}
		}

		
		if (KEY_DOWN_NOW(BUTTON_UP)) {
			int go = 0;
			int collided = 0;
			if (collision(pch, firstO)) {
				go = first.target;
				collided = 1;
			} else if (collision(pch, secondO)) {
				go = second.target;
				collided = 1;
			} else if (collision(pch, thirdO)) {
				go = third.target;
				collided = 1;
			}

//			sprintf(buffer, "%d", go);
//			drawRect(10, 100, 10, 10, BLACK);
//			drawString(10, 100, buffer, WHITE);
//			drawRect(10, 10, 10, 10, BLACK);

			if ((go != 6) && (collided)) {
					current = rooms[go];
					erase(pch);
					pch.r = 83;
					pch.c = 48;
					pch.dr = 0;
					pch.dc = 0;
//					sprintf(buffer, "%d", current.num);
//					drawString(0, 100, buffer, WHITE);

			} else if ((go == 6) && (collided)){
				drawImage3(0, 0, WINNER_WIDTH, WINNER_HEIGHT, winner);
				int i;
				for (i = 0; i < 50; i++) {
					delay(10000);
				}
				main();
			}
		}

		if (jumping) {
			if (jmpCnt >= 12){
				pch.dr = 0;
				jmpCnt = 0;
			} else if (jmpCnt >= 6) {
				pch.dr = 1;
				jmpCnt++;
			} else {
				jmpCnt++;
			}
		}

		waitForVblank();
		erase(pch);
		if (pch.dc != 0) {
		}
		pch.c += pch.dc;
		pch.r += pch.dr;
		pch.dc = 0;

		if(pch.c < 0)
			{
				pch.c = 0;
				pch.dc = 0;
			}
		if(pch.c >240-pch.w)
		{
			pch.c = 240-pch.w;
			pch.dc = 0;
		}

		if (pch.r > 83) {
			pch.dr = 0;
			pch.r = 83;
			jumping = 0;
		}

		if (pch.r < 78) {
			pch.dr = 1;
			pch.r = 78;
		}



	}
	return(0);
}