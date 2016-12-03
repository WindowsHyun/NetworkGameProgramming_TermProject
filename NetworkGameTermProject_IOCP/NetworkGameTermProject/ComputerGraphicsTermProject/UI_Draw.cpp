#include "main.h"

float fps;
int sendPacket;
int maxPacket = 0;

void draw_Crosshair( bool sight, int w, int h ) {
	if ( sight ) {
		glColor3f( 0, 0, 1 );
		glBegin( GL_QUADS );// Crosshair
		glVertex2f( 396, 304 );
		glVertex2f( 404, 304 );
		glVertex2f( 404, 296 );
		glVertex2f( 396, 296 );
		glEnd();
	}
	else {
		glColor3f( 0, 1, 0 );
		glBegin( GL_LINES );// Crosshair
		//왼쪽
		glVertex2f( (w / 2) - 20, h / 2 );
		glVertex2f( (w / 2) - 10, h / 2 );

		//오른쪽
		glVertex2f( (w / 2) + 20, h / 2 );
		glVertex2f( (w / 2) + 10, h / 2 );

		//위쪽
		glVertex2f( w / 2, (h / 2) - 20 );
		glVertex2f( w / 2, (h / 2) - 10 );

		//아래쪽
		glVertex2f( w / 2, (h / 2) + 20 );
		glVertex2f( w / 2, (h / 2) + 10 );
		glEnd();
	}
}

void draw_AccessUser() {
	char buf[MAX_PATH];
	for ( int i = 0; i < MAX_Client; ++i ) {
		if ( server_data.Players[i].live == true ) {
			glPushMatrix(); // 접속 리스트 표시
			if ( i % 2 == 0 )
				glColor3f( 1, 1, 1 );
			else
				glColor3f( 1, 0, 0 );

			glTranslatef( 5, 15 + (i * 15), 0 );
			glRasterPos2f( 0.0, 0.0 );
			sprintf( buf, "Player %d : %s", i + 1, server_data.Players[i].nickName );
			int len = (int)strlen( buf );
			for ( int j = 0; j < len; j++ )
				glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[j] );
			glPopMatrix(); // 접속 리스트 표시
		}
	}
}

void draw_FPS(int w, int h) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS 표시
	glColor3f( 1, 0, 0 );
	glTranslatef( w-137, 15, 0 );
	glRasterPos2f( 0.0, 0.0 );
	sprintf( buf, "FPS : %f", fps );
	int len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();
}

void draw_Packet( int w, int h ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS 표시
	glColor3f( 1, 0, 0 );
	glTranslatef( w - 137, 30, 0 );
	glRasterPos2f( 0.0, 0.0 );
	if ( maxPacket <= sendPacket )
		maxPacket = sendPacket;

	sprintf( buf, "P : %d [%d]", sendPacket, maxPacket );
	int len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();
}

void drawHud( int w, int h )
{
	glPushMatrix();

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, w, h, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable( GL_LIGHTING );      // 조명 활성화
	glDisable( GL_COLOR_MATERIAL );
	glDisable( GL_NORMALIZE );

	draw_Crosshair( false, w, h ); // 크로스 헤어 구현
	draw_AccessUser(); // 접속자 리스트 구하기
	draw_FPS(w, h); // FPS 그려주기
	draw_Packet( w,  h );


	//char ammo[100];
	//glPushMatrix();//총알 표시
	//glColor3f( 0, 1, 0 );
	//glTranslatef( 680, 570, 0 );
	//glRasterPos2f( 0.0, 0.0 );
	////sprintf(ammo, "%d / %d", rifleload, rifleammo);
	//sprintf( ammo, "30/30" );
	//int len = (int)strlen( ammo );
	//for ( int i = 0; i < len; i++ )
	//	glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, ammo[i] );
	//glPopMatrix();//총알 표시

	//glPushMatrix();//체력 표시
	//glColor3f(1, 0, 0);
	//glTranslatef(20, 570, 0);
	//glRasterPos2f(0.0, 0.0);
	//sprintf(health, "%d", hp);
	//len = (int)strlen(ammo);
	//for (int i = 0; i < len; i++)
	//	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, health[i]);
	//glPopMatrix();//체력 표시 pop

	glPopMatrix();
}

void update_FPS() {
	static DWORD frameCount = 0;            //프레임 카운트수
	static float timeElapsed = 0.0f;            //흐른 시간
	static DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

	DWORD curTime = timeGetTime();      //현재 시간
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.

	timeElapsed += timeDelta;
	frameCount++;

	if ( timeElapsed >= 1.0f )         //흐른시간이 1초이상이면 내가 하고싶은것 처리
	{
		fps = (float)frameCount / timeElapsed;
		frameCount = 0;
		timeElapsed = 0.0f;
	}
	else
	{
		//Sleep(0.002f * 1000);   // 100fps   
	}


	lastTime = curTime;
}