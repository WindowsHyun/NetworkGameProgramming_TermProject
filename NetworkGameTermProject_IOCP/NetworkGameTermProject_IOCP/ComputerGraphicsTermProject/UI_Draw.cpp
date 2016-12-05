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
		//����
		glVertex2f( (w / 2) - 20, h / 2 );
		glVertex2f( (w / 2) - 10, h / 2 );

		//������
		glVertex2f( (w / 2) + 20, h / 2 );
		glVertex2f( (w / 2) + 10, h / 2 );

		//����
		glVertex2f( w / 2, (h / 2) - 20 );
		glVertex2f( w / 2, (h / 2) - 10 );

		//�Ʒ���
		glVertex2f( w / 2, (h / 2) + 20 );
		glVertex2f( w / 2, (h / 2) + 10 );
		glEnd();
	}
}

void draw_AccessUser() {
	char buf[MAX_PATH];
	for ( int i = 0; i < MAX_Client; ++i ) {
		if ( (server_data.Players[i].live == true || server_data.Players[i].hp == 0) && server_data.Players[i].Game_Play == true ) {
			glPushMatrix(); // ���� ����Ʈ ǥ��
			if ( i % 2 == 0 )
				glColor3f( 1, 1, 1 );
			else
				glColor3f( 1, 0, 0 );

			glTranslatef( 5, 15 + (i * 15), 0 );
			glRasterPos2f( 0.0, 0.0 );
			if ( server_data.Players[i].hp == 0 ) {
				sprintf( buf, "Player %d : %s -- Die --", i + 1, server_data.Players[i].nickName );
			}
			else {
				sprintf( buf, "Player %d : %s", i + 1, server_data.Players[i].nickName );
			}
			int len = (int)strlen( buf );
			for ( int j = 0; j < len; j++ )
				glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[j] );
			glPopMatrix(); // ���� ����Ʈ ǥ��
		}
	}
}

void draw_FPS( int w, int h ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
	glColor3f( 1, 0, 0 );
	glTranslatef( w - 137, 15, 0 );
	glRasterPos2f( 0.0, 0.0 );
	sprintf( buf, "FPS : %f", fps );
	int len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();
}

void draw_HP( int w, int h, Player_Socket player ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
	glColor3f( 1, 0, 0 );
	glTranslatef( 10, h - 25, 0 );
	glRasterPos2f( 0.0, 0.0 );
	sprintf( buf, "HP : %d", player.hp );
	int len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, buf[i] );
	glPopMatrix();
}

void draw_Respawn( int w, int h, Player_Socket  player ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
	glColor3f( 1, 1, 1 );

	sprintf( buf, "Respawn %d Time Wait..!", player.RespawnTime );
	//sprintf( buf, "Respawn %d Time Wait..!", 9 );
	int len = (int)strlen( buf );

	glTranslatef( (w / 2) - 103, (h / 2) + 10, 0 );
	glRasterPos2f( 0.0, 0.0 );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, buf[i] );
	glPopMatrix();
}

void draw_Packet( int w, int h ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
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

void draw_Roundtime( int w, int h, int sec )
{
	char buf[MAX_PATH];
	glPushMatrix();//Round ǥ��
	glColor3f( 0, 0, 1 );//�Ķ����۾�
	glTranslatef( w / 2 - 17, 15, 0 );//�۾���ġ
	glRasterPos2f( 0.0, 0.0 );
	sprintf( buf, "Round : %d", server_data.gr.GameRound );
	int len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();


	glPushMatrix();//�����ð� ǥ��
	glColor3f( 0, 0, 1 );//�Ķ����۾�
	glTranslatef( w / 2, 30, 0 );//�۾���ġ
	glRasterPos2f( 0.0, 0.0 );
	if ( sec % 60 < 10 ) {
		sprintf( buf, "%d : 0%d", sec / 60, sec % 60 );
	}
	else {
		sprintf( buf, "%d : %d", sec / 60, sec % 60 );
	}
	len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();
}

void update_FPS() {
	static DWORD frameCount = 0;            //������ ī��Ʈ��
	static float timeElapsed = 0.0f;            //�帥 �ð�
	static DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

	DWORD curTime = timeGetTime();      //���� �ð�
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1�������� �帥 �ð�) 1�ʴ����� �ٲ��ش�.

	timeElapsed += timeDelta;
	frameCount++;

	if ( timeElapsed >= 1.0f )         //�帥�ð��� 1���̻��̸� ���� �ϰ������ ó��
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

void draw_GameWait( int w, int h ) {
	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
	glColor3f( 1, 1, 1 );
	if ( server_data.gr.round_wait == false ) {
		sprintf( buf, "Waiting for Game Personnel ...!" );
		glTranslatef( (w / 2) - 130, (h / 2) + 10, 0 );
	}
	else {
		sprintf( buf, "%d Seconds Left To Start The Game.", server_data.gr.time_wait );
		glTranslatef( (w / 2) - 145, (h / 2) + 10, 0 );
	}
	//sprintf( buf, "Respawn %d Time Wait..!", 9 );
	int len = (int)strlen( buf );

	glRasterPos2f( 0.0, 0.0 );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, buf[i] );
	glPopMatrix();
}

void draw_WinLose( int w, int h, Player_Socket player ) {

	char buf[MAX_PATH];
	glPushMatrix();//FPS ǥ��
	glColor3f( 1, 1, 0 );
	if ( server_data.gr.game_result == player.team ) {
		sprintf( buf, "You Won in the Game." );
		glTranslatef( (w / 2) - 92, (h / 2) - 30, 0 );
	}
	else {
		sprintf( buf, "You Defeated in the Game." );
		glTranslatef( (w / 2) - 110, (h / 2) - 30, 0 );
	}
	int len = (int)strlen( buf );

	glRasterPos2f( 0.0, 0.0 );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, buf[i] );
	glPopMatrix();
}

void drawHud( int w, int h, Player_Socket player, int time )
{
	glPushMatrix();

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, w, h, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable( GL_LIGHTING );      // ���� Ȱ��ȭ
	glDisable( GL_COLOR_MATERIAL );
	glDisable( GL_NORMALIZE );

	if ( player.live == false ) {
		draw_Respawn( w, h, player );
	}
	else {
		if ( server_data.gr.round_start == false ) {
			draw_GameWait( w, h );
		}
		else {
			draw_Crosshair( false, w, h ); // ũ�ν� ��� ����
		}
	}

	draw_AccessUser(); // ������ ����Ʈ ���ϱ�
	draw_FPS( w, h ); // FPS �׷��ֱ�
	draw_Packet( w, h );
	draw_HP( w, h, player );
	draw_Roundtime( w, h, time );
	if ( server_data.gr.round_wait == true && server_data.gr.GameRound != 1 ) {
		draw_WinLose( w, h, player );
	}
	glPopMatrix();
}