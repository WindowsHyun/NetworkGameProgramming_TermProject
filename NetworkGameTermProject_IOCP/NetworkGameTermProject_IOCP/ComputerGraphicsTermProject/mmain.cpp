#include "main.h"

GLvoid DrawScene( GLvoid );
GLvoid Reshape( int w, int h );

// ���̾�α� ����
HINSTANCE hInstance;

//��������
SOCKET sock;

//ĳ���� ����ü
Player player_client;
Player_Socket player_socket;
Server_Player server_data;
ClientData client_data; // ����, ������ȣ

Player_Animation Ani[MAX_Client];//�ִϸ��̼� ����ü
Box player_collision[MAX_Client][4];//�浹üũ �ڽ�

//�ʼ� �Լ���
void Mouse( int button, int state, int x, int y );
void Keyboard( unsigned char key, int x, int y );
void Keyboardup( unsigned char key, int x, int y );
void Motion( int x, int y );
void TimerFunction( int value );
void reset_character(); // ������� ��ġ�� �ʱ�ȭ ���ش�.
void SetBoxState();

void Target( int x, int y );//ī�޶� �������� �Լ�
void Keyinput( int key );//Ű���� �����Է��� ���� �Է�ó�� �Լ�
//void crashCheck();//�浹üũ

//Ÿ�̸� ���Žð�
int Time = 10, Bullet_Time = 1;
float fTime, fCTime, fPTime;
bool err;
int round_time = 60;//���� Ÿ�̸�
bool tab = false;//���� ǥ��â

//�ػ�
int width, height;
char Input;

//UI���� ������
int rifleammo = 120, rifleload = 30, hp = 100;
char ammo[10], health[10], PressSpace[50];
int len;

//ĳ���� �� ī�޶� ���� ����
float nx = 0, ny = 0, bx = 0, by = 0;//���콺 �����ӿ� ���̴� �ӽú���
float gravity = 0, Charspeed = 0.7f;
int  Camdistance = 400, MouseSens = 25;
bool RotateCam = false, FirstPersonView = true;
bool Keybuffer[256];//�����Է¿� ���Ǵ� ����

//�������� ���� �Լ�
bool GameStart = false;

//��ֹ�
Box box[BOX_COUNT];
//vector3 Cross3D;

// IOCP ���� ���۽� Ÿ�̸� �����̸� �ش�.
LARGE_INTEGER   tickPerSecond;
LARGE_INTEGER   startTick, endTick;
double dTime;

//�ִϸ��̼� ����
//float left_sholder_x, left_sholder_y, left_sholder_z, right_sholder_x, right_sholder_y, right_sholder_z, left_elbow_x, right_elbow_x;//��ȸ��
bool gunfire = false, dummyfire = false;
float flashtimer = 0;

// ���� ����
/*
bool turn_Light = true;
void show_Light(){
GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0f };
GLfloat DiffuseLight[] = { 0.8, 0.8, 0.8, 1.0f };
GLfloat lightpos[] = { Charx, 200, Charz, 1 };
GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
if (sight)
{
glLightf(GL_LIGHT0, GL_SPOT_DIRECTION, (0, 0, 1));
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, (30.0));
}
else{
glLightf(GL_LIGHT0, GL_SPOT_DIRECTION, (0, 0, 1));
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, (70.0));
}
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight);
glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
}
*/



DWORD WINAPI SR_Thread( LPVOID arg ) {
	QueryPerformanceFrequency( &tickPerSecond ); // �ý����� �ִ� ��� ���������� timer resolution �����ϴ��� �Ǻ�
	QueryPerformanceCounter( &startTick );
	while ( 1 ) {
		QueryPerformanceCounter( &endTick );
		dTime = (double)(endTick.QuadPart - startTick.QuadPart) / tickPerSecond.QuadPart;
		//printf( "%lf\t", dTime );
		while ( dTime >= 0.002000 ) {
			QueryPerformanceCounter( &startTick );
			dTime = 0;
			iocp_Communication( sock, player_socket, client_data, server_data );
			sendPacket++;
		}
	}
	//printf( "Client ID %d : %f\n", client_imei, server_data.player[client_imei].camxrotate );
	return 0;
}

DWORD WINAPI DialogBox_Thread( LPVOID arg ) {
	// ��ȭ���� ����
	//RotateCam = false;
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	//�������ӽõ�
	sock = init_sock();
	strcpy( player_socket.nickName, nickName );
	HANDLE hThread;
	hThread = CreateThread( NULL, 0, SR_Thread, 0, 0, NULL );
	//�÷��̾��� �����͸� ���� �� init_sock���� ��� �����͸� �Ѱ��ָ� ���� ������?
	//Win_Sock.cpp���� ���Ḯ��Ʈ�� ���� �߰��� �����ϸ� �ɵ�
	//��â���� �ڵ� ������ �׽�Ʈ�غ���
	return 0;
}

void main()
{
	srand( (unsigned)time( NULL ) );

	HANDLE hThread;
	hThread = CreateThread( NULL, 0, DialogBox_Thread, 0, 0, NULL );

	//������ �ʱ�ȭ �� ����
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); //���÷��� ��� ����
	//glutInitWindowPosition(500, 0); // ������ ��ġ ����
	glutInitWindowPosition( (glutGet( GLUT_SCREEN_WIDTH ) - Window_Width) / 2, (glutGet( GLUT_SCREEN_HEIGHT ) - Window_Height) / 2 ); // ȭ�� �߾����� �����ϱ�
	glutInitWindowSize( Window_Width, Window_Height ); //������ ũ�� ����
	glutCreateWindow( "NetworkGame TermProject" ); //������ ����(������ �̸�)
	init_Character_Texture();
	init_Zombie_Texture();
	init_Map_Texture();

	//���� ���� �ʱ�ȭ �Լ�
	for ( int i = 0; i < 256; i++ )
		Keybuffer[i] = false;

	SetBoxState();

	//�ʿ��� �ݹ� �Լ� ����

	glutDisplayFunc( DrawScene ); //��� �Լ��� ����
	glutTimerFunc( Time, TimerFunction, 1 );//Ÿ�̸� �ݹ� �Լ�
	glutTimerFunc( 1024, TimerFunction, 2 ); // ��Ŷ ������ Ȯ���ϱ� ���Ͽ�
	glutTimerFunc( 1000, TimerFunction, 3 );


	glutPassiveMotionFunc( Motion );//���콺���
	glutMouseFunc( Mouse );//���콺�Է�
	glutMotionFunc( Motion );//Ŭ���ÿ��� �����̰Բ� ���콺��� ����

	glutKeyboardFunc( Keyboard );//Ű�����Է�
	glutKeyboardUpFunc( Keyboardup );//Ű���� ��ư�� ���� ��

	glutReshapeFunc( Reshape );//�ٽñ׸���
	glutMainLoop();//�̺�Ʈ ���� �����ϱ�

}

//������ ��� �Լ�
GLvoid DrawScene( GLvoid )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );//��������'black'������
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// ������ ������ ��ü�� ĥ�ϱ�

	glViewport( 0, 0, width, height );//����Ʈ ����
	glMatrixMode( GL_PROJECTION );//�Ÿ����� �繰�� �������� �׸��� ���
	glLoadIdentity();//�ʱ�ȭ? ã�ƺ��� ��������
	gluPerspective( 60.0f, (float)width / (float)height, 0.1, 10000.0 );//�þ߰� + �׷��� �Ÿ�
	glMatrixMode( GL_MODELVIEW );//???������

	glLoadIdentity();//�ʱ�ȭ? ã�ƺ��� ��������

	update_FPS();

	if ( GameStart )
	{
		//1��Ī�϶�
		if ( FirstPersonView ) { gluLookAt( player_socket.x, player_socket.y + 240, player_socket.z, player_socket.x + player_client.Viewx, player_socket.y + player_client.Viewy + 170, player_socket.z + player_client.Viewz, 0.0, 1.0, 0.0 ); }
		//3��Ī�϶�
		else { gluLookAt( player_socket.x + player_client.Camx, player_socket.y + player_client.Camy + 190, player_socket.z + player_client.Camz, player_socket.x + player_client.Viewx, player_socket.y + player_client.Viewy + 190, player_socket.z + player_client.Viewz, 0.0, 1.0, 0.0 ); }

		//������
		glEnable( GL_DEPTH_TEST );			// ������ �� ����
		glEnable( GL_CULL_FACE );				// �ĸ� ����

		DrawMap();
		DrawBox( box );

		//�÷��̾� �׸���
		if ( player_socket.live )
			drawCharacter( &player_socket, &player_client.Ani, gunfire );

		//�ٸ� �÷��̾� �׸���
		for ( int i = 0; i < MAX_Client; i++ )
		{
			if ( (i == client_data.client_imei) || (server_data.Players[i].live == false) ) {}
			else
			{
				if ( client_data.team == server_data.Players[i].team )
					drawCharacter( &server_data.Players[i], &Ani[i], dummyfire );
				else
					drawZombie( &server_data.Players[i], &Ani[i] );

				//�浹üũ�ڽ�
				//glPushMatrix();
				//glColor3f( 0, 255, 0 );
				//glTranslatef( server_data.Players[i].x, server_data.Players[i].y + (player_collision[i][3].size / 2), server_data.Players[i].z );
				//glutWireCube( (double)player_collision[i][3].size );
				//glTranslatef( 0, player_collision[i][2].size, 0 );
				//glutWireCube( (double)player_collision[i][2].size );
				//glTranslatef( 0, player_collision[i][1].size, 0 );
				//glutWireCube( (double)player_collision[i][1].size );
				//glTranslatef( 0, player_collision[i][0].size, 0 );
				//glutWireCube( (double)player_collision[i][0].size );
				//glPopMatrix();
				//printf("%d, size : %d\n",player_collision[i][2].size);
			}
		}

		drawHud( width, height, player_socket, server_data.gr.round_time );//HUD �׸��� �κ� ���ο� push pop �˾Ƽ� ��
	}
	else
	{
		//������ ���� ����
		gluLookAt( 2500, 1800, 2500, 0, 0, 0, 0.0, 1.0, 0.0 );

		//������
		glEnable( GL_DEPTH_TEST );			// ������ �� ����
		glEnable( GL_CULL_FACE );				// �ĸ� ����

		DrawMap();
		DrawBox( box );

		//�ٸ� �÷��̾� �׸���
		for ( int i = 0; i < MAX_Client; i++ )
		{
			if ( (i == client_data.client_imei) || (server_data.Players[i].live == false) ) {}
			else
			{
				if ( client_data.team == server_data.Players[i].team )
					drawCharacter( &server_data.Players[i], &Ani[i], dummyfire );
				else
					drawZombie( &server_data.Players[i], &Ani[i] );
			}
		}

		glPushMatrix();//����ǥ�� ����
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, width, height, 0, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glColor3f( 1, 0, 0 );
		glTranslatef( 20, height - 30, 0 );
		glRasterPos2f( 0.0, 0.0 );
		if ( GamePlayWait == false )
			sprintf( PressSpace, "Please Press \"Spacebar\"" );
		else
			sprintf( PressSpace, "Please Connect \"Server\"" );
		len = (int)strlen( PressSpace );
		for ( int i = 0; i < len; i++ )
			glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, PressSpace[i] );
		glPopMatrix();//����ǥ�� ��

	}

	if ( Keybuffer[VK_TAB] )
	{
		draw_score( width, height, server_data );
		//printf("tab in DrawScene\n");
	}

	glutSwapBuffers(); //ȭ�鿡 ����ϱ�
}//end of drawScene

GLvoid Reshape( int w, int h )
{
	//����Ʈ ��ȯ ����
	glViewport( 0, 0, w, h );
	width = w;
	height = h;

	//���� ��� ���� �缳��
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//Ŭ���� ���� ���� : ��������
	gluPerspective( 60.0f, (float)w / (float)h, 0.1, 5000.0 );


	//�� �� ��� ���� �缳��
	glMatrixMode( GL_MODELVIEW );
}

void Mouse( int button, int state, int x, int y )//���콺 Ŭ������ üũ
{
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )//��Ŭ��
	{
		//Bullet(&box[0]);
		BulletCollision( box );
		gunfire = true;
	}
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )//��Ŭ��
	{
		player_client.Ani.sight = false;
		//if ( FirstPersonView )
		//{//1��Ī�ϰ�� ������
		//	if ( player_client.Ani.sight ) {
		//		player_client.Ani.sight = false;
		//		//turn_Light = true;
		//	}
		//	else {
		//		player_client.Ani.sight = true;
		//		//turn_Light = true;
		//	}
		//}
		//else {}
	}
}//end of Mouse

void Motion( int x, int y )//���콺 �Է�(��ǥ��)�� ���ڸ��� Target���� �Ѱ��ְ� Target���� Viewportó��
{
	Target( x, y );
}

void Keyboard( unsigned char key, int x, int y )
{
	Keybuffer[key] = true;

	switch ( key )//
	{
	case ' '://�����̽���
		if ( GameStart )
		{
			if ( player_client.Ani.jump ) {}
			else
			{
				player_client.Ani.jump = true;
				gravity = 15;
				Keybuffer[key] = false;
			}
		}
		else
		{
			//if ( GamePlayWait == true ) break; // ���̾�α� �ڽ��� ���������� ���� �÷��� �����ϰ� �Ѵ�.

			// ���� ������ �ҽ� �������ش�.
			glutSetCursor( GLUT_CURSOR_NONE );//���콺Ŀ�� ���ֱ�
			RotateCam = true;

			GameStart = true;
			player_socket.Game_Play = true;
			player_socket.live = true;
			strcpy( player_socket.nickName, nickName );
			reset_character();
		}
		break;
	case 'c':
		glutSetCursor( GLUT_CURSOR_NONE );
		if ( RotateCam )
		{
			RotateCam = false;
			glutSetCursor( GLUT_CURSOR_CROSSHAIR );//���콺Ŀ�� ���ֱ�
		}
		else
		{
			RotateCam = true;
			glutSetCursor( GLUT_CURSOR_NONE );//���콺Ŀ�� ���ֱ�
		}
		Keybuffer[key] = false;
		break;
	case 'f':
		if ( FirstPersonView ) {
			FirstPersonView = false;
			player_client.Ani.sight = false;
			//turn_Light = true;
		}
		else {
			FirstPersonView = true;
			//turn_Light = true;
		}
		Keybuffer[key] = false;
		break;
	case 't':
		printf( "x : %f, y: %f\n", player_socket.camxrotate, player_socket.camyrotate );
		//server_data.Players[1].live = true;
		/*printf( "\n%d\n", player_socket.live );
		for ( int i = 0; i < MAX_Client; i++ )
			printf( "%d", server_data.Players[i].live );
		printf( "\n" );
		for (int i = 0; i < MAX_Client; i++)
			printf("%d", server_data.Players[i].character_down_state);*/
			//for ( int i = 0; i < MAX_Client; i++ )
			//	printf( "%d", server_data.Players[i].team );
		break;
	case 27://ESC
		if ( GameStart )
		{
			GameStart = false;
			Keybuffer[key] = false;
			player_socket.x = 0;
			player_socket.y = -1000;
			player_socket.z = 0;
			player_client.Camx = 0;
			player_client.Camy = 0;
			player_client.Camz = 0;
			player_client.Viewx = 0;
			player_client.Viewy = 0;
			player_client.Viewz = 0;
			player_socket.live = false;
			player_socket.Game_Play = false;
		}
		else exit( 0 );
		break;
	case VK_TAB://TABŰ
		//printf("tab\n");
		break;
	}
}//end of Keyboard

void Keyboardup( unsigned char key, int x, int y )
{
	Keybuffer[key] = false;
	if ( !Keybuffer['w'] && !Keybuffer['a'] && !Keybuffer['s'] && !Keybuffer['d'] )
		player_socket.character_down_state = CHARACTER_STAND;
	if ( Keybuffer['x'] == false )
	{
		//Charspeed = 8;
		player_client.Ani.crouch = false;
	}
}

void TimerFunction( int value )
{
	switch ( value )
	{
	case 2:
		sendPacket = 0;

		if ( player_socket.RespawnTime == Respawn_Complete ) {
			player_socket.RespawnTime = Not_Respwan;
		}

		if ( player_socket.RespawnTime != Not_Respwan || player_socket.live == false ) {
			if ( player_socket.RespawnTime == Not_Respwan ) {
				player_socket.RespawnTime = 10;
			}
			if ( player_socket.RespawnTime == 0 ) {
				player_socket.RespawnTime = Respawn_Complete;
				player_socket.live = true;
				reset_character();
			}
			else {
				player_socket.RespawnTime -= 1;
			}
		}

		if ( server_data.gr.time_wait <= 0 ) {
			// ���� ������ ī��Ʈ�ٿ� 10���� �ʱ� ��ġ�� �̵�..!
			reset_character();
		}
		if ( server_data.gr.exit_Round == true ) {
			// �������� ������ �����ϸ� ��� �������� �̵���Ų��.
			GameStart = false;
			player_socket.Game_Play = false;
			player_socket.x = 0;
			player_socket.y = -1000;
			player_socket.z = 0;
			player_client.Camx = 0;
			player_client.Camy = 0;
			player_client.Camz = 0;
			player_client.Viewx = 0;
			player_client.Viewy = 0;
			player_client.Viewz = 0;
			player_socket.live = false;
			player_socket.Game_Play = false;
		}
		glutTimerFunc( 1024, TimerFunction, 2 ); // ��Ŷ ������ Ȯ���ϱ� ���Ͽ�
		break;

	case 1:
		fPTime = fCTime;
		fCTime = GetTickCount();
		fTime = fCTime - fPTime;

		flashtimer += fTime;
		if ( flashtimer > 100.0 )
		{
			gunfire = false;
			flashtimer = 0;
		}

		if ( RotateCam )
			glutWarpPointer( width / 2, height / 2 );//�ػ󵵿� ���� ���������� �ٲ��


		//�����浹üũ
		player_socket.y += gravity;//�߷��ۿ�
		if ( player_client.Ani.jump )//���������� ��� �߷��� �ۿ�
		{
			gravity -= 0.5;
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		}

		if ( player_socket.y < 0 )//���ٴ��� �հ� �������°� ����
		{
			player_socket.y = 0;
			gravity = 0;
			player_socket.character_down_state = CHARACTER_STAND;//�ٴڿ��� �������·� �����Ǵ� ���� ����
			player_client.Ani.jump = false;
		}


		//�ִϸ��̼� ���źκ�
		PlayAnimation( player_client.Ani, player_socket.character_down_state );//�÷��̾� �ִϸ��̼�
		for ( int i = 0; i < MAX_Client; i++ )//�ٸ� �÷��̾� �ִϸ��̼�
		{
			if ( i == client_data.client_imei ) {}
			else if ( server_data.Players[i].live )
			{
				PlayAnimation( Ani[i], server_data.Players[i].character_down_state );
			}
		}

		//ĳ���� �浹�ڽ� ����
		for ( int i = 0; i < MAX_Client; i++ )
		{
			if ( (server_data.Players[i].live) && !(client_data.client_imei == i) )
			{
				//�Ӹ�
				player_collision[i][0].x = server_data.Players[i].x;
				player_collision[i][0].y = server_data.Players[i].y + 210;
				player_collision[i][0].z = server_data.Players[i].z;
				player_collision[i][0].size = 60;
				//����1
				player_collision[i][1].x = server_data.Players[i].x;
				player_collision[i][1].y = server_data.Players[i].y + 150;
				player_collision[i][1].z = server_data.Players[i].z;
				player_collision[i][1].size = 60;
				//����2
				player_collision[i][2].x = server_data.Players[i].x;
				player_collision[i][2].y = server_data.Players[i].y + 90;
				player_collision[i][2].z = server_data.Players[i].z;
				player_collision[i][2].size = 60;
				//����3
				player_collision[i][3].x = server_data.Players[i].x;
				player_collision[i][3].y = server_data.Players[i].y + 30;
				player_collision[i][3].z = server_data.Players[i].z;
				player_collision[i][3].size = 60;

				//printf("��%d\n", player_collision[i][2].size);
			}
		}

		//Ű���� �����Է¿� ���۰���
		for ( int i = 0; i < 256; i++ )
			if ( Keybuffer[i] )
				Keyinput( i );
		glutPostRedisplay();
		glutTimerFunc( Time, TimerFunction, 1 );

		break;

		//case 3:
		//	if ( server_data.gr.time_wait <= 0 ) {
		//		reset_character();
		//	}
		//	glutTimerFunc(100, TimerFunction, 3);
		//	break;
	}


}//end of TimerFunction

void Target( int x, int y )
{
	if ( RotateCam )
	{
		nx = (width / 2) - x;
		ny = (height / 2) - y;

		player_socket.camxrotate = player_socket.camxrotate + (nx / MouseSens);
		player_socket.camyrotate = player_socket.camyrotate + (ny / MouseSens);
		//left_sholder_x = player_socket.camyrotate;
		//right_sholder_x = player_socket.camyrotate;

		player_client.Viewz = 10000 * sin( (player_socket.camyrotate)* 3.141592 / 180 ) * cos( (player_socket.camxrotate)* 3.141592 / 180 );
		player_client.Viewx = 10000 * sin( (player_socket.camyrotate)* 3.141592 / 180 ) * sin( (player_socket.camxrotate)* 3.141592 / 180 );
		player_client.Viewy = 10000 * cos( (player_socket.camyrotate)* 3.141592 / 180 );

		player_client.Camz = -Camdistance * sin( (player_socket.camyrotate)* 3.141592 / 180 ) * cos( (player_socket.camxrotate + 15)* 3.141592 / 180 );
		player_client.Camx = -Camdistance * sin( (player_socket.camyrotate)* 3.141592 / 180 ) * sin( (player_socket.camxrotate + 15)* 3.141592 / 180 );
		player_client.Camy = -Camdistance * cos( (player_socket.camyrotate - 15)* 3.141592 / 180 );

		player_client.RayPos.x = (double)player_socket.x;
		player_client.RayPos.y = (double)player_socket.y + 240.0;
		player_client.RayPos.z = (double)player_socket.z;

		player_client.RayDir.x = (double)player_socket.x + (double)player_client.Viewx;
		player_client.RayDir.y = (double)player_socket.y + 240.0 + (double)player_client.Viewy;
		player_client.RayDir.z = (double)player_socket.z + (double)player_client.Viewz;

		if ( player_socket.camxrotate <= -360 || player_socket.camxrotate >= 360 )
			player_socket.camxrotate = 0;

		if ( player_socket.camyrotate < -179 )
			player_socket.camyrotate = -179;
		else if ( player_socket.camyrotate > -1 )
			player_socket.camyrotate = -1;
		bx = nx;
		by = ny;
	}
}

void Keyinput( int key )
{
	if ( key == 'w' )
	{
		player_socket.x += fTime*Charspeed * cos( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		player_socket.z += fTime*Charspeed * sin( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		if ( player_client.Ani.jump )
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
		//printf("%f, %f, %f\n", fTime, Charspeed, fTime*Charspeed);
	}
	else if ( key == 's' )
	{
		player_socket.x -= fTime*Charspeed * cos( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		player_socket.z -= fTime*Charspeed * sin( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		if ( player_client.Ani.jump )
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
	if ( key == 'a' )
	{
		player_socket.x -= fTime*Charspeed * cos( (-player_socket.camxrotate) * 3.141592 / 180 );
		player_socket.z -= fTime*Charspeed * sin( (-player_socket.camxrotate) * 3.141592 / 180 );
		if ( player_client.Ani.jump )
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
	else if ( key == 'd' )
	{
		player_socket.x += fTime*Charspeed * cos( (-player_socket.camxrotate) * 3.141592 / 180 );
		player_socket.z += fTime*Charspeed * sin( (-player_socket.camxrotate) * 3.141592 / 180 );
		if ( player_client.Ani.jump )
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
	WallCollision( &player_socket );
}

void BulletCollision( Box box[] )
{
	double Dist = 10000.0;
	double BoxShortest = 10000.0;
	double CharShortest = 10000.0;
	int Cshort = -1, Bshort = -1;
	for ( int i = 0; i < BOX_COUNT; i++ )//�� ��ֹ� �浹üũ
	{
		if ( Bullet( &box[i] ) == 1.0 )
		{
			//printf( "�����浹!\n" );
			Dist = sqrt( pow( player_socket.x - box[i].x, 2.0f ) + pow( player_socket.y - box[i].y, 2.0f ) + pow( player_socket.z - box[i].z, 2.0f ) );
			if ( BoxShortest > Dist )
			{
				BoxShortest = Dist;
				Bshort = i;
			}
		}
	}
	for ( int i = 0; i < MAX_Client; i++ )
	{
		if ( server_data.Players[i].live && !(client_data.client_imei == i) )
		{
			if ( ((Bullet( &player_collision[i][0] ) == 1.0) || (Bullet( &player_collision[i][1] ) == 1.0)) || ((Bullet( &player_collision[i][2] ) == 1.0) || (Bullet( &player_collision[i][3] ) == 1.0)) )
			{
				//printf( "�÷��̾� %d �浹!\n", i );
				Dist = sqrt( pow( player_socket.x - server_data.Players[i].x, 2.0f ) + pow( player_socket.y - server_data.Players[i].y, 2.0f ) + pow( player_socket.z - server_data.Players[i].z, 2.0f ) );
				if ( CharShortest > Dist )
				{
					CharShortest = Dist;
					Cshort = i;
				}
			}
		}
	}
	if ( CharShortest < BoxShortest )
	{
		//printf( "�÷��̾� �浹!\n" );/////////////////////////////////////�÷��̾ �Ѿ˿� ������ Ȯ���϶�//////////////////////////
		if ( player_socket.live == true && server_data.gr.round_start == true ) {
			server_data.Players[Cshort].live = false;
			player_socket.AttackedPlayer = Cshort;
		}
	}
}

double Bullet( Box *box )
{
	//vector2 BoxA, BoxB, RayA, RayB;
	//double Dist;
	return PlaneCollision( box, &player_client.RayPos, &player_client.RayDir );
	//printf("�浹!\n");
}

void reset_character() {
	if ( client_data.team )
	{
		player_socket.x = 3800;
		player_socket.y = 0;
		player_socket.z = client_data.client_imei * 300;
		player_client.Viewx = -3800;
		player_client.Viewy = 0;
		player_client.Viewz = client_data.client_imei * 300;
		player_socket.camxrotate = -276.360107;
		player_socket.camyrotate = -92.9999992;
	}
	else
	{
		player_socket.x = -3800;
		player_socket.y = 0;
		player_socket.z = client_data.client_imei * 300;
		player_client.Viewx = 3800;
		player_client.Viewy = 0;
		player_client.Viewz = client_data.client_imei * 300;
		player_socket.camxrotate = -96.400032;
		player_socket.camyrotate = -89.919991;
	}
}

void WallCollision( Player_Socket *player_socket )
{
	if ( player_socket->x < -3950 )
		player_socket->x = -3950;
	else if ( player_socket->x > 3950 )
		player_socket->x = 3950;
	if ( player_socket->z < -3950 )
		player_socket->z = -3950;
	else if ( player_socket->z > 3950 )
		player_socket->z = 3950;
}

void SetBoxState() {
	for ( int i = 0; i < 25; i += 2 ) {
		Mapping_Box( box[i], 3, 0, i, 0 ); // x, y, z, image
	}
	int j = 1;
	for ( int i = 25; i < 38; ++i ) {
		Mapping_Box( box[i], 23, 0, j, 0 ); // x, y, z, image
		j += 2;
	}

	//------------------------------------------------------------------
	// �߾� �ڽ�
	box[1].x = 0;
	box[1].y = 0;
	box[1].z = 0;
	box[1].size = 600;
	box[1].image = 1;

	box[3].x = 0;
	box[3].y = 0;
	box[3].z = 2000;
	box[3].size = 600;
	box[3].image = 1;

	box[5].x = 0;
	box[5].y = 0;
	box[5].z = -2000;
	box[5].size = 600;
	box[5].image = 1;
	//------------------------------------------------------------------
	box[7].x = -1600;
	box[7].y = 0;
	box[7].z = 900;
	box[7].size = 400;
	box[7].image = 2;

	box[9].x = -1600;
	box[9].y = 0;
	box[9].z = 2900;
	box[9].size = 400;
	box[9].image = 2;

	box[11].x = -1600;
	box[11].y = 0;
	box[11].z = -2900;
	box[11].size = 400;
	box[11].image = 2;

	box[13].x = -1600;
	box[13].y = 0;
	box[13].z = -1000;
	box[13].size = 400;
	box[13].image = 2;
	//------------------------------------------------------------------
	box[15].x = 1600;
	box[15].y = 0;
	box[15].z = 900;
	box[15].size = 400;
	box[15].image = 2;

	box[17].x = 1600;
	box[17].y = 0;
	box[17].z = 2900;
	box[17].size = 400;
	box[17].image = 2;

	box[19].x = 1600;
	box[19].y = 0;
	box[19].z = -2900;
	box[19].size = 400;
	box[19].image = 2;

	box[21].x = 1600;
	box[21].y = 0;
	box[21].z = -1000;
	box[21].size = 400;
	box[21].image = 2;
	//------------------------------------------------------------------
}

void draw_score( int w, int h, Server_Player server_data )
{
	int len;

	glPushMatrix();//draw_score ����

	glViewport( 0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, w, h, 0, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable( GL_LIGHTING );      // ���� Ȱ��ȭ
	glDisable( GL_COLOR_MATERIAL );
	glDisable( GL_NORMALIZE );


	char buf[MAX_PATH];
	glPushMatrix();//�����ð� ǥ��
	glColor3f( 1, 1, 1 );//�Ķ����۾�
	glTranslatef( 0, 200, 0 );//�۾���ġ
	glRasterPos2f( 0.0, 0.0 );

	//-------------------------------------------------------------------------
	//�� �̸� ǥ��
	glPushMatrix();
	for ( int j = 0; j < 16; j++ )//A��
		buf[j] = '\0';
	glTranslatef( 0, 0, 0 );
	glRasterPos2f( 100.0, 0.0 );
	sprintf( buf, "A Team" );
	len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();

	glPushMatrix();
	for ( int j = 0; j < 16; j++ )//B��
		buf[j] = '\0';
	glTranslatef( w / 2 + 30, 0, 0 );
	glRasterPos2f( 0.0, 0.0 );
	sprintf( buf, "B Team" );
	len = (int)strlen( buf );
	for ( int i = 0; i < len; i++ )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
	glPopMatrix();
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//Team A(bool�� false)
	glPushMatrix();//Team A ǥ�� ����
	glTranslatef( 0, 30, 0 );
	for ( int i = 0; i < MAX_Client / 2; i++ )
	{
		glTranslatef( 0, 30, 0 );
		glPushMatrix();
		for ( int j = 0; j < 16; j++ )
			buf[j] = '\0';
		glTranslatef( 100, 0, 0 );
		glRasterPos2f( 0.0, 0.0 );
		sprintf( buf, "%s = ", server_data.Players[i * 2].nickName );
		len = (int)strlen( buf );
		for ( int i = 0; i < len; i++ )
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );


		for ( int j = 0; j < 16; j++ )
			buf[j] = '\0';
		glTranslatef( 140, 0, 0 );
		glRasterPos2f( 0.0, 0.0 );
		sprintf( buf, "%d : %d", server_data.Players[i * 2].kill, server_data.Players[i * 2].death );
		len = (int)strlen( buf );
		for ( int i = 0; i < len; i++ )
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
		glPopMatrix();

		//printf("%d", i);
	}
	glPopMatrix();//Team A ǥ�� ����
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//Team B(bool�� true)
	glPushMatrix();//Team B ǥ�� ����
	glTranslatef( w / 2 - 70, 30, 0 );
	for ( int i = 0; i < MAX_Client / 2; i++ )
	{
		glTranslatef( 0, 30, 0 );
		glPushMatrix();
		for ( int j = 0; j < 16; j++ )
			buf[j] = '\0';
		glTranslatef( 100, 0, 0 );
		glRasterPos2f( 0.0, 0.0 );
		sprintf( buf, "%s = ", server_data.Players[(i * 2) + 1].nickName );
		len = (int)strlen( buf );
		for ( int i = 0; i < len; i++ )
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );


		for ( int j = 0; j < 16; j++ )
			buf[j] = '\0';
		glTranslatef( 140, 0, 0 );
		glRasterPos2f( 0.0, 0.0 );
		sprintf( buf, "%d : %d", server_data.Players[(i * 2) + 1].kill, server_data.Players[(i * 2) + 1].death );
		len = (int)strlen( buf );
		for ( int i = 0; i < len; i++ )
			glutBitmapCharacter( GLUT_BITMAP_9_BY_15, buf[i] );
		glPopMatrix();

		//printf("%d", i);
	}
	glPopMatrix();//Team B ǥ�� ����
	//-------------------------------------------------------------------------


	glPopMatrix();//�����ð� ǥ�� ����


	glPopMatrix();//draw_score ����
}