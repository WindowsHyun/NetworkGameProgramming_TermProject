#include "Texture_Load.h"

enum { Front, Back, Left, Right, Top, Bottom };

void draw_StoneMap1(GLuint object[]);
void draw_StoneMap2(GLuint object[]);
void draw_StoneMap3(GLuint object[]);
void draw_StoneMap4(GLuint object[]);
void draw_StoneMap5(GLuint object[]);
void draw_StoneMap6(GLuint object[]);
void draw_StoneMap7(GLuint object[]);
void draw_StoneMap8(GLuint object[]);
void draw_StoneMap9(GLuint object[]);
void draw_StoneMap10(GLuint object[]);
void draw_StoneMap11(GLuint object[]);
void draw_StoneMap12(GLuint object[]);

void draw_Location_Box(GLuint object[], int x, int y, int h){
	glPushMatrix();
	glTranslatef(120 * x, -120 + (120 * (h)), 120 * y);
	map_DATA[h][x + 36][y + 13] = 1;
	draw_Block(60, object);
	glPopMatrix();
}

void draw_Map_Location_Box(GLuint object[], int x, int y, int h, int location){
	glPushMatrix();
	glTranslatef(120 * x, -120 + (120 * (h)), 120 * y);
	map_DATA[h][x + 36][y + 13] = 1;
	draw_Map_Block(60, object, location);
	glPopMatrix();
}

void draw_Location_Ground(GLuint object[], int x, int y, int h){
	glPushMatrix();
	glTranslatef(120 * x, -120 + (120 * (h)), 120 * y);
	map_DATA[h][x + 36][y + 13] = 1;
	draw_Ground(60, object);
	glPopMatrix();
}

void draw_Ground(GLuint object[]){
	glPushMatrix();
	for (int i = -36; i < 36; ++i){
		for (int j = -13; j <= 13; ++j){
			draw_Location_Ground(object, i, j, 0);
		}
	}
	glPopMatrix();
}

void draw_Wall(GLuint object[]){
	glPushMatrix();
	for (int i = -27; i <= 27; ++i){
		if (i == -26) i = 27;
		for (int k = 1; k <= 4; ++k){
			for (int j = -13; j <= 13; ++j){
				if (j == -9 || j == -8 || j == -7 || j == 9 || j == 8 || j == 7){
				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}
	for (int i = -27; i <= 27; ++i){
		if (i == -26) i = 27;
		for (int k = 5; k <= 5; ++k){
			for (int j = -13; j <= 13; ++j){
				draw_Location_Box(object, i, j, k);
			}
		}
	}
	glPopMatrix();
}

void draw_Wooden(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -26; i <= -25; ++i){
		for (int j = -4; j <= -3; ++j){
			for (int k = 1; k <= 2; ++k){
				draw_Location_Box(object, i, j, k);
			}
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -15; i <= -13; ++i){
		for (int j = 5; j <= 7; ++j){
			draw_Location_Box(object, i, j, 1);
		}
	}
	draw_Location_Box(object, -15, 5, 2);
	draw_Location_Box(object, -14, 6, 2);
	draw_Location_Box(object, -14, 5, 2);
	draw_Location_Box(object, -13, 5, 2);
	draw_Location_Box(object, -13, 6, 2);
	draw_Location_Box(object, -13, 7, 2);
	draw_Location_Box(object, -13, 5, 3);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -7; i <= -6; ++i){
		for (int j = 8; j <= 9; ++j){
			draw_Location_Box(object, i, j, 1);
		}
	}
	draw_Location_Box(object, -6, 8, 2);
	draw_Location_Box(object, -7, 9, 2);
	draw_Location_Box(object, -6, 9, 2);
	draw_Location_Box(object, -6, 9, 3);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 22; i <= 23; ++i){
		for (int j = -10; j <= -9; ++j){
			draw_Location_Box(object, i, j, 1);
		}
	}
	draw_Location_Box(object, 22, -10, 2);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 9; i <= 10; ++i){
		for (int j = -13; j <= -12; ++j){
			for (int k = 1; k <= 2; ++k){
				if (i == 10 && j == -12 && k == 2){

				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}

	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 18; i <= 19; ++i){
		for (int j = 9; j <= 10; ++j){
			for (int k = 1; k <= 2; ++k){
				if (i == 19 && j == 9 && k == 2){

				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}

	draw_Location_Box(object, 18, 10, 3);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -5; i <= -3; ++i){
		for (int j = -7; j <= -5; ++j){
			for (int k = 1; k <= 2; ++k){
				if ((i == -4 && j == -5) && k == 2 || (i == -3 && j == -5) && k == 2){

				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}

	draw_Location_Box(object, -5, -7, 3);
	draw_Location_Box(object, -5, -5, 3);
	draw_Location_Box(object, -3, -7, 3);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 2; i <= 3; ++i){
		for (int j = 11; j <= 12; ++j){
			for (int k = 1; k <= 1; ++k){
				draw_Location_Box(object, i, j, k);
			}
		}
	}

	draw_Location_Box(object, 2, 11, 2);
	glPopMatrix();
	//-----------------------------------------------------------------------------------------





}

void draw_Stone(GLuint object[]){
	draw_StoneMap1(object);
	draw_StoneMap2(object);
	draw_StoneMap3(object);
	draw_StoneMap4(object);
	draw_StoneMap5(object);
	draw_StoneMap6(object);
	draw_StoneMap7(object);
	draw_StoneMap8(object);
	draw_StoneMap9(object);
	draw_StoneMap10(object);
	draw_StoneMap11(object);
	draw_StoneMap12(object);
}

void draw_StoneMap1(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 15; i <= 26; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 13, k, Front);

			draw_Map_Location_Box(object, i, 11, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 11; j <= 13; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 15, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 15; i <= 26; ++i){
		for (int j = 11; j <= 13; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 24; i <= 26; ++i){
		draw_Map_Location_Box(object, i, 10, 3, Top);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 24; i <= 26; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 10, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Map_Location_Box(object, 24, 10, k, Left);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap2(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 24; i <= 26; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -3, k, Back);

			draw_Map_Location_Box(object, i, 4, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -3; j <= 4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 24, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 24; i <= 26; ++i){
		for (int j = -3; j <= 4; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap3(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 19; i <= 26; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -13, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -13; j <= -4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 19, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 19; i <= 21; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -4, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -10; j <= -4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 21, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 22; i <= 26; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -11, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 19; i <= 21; ++i){
		for (int j = -10; j <= -4; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 19; i <= 26; ++i){
		for (int j = -13; j <= -11; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap4(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 11; i <= 13; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -2, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -2; j <= 0; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 11, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -2; j <= 0; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 11, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Map_Location_Box(object, 11, 0, k, Front);
		draw_Map_Location_Box(object, 12, 2, k, Front);
		draw_Map_Location_Box(object, 13, 4, k, Front);
		draw_Map_Location_Box(object, 14, 4, k, Front);
		draw_Map_Location_Box(object, 15, 4, k, Front);
		draw_Map_Location_Box(object, 15, 2, k, Back);
		draw_Map_Location_Box(object, 14, 0, k, Back);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 1; j <= 2; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 12, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 3; j <= 4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 13, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 2; j <= 4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 15, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 0; j <= 1; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 14, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -2; j <= -1; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 13, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 11; i <= 13; ++i){
		for (int j = -2; j <= 0; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 12; i <= 14; ++i){
		for (int j = 1; j <= 2; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 13; i <= 15; ++i){
		for (int j = 3; j <= 4; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	draw_Map_Location_Box(object, 14, 0, 3, Top);
	draw_Map_Location_Box(object, 15, 2, 3, Top);
}
void draw_StoneMap5(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 8; i <= 10; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -8, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -8; j <= -7; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 8, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -6; j <= -5; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 7, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Map_Location_Box(object, 7, -6, k, Back);
		draw_Map_Location_Box(object, 6, -4, k, Back);
		draw_Map_Location_Box(object, 9, -4, k, Front);
		draw_Map_Location_Box(object, 10, -6, k, Front);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -4; j <= -2; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 6, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();

	for (int i = 6; i <= 8; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -2, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -3; j <= -2; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 8, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -5; j <= -4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 9, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -8; j <= -6; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 10, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 6; i <= 8; ++i){
		for (int j = -4; j <= -2; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 7; i <= 9; ++i){
		for (int j = -6; j <= -5; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 8; i <= 10; ++i){
		for (int j = -8; j <= -7; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	draw_Map_Location_Box(object, 9, -4, 3, Top);
	draw_Map_Location_Box(object, 10, -6, 3, Top);
}
void draw_StoneMap6(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 1; i <= 3; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -12, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -12; j <= -10; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 1, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -9; j <= -8; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 2, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Map_Location_Box(object, 1, -10, k, Front);
		draw_Map_Location_Box(object, 2, -8, k, Front);
		draw_Map_Location_Box(object, 5, -8, k, Back);
		draw_Map_Location_Box(object, 4, -10, k, Back);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -7; j <= -6; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 3, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();

	for (int i = 3; i <= 5; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -6, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -8; j <= -6; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 5, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -10; j <= -9; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 4, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -12; j <= -11; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 3, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 1; i <= 3; ++i){
		for (int j = -12; j <= -10; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 2; i <= 4; ++i){
		for (int j = -9; j <= -8; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 3; i <= 5; ++i){
		for (int j = -7; j <= -6; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	draw_Map_Location_Box(object, 4, -10, 3, Top);
	draw_Map_Location_Box(object, 5, -8, 3, Top);
}
void draw_StoneMap7(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -14; i <= -6; ++i){
		for (int j = -13; j <= -10; ++j){
			for (int k = 1; k <= 4; ++k){
				if ((j == -12 || j == -11) && (k == 1 || k == 2 || k == 3)){
				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}
	for (int i = 1; i <= 3; ++i){
		draw_Location_Box(object, -14, -11, i);
	}

	for (int i = 1; i <= 3; ++i){
		draw_Location_Box(object, -6, -12, i);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap8(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -13; i <= -11; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -5, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -5; j <= -4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -13, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -3; j <= -2; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -14, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Map_Location_Box(object, -12, -1, k, Front);
		draw_Map_Location_Box(object, -11, -3, k, Front);
		draw_Map_Location_Box(object, -14, -3, k, Back);
		draw_Map_Location_Box(object, -15, -1, k, Back);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -1; j <= 1; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -15, j, k, Left);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -15; i <= -13; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 1, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 0; j <= 1; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -13, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -2; j <= -1; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -12, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -5; j <= -3; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -11, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -15; i <= -13; ++i){
		for (int j = -1; j <= 1; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -14; i <= -12; ++i){
		for (int j = -3; j <= -2; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -13; i <= -11; ++i){
		for (int j = -5; j <= -4; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	draw_Map_Location_Box(object, -11, -3, 3, Top);
	draw_Map_Location_Box(object, -12, -1, 3, Top);
}
void draw_StoneMap9(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -16; i <= -9; ++i){
		for (int j = 10; j <= 13; ++j){
			for (int k = 1; k <= 4; ++k){
				if ((j == 12 || j == 11) && (k == 1 || k == 2 || k == 3)){
				}
				else{
					draw_Location_Box(object, i, j, k);
				}
			}
		}
	}
	for (int i = 1; i <= 3; ++i){
		draw_Location_Box(object, -16, 11, i);
	}

	for (int i = 1; i <= 3; ++i){
		draw_Location_Box(object, -9, 12, i);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap10(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -23; i <= -21; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 5, k, Back);

			draw_Map_Location_Box(object, i, 12, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 5; j <= 12; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -23, j, k, Left);
			draw_Map_Location_Box(object, -21, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -23; i <= -21; ++i){
		for (int j = 5; j <= 12; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
}
void draw_StoneMap11(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -23; i <= -21; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -12, k, Back);

			draw_Map_Location_Box(object, i, -6, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -12; j <= -6; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -23, j, k, Left);
			draw_Map_Location_Box(object, -21, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -23; i <= -21; ++i){
		for (int j = -12; j <= -6; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -24; i <= -22; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, -4, k, Front);
			draw_Map_Location_Box(object, i, -5, k, Back);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = -5; j <= -4; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -24, j, k, Left);
			draw_Map_Location_Box(object, -22, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -24; i <= -22; ++i){
		for (int j = -5; j <= -4; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------

}
void draw_StoneMap12(GLuint object[]){
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -1; i <= 11; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 7, k, Back);
			draw_Map_Location_Box(object, i, 10, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 7; j <= 10; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -1, j, k, Left);
			draw_Map_Location_Box(object, 11, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -1; i <= 11; ++i){
		for (int j = 7; j <= 10; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -1; i <= 1; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 11, k, Back);
			draw_Map_Location_Box(object, i, 13, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 11; j <= 13; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, -1, j, k, Left);
			draw_Map_Location_Box(object, 1, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -1; i <= 1; ++i){
		for (int j = 11; j <= 13; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 2; i <= 4; ++i){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, i, 0, k, Back);
			draw_Map_Location_Box(object, i, 7, k, Front);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 0; j <= 7; ++j){
		for (int k = 1; k <= 3; ++k){
			draw_Map_Location_Box(object, 2, j, k, Left);
			draw_Map_Location_Box(object, 4, j, k, Right);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = 2; i <= 4; ++i){
		for (int j = 0; j <= 7; ++j){
			draw_Map_Location_Box(object, i, j, 3, Top);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------


















	glPushMatrix();
	for (int k = 1; k <= 3; ++k){
		draw_Location_Box(object, 1, 7, k);
		draw_Location_Box(object, 0, 6, k);
		draw_Location_Box(object, 0, 5, k);
		draw_Location_Box(object, -2, 6, k);
		draw_Location_Box(object, -2, 7, k);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int k = 1; k <= 2; ++k){
		draw_Location_Box(object, 0, 7, k);
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int j = 3; j <= 7; ++j){
		for (int k = 1; k <= 2; ++k){
			draw_Location_Box(object, -1, j, k);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -3; i <= -2; ++i){
		for (int k = 1; k <= 2; ++k){
			draw_Location_Box(object, i, 5, k);
		}
	}
	glPopMatrix();
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	for (int i = -3; i <= -2; ++i){
		for (int j = 3; j <= 4; ++j){
			draw_Location_Box(object, i, j, 1);
		}
	}
	glPopMatrix();
}