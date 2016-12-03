#include "main.h"

GLvoid DrawScene( GLvoid );
GLvoid Reshape( int w, int h );

// 다이얼로그 관련
HINSTANCE hInstance;

//서버관련
SOCKET sock;

//캐릭터 구조체
Player player_client;
Player_Socket player_socket;
Server_Player server_data;
int client_imei = -1;//몇번째 클라인지 기억하기

Player_Animation Ani[MAX_Client];

//필수 함수들
void Mouse( int button, int state, int x, int y );
void Keyboard( unsigned char key, int x, int y );
void Keyboardup( unsigned char key, int x, int y );
void Motion( int x, int y );
void TimerFunction( int value );

void Target( int x, int y );//카메라 시점관련 함수
void Keyinput( int key );//키보드 동시입력을 위한 입력처리 함수
//void crashCheck();//충돌체크

//타이머 갱신시간
int Time = 10, Bullet_Time = 1;
float fTime, fCTime, fPTime;
bool err;


//해상도
int width, height;
char Input;

//UI관련 변수들
int rifleammo = 120, rifleload = 30, hp = 100;
char ammo[10], health[10], PressSpace[50];
int len;

//캐릭터 및 카메라 관련 변수
float nx = 0, ny = 0, bx = 0, by = 0;//마우스 움직임에 쓰이는 임시변수
float gravity = 0, Charspeed = 0.7f;
int  Camdistance = 400, MouseSens = 25;
bool RotateCam = false, FirstPersonView = true;
bool Keybuffer[256];//동시입력에 사용되는 버퍼

//게임진행 관련 함수
bool GameStart = false;


//애니메이션 변수
//float left_sholder_x, left_sholder_y, left_sholder_z, right_sholder_x, right_sholder_y, right_sholder_z, left_elbow_x, right_elbow_x;//팔회전


// 조명 설정
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
	while ( 1 ) {
		send_Player( sock, player_socket );
		server_data = recv_Player( sock );
		sendPacket++;
		//printf( "Client ID %d : %f\n", client_imei, server_data.player[client_imei].camxrotate );
	}
	return 0;
}

DWORD WINAPI DialogBox_Thread( LPVOID arg ) {
	// 대화상자 생성
	//RotateCam = false;
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	//서버접속시도
	HANDLE hThread;
	sock = init_sock();
	client_imei = get_ClientID( sock );
	player_socket.team = get_ClientTeam( sock );
	hThread = CreateThread( NULL, 0, SR_Thread, 0, 0, NULL );
	GamePlayWait = false;
	//플레이어의 데이터를 받을 때 init_sock으로 헤더 포인터만 넘겨주면 되지 않을까?
	//Win_Sock.cpp에서 연결리스트의 삭제 추가를 구현하면 될듯
	//권창에게 코드 받으면 테스트해보자
	return 0;
}

void main()
{
	srand( (unsigned)time( NULL ) );

	HANDLE hThread;
	hThread = CreateThread( NULL, 0, DialogBox_Thread, 0, 0, NULL );

	//윈도우 초기화 및 생성
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); //디스플레이 모드 설정
	//glutInitWindowPosition(500, 0); // 윈도우 위치 지정
	glutInitWindowPosition( (glutGet( GLUT_SCREEN_WIDTH ) - Window_Width) / 2, (glutGet( GLUT_SCREEN_HEIGHT ) - Window_Height) / 2 ); // 화면 중앙으로 정렬하기
	glutInitWindowSize( Window_Width, Window_Height ); //윈도우 크기 지정
	glutCreateWindow( "NetworkGame TermProject" ); //윈도우 생성(윈도우 이름)
	init_Character_Texture();
	init_Zombie_Texture();
	init_Map_Texture();

	//상태 변수 초기화 함수
	for ( int i = 0; i < 256; i++ )
		Keybuffer[i] = false;


	//필요한 콜백 함수 설정

	glutDisplayFunc( DrawScene ); //출력 함수의 지정
	glutTimerFunc( Time, TimerFunction, 1 ); //타이머 콜백 함수
	glutTimerFunc( 1024, TimerFunction, 2 ); // 패킷 전송을 확인하기 위하여

	glutPassiveMotionFunc( Motion );//마우스모션
	glutMouseFunc( Mouse );//마우스입력
	glutMotionFunc( Motion );//클릭시에도 움직이게끔 마우스모션 적용

	glutKeyboardFunc( Keyboard );//키보드입력
	glutKeyboardUpFunc( Keyboardup );//키보드 버튼을 뗐을 때

	glutReshapeFunc( Reshape );//다시그리기
	glutMainLoop();//이벤트 루프 실행하기

}

//윈도우 출력 함수
GLvoid DrawScene( GLvoid )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );//바탕색을'black'로지정
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// 설정된 색으로 전체를 칠하기

	glViewport( 0, 0, width, height );//뷰포트 설정
	glMatrixMode( GL_PROJECTION );//거리별로 사물이 가려지게 그리는 모드
	glLoadIdentity();//초기화? 찾아보고 이해하자
	gluPerspective( 60.0f, (float)width / (float)height, 0.1, 10000.0 );//시야각 + 그려줄 거리
	glMatrixMode( GL_MODELVIEW );//???뭐였지

	glLoadIdentity();//초기화? 찾아보고 이해하자

	update_FPS();


	if ( GameStart )
	{
		//1인칭일때
		if ( FirstPersonView ) { gluLookAt( player_socket.x, player_socket.y + 240, player_socket.z, player_socket.x + player_client.Viewx, player_socket.y + player_client.Viewy + 170, player_socket.z + player_client.Viewz, 0.0, 1.0, 0.0 ); }
		//3인칭일때
		else { gluLookAt( player_socket.x + player_client.Camx, player_socket.y + player_client.Camy + 190, player_socket.z + player_client.Camz, player_socket.x + player_client.Viewx, player_socket.y + player_client.Viewy + 190, player_socket.z + player_client.Viewz, 0.0, 1.0, 0.0 ); }

		//조명설정
		glEnable( GL_DEPTH_TEST );			// 가려진 면 제거
		glEnable( GL_CULL_FACE );				// 후면 제거

		//맵 그리기
		DrawMap();

		//플레이어 그리기
		if ( player_socket.live )
			drawCharacter(&player_socket, &player_client.Ani);

		//다른 플레이어 그리기
		for ( int i = 0; i < MAX_Client; i++ )
		{
			if ( (i == client_imei) || (server_data.Players[i].live == false) ) {}
			else
			{
				if ( player_socket.team == server_data.Players[i].team )
					drawCharacter( &server_data.Players[i], &Ani[i]);
				else
					drawZombie( &server_data.Players[i], &Ani[i]);
			}
		}

		drawHud( width, height );//HUD 그리는 부분 내부에 push pop 알아서 함
	}
	else
	{
		//관전자 시점 설정
		gluLookAt( 2500, 1800, 2500, 0, 0, 0, 0.0, 1.0, 0.0 );

		//조명설정
		glEnable(GL_DEPTH_TEST);			// 가려진 면 제거
		glEnable(GL_CULL_FACE);				// 후면 제거


		//맵 그리기
		DrawMap();

		//다른 플레이어 그리기
		for ( int i = 0; i < MAX_Client; i++ )
		{
			if ( (i == client_imei) || (server_data.Players[i].live == false) ) {}
			else
			{
				if (player_socket.team == server_data.Players[i].team)
					drawCharacter(&server_data.Players[i], &Ani[i]);
				else
					drawZombie(&server_data.Players[i], &Ani[i]);
			}
		}

		glPushMatrix();//글자표시 시작
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
		glPopMatrix();//글자표시 끝

	}

	glutSwapBuffers(); //화면에 출력하기
}//end of drawScene


GLvoid Reshape( int w, int h )
{
	//뷰포트 변환 설정
	glViewport( 0, 0, w, h );
	width = w;
	height = h;

	//투영 행렬 스택 재설정
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	//클리핑 공간 설정 : 원근투영
	gluPerspective( 60.0f, (float)w / (float)h, 0.1, 5000.0 );


	//모델 뷰 행렬 스택 재설정
	glMatrixMode( GL_MODELVIEW );
}

void Mouse( int button, int state, int x, int y )//마우스 클릭관련 체크
{
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )//좌클릭
	{
	}
	if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )//우클릭
	{
		if ( FirstPersonView ) {//1인칭일경우 정조준
			if (player_client.Ani.sight) {
				player_client.Ani.sight = false;
				//turn_Light = true;
			}
			else {
				player_client.Ani.sight = true;
				//turn_Light = true;
			}
		}
		else {}
	}
}//end of Mouse

void Motion( int x, int y )//마우스 입력(좌표값)을 받자마자 Target으로 넘겨주고 Target에서 Viewport처리
{
	Target( x, y );
}

void Keyboard( unsigned char key, int x, int y )
{
	Keybuffer[key] = true;

	switch ( key )//
	{
	case ' '://스페이스바
		if ( GameStart )
		{
			if (player_client.Ani.jump) {}
			else
			{
				player_client.Ani.jump = true;
				gravity = 15;
				Keybuffer[key] = false;
			}
		}
		else
		{
			//if ( GamePlayWait == true ) break; // 다이얼로그 박스가 끝나야지만 게임 플레이 가능하게 한다.

			// 게임 시작을 할시 적용해준다.
			glutSetCursor( GLUT_CURSOR_NONE );//마우스커서 없애기
			RotateCam = true;

			GameStart = true;
			player_socket.live = true;
			strcpy( player_socket.nickName, nickName );
			if ( player_socket.team )
			{
				player_socket.x = 2000;
				player_socket.y = 0;
				player_socket.z = 0;
				player_client.Viewx = -2000;
				player_client.Viewy = 0;
				player_client.Viewz = 0;
			}
			else
			{
				player_socket.x = -2000;
				player_socket.y = 0;
				player_socket.z = 0;
				player_client.Viewx = 2000;
				player_client.Viewy = 0;
				player_client.Viewz = 0;
			}
		}
		break;
	case 'c':
		glutSetCursor( GLUT_CURSOR_NONE );
		if (RotateCam)
		{
			RotateCam = false;
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);//마우스커서 없애기
		}
		else
		{
			RotateCam = true;
			glutSetCursor(GLUT_CURSOR_NONE);//마우스커서 없애기
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
		printf( "\n%d\n", player_socket.live );
		for ( int i = 0; i < MAX_Client; i++ )
			printf( "%d", server_data.Players[i].live );
		printf( "\n" );
		for (int i = 0; i < MAX_Client; i++)
			printf("%d", server_data.Players[i].character_down_state);

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
		}
		else exit( 0 );
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
		glutTimerFunc( 1024, TimerFunction, 2 ); // 패킷 전송을 확인하기 위하여
		break;

	case 1:
		fPTime = fCTime;
		fCTime = GetTickCount();
		fTime = fCTime - fPTime;

		if ( RotateCam )
			glutWarpPointer( width / 2, height / 2 );//해상도에 따라 유동적으로 바뀌도록


		//간단충돌체크
		player_socket.y += gravity;//중력작용
		if ( player_client.Ani.jump )//점프상태일 경우 중력이 작용
		{
			gravity -= 0.5;
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		}

		if ( player_socket.y < 0 )//땅바닥을 뚫고 내려가는걸 방지
		{
			player_socket.y = 0;
			gravity = 0;
			player_socket.character_down_state = CHARACTER_STAND;//바닥에서 점프상태로 고정되는 현상 방지
			player_client.Ani.jump = false;
		}


		//애니메이션 갱신부분
		PlayAnimation( player_client.Ani, player_socket.character_down_state );//플레이어 애니메이션
		for ( int i = 0; i < MAX_Client; i++ )//다른 플레이어 애니메이션
		{
			if ( i == client_imei ) {}
			else if ( server_data.Players[i].live )
			{
				PlayAnimation( Ani[i], server_data.Players[i].character_down_state );
			}
		}


		for ( int i = 0; i < 256; i++ )
			if ( Keybuffer[i] )
				Keyinput( i );
		glutPostRedisplay();
		glutTimerFunc( Time, TimerFunction, 1 );

		break;
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
		if (player_client.Ani.jump)
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
		//printf("%f, %f, %f\n", fTime, Charspeed, fTime*Charspeed);
	}
	else if ( key == 's' )
	{
		player_socket.x -= fTime*Charspeed * cos( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		player_socket.z -= fTime*Charspeed * sin( (-player_socket.camxrotate - 90) * 3.141592 / 180 );
		if (player_client.Ani.jump)
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
	if ( key == 'a' )
	{
		player_socket.x -= fTime*Charspeed * cos( (-player_socket.camxrotate) * 3.141592 / 180 );
		player_socket.z -= fTime*Charspeed * sin( (-player_socket.camxrotate) * 3.141592 / 180 );
		if (player_client.Ani.jump)
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
	else if ( key == 'd' )
	{
		player_socket.x += fTime*Charspeed * cos( (-player_socket.camxrotate) * 3.141592 / 180 );
		player_socket.z += fTime*Charspeed * sin( (-player_socket.camxrotate) * 3.141592 / 180 );
		if (player_client.Ani.jump)
			player_socket.character_down_state = CHARACTER_CROUCH_JUMP;
		else
			player_socket.character_down_state = CHARACTER_WALK;
	}
}
