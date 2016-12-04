#include "main.h"

void Draw_Box_Top( Box* box, int i );
void Draw_Box_Bottom( Box* box, int i );
void Draw_Box_Right( Box* box, int i );
void Draw_Box_Left( Box* box, int i );
void Draw_Box_Front( Box* box, int i );
void Draw_Box_Back( Box* box, int i );

void init_Map_Texture()
{
	Load_TextureBMP( map_object, 0, "ImageData/Wall/wall.bmp" );
	Load_TextureBMP( stone_object, 0, "ImageData/Block/Stone/Around.bmp" );
	Load_TextureBMP( stone_object, 1, "ImageData/Block/Wooden/Around.bmp" );
	Load_TextureBMP( stone_object, 2, "ImageData/Block/Brick/Around.bmp" );
	Load_TextureBMP( stone_object, 3, "ImageData/Block/Nomal/Top.bmp" );
}

void DrawMap() {
	glPushMatrix(); //바닥그리기
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, map_object[0] );
	glBegin( GL_QUADS );//사각평면그리기
						//인호's 샘플코드
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( -4000, 0, 4000 );  //1
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( 4000, 0, 4000 );   //4
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( 4000, 0, -4000 );   //6
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( -4000, 0, -4000 );   //5
	glEnd();//사각평면그리기 종료
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();//바닥그리기 종료

	glPushMatrix(); //벽면 그리기
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, map_object[0] );
	glBegin( GL_QUADS );//사각평면그리기
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( -4000, 0, +4000 );//벽면
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -4000, +3000, +4000 );
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( +4000, +3000, +4000 );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +4000, 0, +4000 );
	glEnd();//사각평면그리기 종료
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();//벽면 종료


	glPushMatrix(); //벽면 그리기
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, map_object[0] );
	glBegin( GL_QUADS );//사각평면그리기
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( -4000, 3000, -4000 );//벽면
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -4000, 0, -4000 );
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( +4000, 0, -4000 );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +4000, 3000, -4000 );
	glEnd();//사각평면그리기 종료
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();//벽면 종료


	glPushMatrix(); //벽면 그리기
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, map_object[0] );
	glBegin( GL_QUADS );//사각평면그리기
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( -4000, 0, -4000 );//False 팀쪽 벽면
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -4000, 3000, -4000 );
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -4000, 3000, +4000 );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( -4000, 0, +4000 );
	glEnd();//사각평면그리기 종료
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();//벽면 종료


	glPushMatrix(); //벽면 그리기
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, map_object[0] );
	glBegin( GL_QUADS );//사각평면그리기
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +4000, 3000, -4000 );//True 팀쪽 벽면
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( +4000, 0, -4000 );
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( +4000, 0, +4000 );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +4000, 3000, +4000 );
	glEnd();//사각평면그리기 종료
	glDisable( GL_TEXTURE_2D );
	glPopMatrix();//벽면 종료
}

void DrawBox( Box* box ) {
	//맵그리기
	for ( int i = 0; i < BOX_COUNT; i++ ) {
		glPushMatrix();
		glEnable( GL_TEXTURE_2D );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glTranslatef( box[i].x, box[i].y + (box[i].size / 2), box[i].z );
		glBindTexture( GL_TEXTURE_2D, stone_object[box[i].image] );
		glBegin( GL_QUADS );
		Draw_Box_Top( box, i );
		Draw_Box_Bottom( box, i );
		Draw_Box_Right( box, i );
		Draw_Box_Left( box, i );
		Draw_Box_Front( box, i );
		Draw_Box_Back( box, i );
		glEnd();
		glDisable( GL_TEXTURE_2D );
		glPopMatrix();
	}
}

void Mapping_Box(Box &box, int x, int y, int z, int img_num) {
	int temp_x = -3850, temp_z = -3850;

	for ( int i = 0; i < x; ++i )
		temp_x += 300;

	for ( int i = 0; i < z; ++i )
		temp_z += 300;

	box.x = temp_x;
	box.y = 0;
	box.z = temp_z;
	box.size = 300;
	box.image = img_num;
}

void Draw_Box_Top( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) );//윗면
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) );
}

void Draw_Box_Bottom( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) );//아랫면
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) );
}

void Draw_Box_Right( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) ); // 오른쪽
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) );
}

void Draw_Box_Left( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) ); // 왼쪽
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) );
}

void Draw_Box_Front( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) ); //앞면
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( -(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( -(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) );
}

void Draw_Box_Back( Box* box, int i ) {
	glTexCoord2d( 0.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), +(box[i].size / 2) ); // 뒷면
	glTexCoord2d( 0.0f, 0.0f );
	glVertex3f( +(box[i].size / 2), -(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 0.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), -(box[i].size / 2) );
	glTexCoord2d( 1.0f, 1.0f );
	glVertex3f( +(box[i].size / 2), +(box[i].size / 2), +(box[i].size / 2) );
}