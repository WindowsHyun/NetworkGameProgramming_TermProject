#include "Texture_Load.h"


void draw_Block(int size, GLuint object[]){
	drawBoxFront(size, false, object[0]);
	drawBoxBack(size, false, object[1]);
	drawBoxLeft(size, false, object[2]);
	drawBoxRight(size, false, object[3]);
	drawBoxTop(size, false, object[4]);
	drawBoxBottom(size, false, object[5]);
}

void draw_Ground(int size, GLuint object[]){
	drawBoxTop(size, false, object[4]);
}

void draw_Map_Block(int size, GLuint object[], int location){
	if (location == 0){
		drawBoxFront(size, false, object[0]);
	}
	else if (location == 1){
		drawBoxBack(size, false, object[1]);
	}
	else if (location == 2){
		drawBoxLeft(size, false, object[2]);
	}
	else if (location == 3){
		drawBoxRight(size, false, object[3]);
	}
	else if (location == 4){
		drawBoxTop(size, false, object[4]);
	}
	else if (location == 5){
		drawBoxBottom(size, false, object[5]);
	}
}
