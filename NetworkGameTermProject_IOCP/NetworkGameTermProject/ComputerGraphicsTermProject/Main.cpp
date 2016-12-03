#define _CRT_SECURE_NO_WARNINGS
#include "Texture_Load.h"
#include <math.h>

GLvoid DrawScene(GLvoid);
GLvoid Reshape(int w, int h);

SOCKET sock;
Player_Socket player;
Server_Player server_data;
int client_imei = -1; //몇번째 클라인지 기억하기

typedef struct Bullet
{
	float x, y, z;
	float fx, fy, fz;
	Bullet * next;
}Bullet;

void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);
void Motion(int x, int y);
void TimerFunction(int value);
void MenuFunc(int button);

void drawCharacter();//캐릭터 드로우 함수

void draw_Zombie_Player(Server_Player, int); // 상대팀 그리기 New
void drawZombie( Server_Player , int); // 좀비 드로우 함수


void init_Texture();//텍스쳐 로드 함수
void animationCharleg();//캐릭터 다리 애니메이션 함수
void animationChararm();//캐릭터 팔 애니메이션 함수
void Target(int x, int y);//카메라 시점관련 함수
void Keyinput(int key);//키보드 동시입력을 위한 입력처리 함수
void drawHud();//HUD 그리는 함수
void crashCheck();//충돌체크

void drawPistol();//권총그리기
void bulletCreate();//총알 생성
void bulletDestroy(Bullet * Del);//총알 파괴
void drawBullet();//총알 그리기

int Time = 10, Bullet_Time = 1;//타이머 갱신시간
int width, height;
char Input;

int rifleammo = 120, rifleload = 30, hp = 100;
char ammo[10], health[10];

//캐릭터 및 카메라 관련 변수
float  Viewx = 0, Viewy = 0, Viewz = -1000, Camx, Camy, Camz;
//float player.x = 0, player.z = 0, player.y = 0;
float nx = 0, ny = 0, bx = 0, by = 0;
float testx = 0, texty = 0;
float gravity = 0;
int Charspeed = 8, Camdistance = 400, MouseSens = 25;
bool RotateCam = true, FirstPersonView = true;
bool Keybuffer[256];

// FPS 측정을 위한 변수
float fps;
char draw_FPS[100];
void update_FPS();

//애니메이션 변수
int character_up_state = 2, character_down_state = 0, legtimer = 0;//이동 애니메이션 구분용
float head_angle_x;//머리회전
float left_sholder_x, left_sholder_y, left_sholder_z, right_sholder_x, right_sholder_y, right_sholder_z, left_elbow_x, right_elbow_x;//팔회전
float left_leg_x, left_leg_y, left_knee_x, right_leg_x, right_leg_y, right_knee_x, left_leg_z, right_leg_z;//다리회전
bool sight = false, jump = false, crouch = false;//정조준

//맵 데이터 변수
int map_DATA[6][72][27];

//충돌체크 거리
int crashdist = 20;

//총알데이터
Bullet * Head = NULL;
Bullet * Temp = NULL;
Bullet * CTemp = NULL;
Bullet * DTemp = NULL;

// 조명 설정
bool turn_Light = true;
void show_Light(){
	GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0f };
	GLfloat DiffuseLight[] = { 0.8, 0.8, 0.8, 1.0f };
	GLfloat lightpos[] = { player.x, 200, player.z, 1 };
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


DWORD WINAPI SR_Thread( LPVOID arg ) {
	while ( 1 ) {
		send_Player( sock, player );
		server_data = recv_Player( sock );
		//printf( "Client ID %d : %f\n", client_imei, server_data.player[client_imei].camxrotate );
	}
	return 0;
}

void main()
{
	HANDLE hThread;
	srand((unsigned)time(NULL));

	sock = init_sock();
	client_imei = get_ClientID( sock );
	hThread = CreateThread( NULL, 0, SR_Thread, (LPVOID)sock, 0, NULL );


	//윈도우 초기화 및 생성
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //디스플레이 모드 설정
	glutInitWindowPosition(500, 0); // 윈도우 위치 지정
	glutInitWindowSize(800, 600); //윈도우 크기 지정
	glutCreateWindow("ComputerGrapgics TermProject"); //윈도우 생성(윈도우 이름)

	//상태 변수 초기화 함수
	glutSetCursor(GLUT_CURSOR_NONE);//마우스커서 없애기
	init_Texture();//텍스쳐 불러오기
	for (int i = 0; i < 256; i++)
		Keybuffer[i] = false;

	//필요한 콜백 함수 설정
	glutDisplayFunc(DrawScene); //출력 함수의 지정
	glutTimerFunc(Time, TimerFunction, 1);//타이머 콜백 함수
	glutTimerFunc(Bullet_Time, TimerFunction, 2);

	glutPassiveMotionFunc(Motion);//마우스모션
	glutMouseFunc(Mouse);//마우스입력
	glutMotionFunc(Motion);//클릭시에도 움직이게끔 마우스모션 적용

	glutKeyboardFunc(Keyboard);//키보드입력
	glutKeyboardUpFunc(Keyboardup);//키보드 버튼을 뗐을 때

	//PlaySound("SoundData/BGM.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	glutReshapeFunc(Reshape);//다시그리기
	glutMainLoop();//이벤트 루프 실행하기
}

void draw_Zombie_Player( Server_Player server_data, int client_imei ) {
	for ( int i = 0; i < MAX_Client; ++i ) {
		if ( i != client_imei && server_data.player[i].x != -1000 ) {
			glPushMatrix();
			glTranslatef( server_data.player[i].x, server_data.player[i].y + 70, server_data.player[i].z );

			glRotatef( server_data.player[i].camxrotate + 180, 0, 1, 0 );   //캐릭터 몸통 전체 회전
			drawZombie(server_data, i);
			glPopMatrix();
		}
	}
}

//윈도우 출력 함수
GLvoid DrawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//바탕색을'black'로지정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 설정된 색으로 전체를 칠하기

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)width / (float)height, 0.1, 10000.0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);      // 조명 활성화
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	if (turn_Light == true){
		glEnable(GL_LIGHT0);
		show_Light();
	}
	else{
		glDisable(GL_LIGHT0);
	}

	//3D Draw----------------------------------------------------------------------
	glLoadIdentity();
	update_FPS();
	
	if (FirstPersonView)
	{
		if (character_down_state == 3)
			gluLookAt(player.x, player.y + 100, player.z, player.x + Viewx, player.y + Viewy + 170, player.z + Viewz, 0.0, 1.0, 0.0);
		else
			gluLookAt(player.x, player.y + 170, player.z, player.x + Viewx, player.y + Viewy + 170, player.z + Viewz, 0.0, 1.0, 0.0);
	}
	else
	{
		if (character_down_state == 3)
			gluLookAt(player.x + Camx, player.y + Camy + 120, player.z + Camz, player.x + Viewx, player.y + Viewy + 190, player.z + Viewz, 0.0, 1.0, 0.0);
		else
			gluLookAt(player.x + Camx, player.y + Camy + 190, player.z + Camz, player.x + Viewx, player.y + Viewy + 190, player.z + Viewz, 0.0, 1.0, 0.0);
	}

	glPushMatrix();
	//조명설정
	glEnable(GL_DEPTH_TEST);                              // 가려진 면 제거
	glEnable(GL_CULL_FACE);                               // 후면 제거

	glPushMatrix();//캐릭터 그리기
	if (character_down_state == 0 || character_down_state == 1 || character_down_state == 2)
		glTranslatef(player.x, player.y + 70, player.z);      //캐릭터 위치 이동
	else if (character_down_state == 3)
		glTranslatef(player.x, player.y, player.z);      //캐릭터 위치 이동

	glRotatef(player.camxrotate + 180, 0, 1, 0);   //캐릭터 몸통 전체 회전
	drawCharacter();//캐릭터 그리기
	if (character_up_state == 1 || character_up_state == 2)
		drawPistol();//권총그리기
	glPopMatrix();//캐릭터그리기 end

	//3D END------------------------------------------------------------------------------
	drawBullet();//총알그리기

	glPushMatrix();//좀비 그리기
	draw_Zombie_Player( server_data, client_imei );
	glPopMatrix();//좀비 그리기 end


	glPushMatrix();//블록그리기
	glTranslatef(60, 0, 60);

	glPushMatrix();//땅
	draw_Ground(block_Nomal_object);
	glPopMatrix();//땅end
	glPushMatrix();//벽돌
	draw_Wall(block_Brick_object);
	glPopMatrix();//벽돌end
	glPushMatrix();//나무
	draw_Wooden(block_Wooden_object);
	glPopMatrix();//나무end
	glPushMatrix();//돌
	draw_Stone(block_Stone_object);
	glPopMatrix();//돌end

	glPopMatrix();//블록그리기 end

	//2D Draw-----------------------------------------------------------------------------
	drawHud();
	//2D END------------------------------------------------------------------------------
	glPopMatrix();
	glutSwapBuffers(); //화면에 출력하기
}//end of drawScene

GLvoid Reshape(int w, int h)
{
	//뷰포트 변환 설정
	glViewport(0, 0, w, h);
	width = w;
	height = h;

	//투영 행렬 스택 재설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//클리핑 공간 설정 : 원근투영
	gluPerspective(60.0f, (float)w / (float)h, 0.1, 5000.0);

	//모델 뷰 행렬 스택 재설정
	glMatrixMode(GL_MODELVIEW);

}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bulletCreate();
		glutTimerFunc(Bullet_Time, TimerFunction, 2);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (FirstPersonView){
			if (sight){
				sight = false;
				turn_Light = true;
			}
			else{
				sight = true;
				turn_Light = true;
			}
		}
		else{}

	}
}//end of Mouse

void Motion(int x, int y)
{
	Target(x, y);
}

void Keyboard(unsigned char key, int x, int y)
{
	Keybuffer[key] = true;
	switch (key)
	{
	case ' '://스페이스바
		if (jump){}
		else
		{
			jump = true;
			gravity = 15;
		}
		break;
	}
}//end of Keyboard

void Keyboardup(unsigned char key, int x, int y)
{
	Keybuffer[key] = false;
	if (!Keybuffer['w'] && !Keybuffer['a'] && !Keybuffer['s'] && !Keybuffer['d'])
		character_down_state = 0;
	if (Keybuffer['x'] == false)
	{
		Charspeed = 8;
		crouch = false;
	}
}

void TimerFunction(int value)
{
	switch (value)
	{
	case 1:
		if (RotateCam)
			glutWarpPointer(400, 300);
		animationCharleg();//캐릭터 다리 애니메이션
		animationChararm();//캐릭터 팔 애니메이션

		player.y += gravity;
		if (jump)
		{
			gravity -= 0.5;
			character_down_state = 3;
		}


		for (int i = 0; i < 256; i++)
			if (Keybuffer[i])
				Keyinput(i);
		crashCheck();
		glutPostRedisplay();
		glutTimerFunc(Time, TimerFunction, 1);
		break;
	case 2:
		if (Head != NULL)
		{
			Temp = Head;
			while (Temp != NULL)
			{
				Temp->x += Temp->fx;
				Temp->y += Temp->fy;
				Temp->z += Temp->fz;
				if ((Temp->x < (-37 * 120)) || (Temp->x >(37 * 120)) || (Temp->y < 0) || (Temp->y >(7 * 120)) || (Temp->z < (-14 * 120)) || (Temp->z >(14 * 120)))
				{
					Bullet * Del = Temp;
					Temp = Temp->next;
					bulletDestroy(Del);
				}
				else
					Temp = Temp->next;
			}
			glutPostRedisplay();
			glutTimerFunc(Bullet_Time, TimerFunction, 2);
		}

		break;
	}


}//end of TimerFunction

void drawCharacter(){
	glPushMatrix(); //Save 머리
	glTranslated(0, 75, 0);
	glRotatef(-(player.camyrotate + 90) / 3, 1, 0, 0);
	glScaled(1.0, 1.0, 0.7);
	drawBoxFront(30, false, character_head_object[0]);
	drawBoxBack(30, false, character_head_object[1]);
	drawBoxLeft(30, false, character_head_object[2]);
	drawBoxRight(30, false, character_head_object[3]);
	drawBoxTop(30, false, character_head_object[4]);
	drawBoxBottom(30, false, character_head_object[5]);
	glPopMatrix(); //머리 end

	glPushMatrix(); //Save 몸통
	glScaled(1.0, 1.5, 0.5);
	drawBoxFront(30, false, character_body_object[0]);
	drawBoxBack(30, false, character_body_object[1]);
	drawBoxLeft(30, false, character_body_object[2]);
	drawBoxRight(30, false, character_body_object[3]);
	drawBoxTop(30, false, character_body_object[4]);
	drawBoxBottom(30, false, character_body_object[5]);
	glPopMatrix(); //몸통 end


	if (sight){}
	else
	{
		if (character_up_state == 1 || character_up_state == 2)
		{
			glPushMatrix(); //Save 오른팔 어께
			glTranslated(-45, 44, 0);
			glRotatef(-right_sholder_x + 210, 1, 0, 0);
			glRotatef(right_sholder_y, 0, 1, 0);
			glRotatef(right_sholder_z, 0, 0, 1);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_top_object[0]);
			drawBoxBack(30, true, character_arm_top_object[1]);
			drawBoxLeft(30, true, character_arm_top_object[2]);
			drawBoxRight(30, true, character_arm_top_object[3]);
			drawBoxTop(30, true, character_arm_top_object[4]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glScaled(2, 1.333333, 2);

			glPushMatrix(); //Save 오른팔 팔꿈치
			glTranslated(0, -45, 0);
			glRotatef(right_elbow_x, 1, 0, 0);
			glPushMatrix();
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_bottom_object[0]);
			drawBoxBack(30, true, character_arm_bottom_object[1]);
			drawBoxLeft(30, true, character_arm_bottom_object[2]);
			drawBoxRight(30, true, character_arm_bottom_object[3]);
			drawBoxTop(30, true, character_arm_top_object[5]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glPopMatrix();
			glPopMatrix();//오른팔 팔꿈치 종료
			glPopMatrix();//오른팔 어께 종료


			glPushMatrix(); //Save 왼팔 어께 
			glTranslated(45, 44, 0);
			glRotatef(-left_sholder_x - 90, 1, 0, 0);
			glRotatef(left_sholder_y, 0, 1, 0);
			glRotatef(left_sholder_z, 0, 0, 1);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_top_object[0]);
			drawBoxBack(30, true, character_arm_top_object[1]);
			drawBoxLeft(30, true, character_arm_top_object[2]);
			drawBoxRight(30, true, character_arm_top_object[3]);
			drawBoxTop(30, true, character_arm_top_object[4]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glScaled(2, 1.333333, 2);

			glPushMatrix(); //Save 왼팔 팔꿈치
			glTranslated(0, -45, 0);
			glRotatef(left_elbow_x, 1, 0, 0);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_bottom_object[0]);
			drawBoxBack(30, true, character_arm_bottom_object[1]);
			drawBoxLeft(30, true, character_arm_bottom_object[2]);
			drawBoxRight(30, true, character_arm_bottom_object[3]);
			drawBoxTop(30, true, character_arm_top_object[5]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glPopMatrix();//왼팔 팔꿈치 종료
			glPopMatrix();//왼팔어께 종료
		}
		else if (character_up_state == 0)
		{
			glPushMatrix(); //Save 오른팔 어께
			glTranslated(-45, 44, 0);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_top_object[0]);
			drawBoxBack(30, true, character_arm_top_object[1]);
			drawBoxLeft(30, true, character_arm_top_object[2]);
			drawBoxRight(30, true, character_arm_top_object[3]);
			drawBoxTop(30, true, character_arm_top_object[4]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glScaled(2, 1.333333, 2);

			glPushMatrix(); //Save 오른팔 팔꿈치
			glTranslated(0, -45, 0);
			glPushMatrix();
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_bottom_object[0]);
			drawBoxBack(30, true, character_arm_bottom_object[1]);
			drawBoxLeft(30, true, character_arm_bottom_object[2]);
			drawBoxRight(30, true, character_arm_bottom_object[3]);
			drawBoxTop(30, true, character_arm_top_object[5]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glPopMatrix();
			glPopMatrix();//오른팔 팔꿈치 종료
			glPopMatrix();//오른팔 어께 종료


			glPushMatrix(); //Save 왼팔 어께 
			glTranslated(45, 44, 0);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_top_object[0]);
			drawBoxBack(30, true, character_arm_top_object[1]);
			drawBoxLeft(30, true, character_arm_top_object[2]);
			drawBoxRight(30, true, character_arm_top_object[3]);
			drawBoxTop(30, true, character_arm_top_object[4]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glScaled(2, 1.333333, 2);

			glPushMatrix(); //Save 왼팔 팔꿈치
			glTranslated(0, -45, 0);
			glScaled(0.5, 0.75, 0.5);
			drawBoxFront(30, true, character_arm_bottom_object[0]);
			drawBoxBack(30, true, character_arm_bottom_object[1]);
			drawBoxLeft(30, true, character_arm_bottom_object[2]);
			drawBoxRight(30, true, character_arm_bottom_object[3]);
			drawBoxTop(30, true, character_arm_top_object[5]);
			drawBoxBottom(30, true, character_arm_top_object[5]);
			glPopMatrix();//왼팔 팔꿈치 종료
			glPopMatrix();//왼팔어께 종료
		}
	}


	glPushMatrix(); //Save 오른쪽 골반
	glTranslated(-15, -40, 0);
	glRotatef(right_leg_x, 1, 0, 0);
	glRotatef(right_leg_y, 0, 1, 0);
	glRotatef(right_leg_z, 0, 0, 1);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, character_leg_top_object[0]);
	drawBoxBack(30, true, character_leg_top_object[1]);
	drawBoxLeft(30, true, character_leg_top_object[2]);
	drawBoxRight(30, true, character_leg_top_object[3]);
	drawBoxTop(30, true, character_leg_top_object[4]);
	drawBoxBottom(30, true, character_leg_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 오른쪽 무릎
	glTranslated(0, -45, 0);
	glRotatef(right_knee_x, 1, 0, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, character_leg_bottom_object[0]);
	drawBoxBack(30, true, character_leg_bottom_object[1]);
	drawBoxLeft(30, true, character_leg_bottom_object[2]);
	drawBoxRight(30, true, character_leg_bottom_object[3]);
	drawBoxTop(30, true, character_leg_top_object[4]);
	drawBoxBottom(30, true, character_leg_top_object[5]);
	glPopMatrix();//오른쪽 무릎 종료

	glPopMatrix();//오른쪽 골반 종료


	glPushMatrix(); //Save 왼쪽 골반
	glTranslated(15, -40, 0);
	glRotatef(left_leg_x, 1, 0, 0);
	glRotatef(left_leg_y, 0, 1, 0);
	glRotatef(left_leg_z, 0, 0, 1);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, character_leg_top_object[0]);
	drawBoxBack(30, true, character_leg_top_object[1]);
	drawBoxLeft(30, true, character_leg_top_object[2]);
	drawBoxRight(30, true, character_leg_top_object[3]);
	drawBoxTop(30, true, character_leg_top_object[4]);
	drawBoxBottom(30, true, character_leg_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 왼쪽 무릎
	glTranslated(0, -45, 0);
	glRotatef(left_knee_x, 1, 0, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, character_leg_bottom_object[0]);
	drawBoxBack(30, true, character_leg_bottom_object[1]);
	drawBoxLeft(30, true, character_leg_bottom_object[2]);
	drawBoxRight(30, true, character_leg_bottom_object[3]);
	drawBoxTop(30, true, character_leg_top_object[4]);
	drawBoxBottom(30, true, character_leg_top_object[5]);
	glPopMatrix(); //왼쪽 무릎 종료

	glPopMatrix(); //왼쪽골반 종료
}

void drawZombie( Server_Player  server_data, int client_imei){
	glPushMatrix(); //Save 머리
	glTranslated(0, 75, 0);
	glRotatef(-(server_data.player[client_imei].camyrotate + 90) / 3, 1, 0, 0);
	glScaled(1.0, 1.0, 0.7);
	drawBoxFront(30, false, zombie_head_object[0]);
	drawBoxBack(30, false, zombie_head_object[1]);
	drawBoxLeft(30, false, zombie_head_object[2]);
	drawBoxRight(30, false, zombie_head_object[3]);
	drawBoxTop(30, false, zombie_head_object[4]);
	drawBoxBottom(30, false, zombie_head_object[5]);
	glPopMatrix(); //머리 end

	glPushMatrix(); //Save 몸통
	glScaled(1.0, 1.5, 0.5);
	drawBoxFront(30, false, zombie_body_object[0]);
	drawBoxBack(30, false, zombie_body_object[1]);
	drawBoxLeft(30, false, zombie_body_object[2]);
	drawBoxRight(30, false, zombie_body_object[3]);
	drawBoxTop(30, false, zombie_body_object[4]);
	drawBoxBottom(30, false, zombie_body_object[5]);
	glPopMatrix(); //몸통 end



	glPushMatrix(); //Save 오른팔 어께
	glTranslated(-45, 44, 0);
	glRotatef(-right_sholder_x + 210, 1, 0, 0);
	glRotatef(right_sholder_y, 0, 1, 0);
	glRotatef(right_sholder_z, 0, 0, 1);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_arm_top_object[0]);
	drawBoxBack(30, true, zombie_arm_top_object[1]);
	drawBoxLeft(30, true, zombie_arm_top_object[2]);
	drawBoxRight(30, true, zombie_arm_top_object[3]);
	drawBoxTop(30, true, zombie_arm_top_object[4]);
	drawBoxBottom(30, true, zombie_arm_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 오른팔 팔꿈치
	glTranslated(0, -45, 0);
	glRotatef(right_elbow_x, 1, 0, 0);
	glPushMatrix();
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_arm_bottom_object[0]);
	drawBoxBack(30, true, zombie_arm_bottom_object[1]);
	drawBoxLeft(30, true, zombie_arm_bottom_object[2]);
	drawBoxRight(30, true, zombie_arm_bottom_object[3]);
	drawBoxTop(30, true, zombie_arm_top_object[5]);
	drawBoxBottom(30, true, zombie_arm_top_object[5]);
	glPopMatrix();
	glPopMatrix();//오른팔 팔꿈치 종료
	glPopMatrix();//오른팔 어께 종료


	glPushMatrix(); //Save 왼팔 어께 
	glTranslated(45, 44, 0);
	glRotatef(-left_sholder_x - 90, 1, 0, 0);
	glRotatef(left_sholder_y, 0, 1, 0);
	glRotatef(left_sholder_z, 0, 0, 1);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_arm_top_object[0]);
	drawBoxBack(30, true, zombie_arm_top_object[1]);
	drawBoxLeft(30, true, zombie_arm_top_object[2]);
	drawBoxRight(30, true, zombie_arm_top_object[3]);
	drawBoxTop(30, true, zombie_arm_top_object[4]);
	drawBoxBottom(30, true, zombie_arm_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 왼팔 팔꿈치
	glTranslated(0, -45, 0);
	glRotatef(left_elbow_x, 1, 0, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_arm_bottom_object[0]);
	drawBoxBack(30, true, zombie_arm_bottom_object[1]);
	drawBoxLeft(30, true, zombie_arm_bottom_object[2]);
	drawBoxRight(30, true, zombie_arm_bottom_object[3]);
	drawBoxTop(30, true, zombie_arm_top_object[5]);
	drawBoxBottom(30, true, zombie_arm_top_object[5]);
	glPopMatrix();//왼팔 팔꿈치 종료
	glPopMatrix();//왼팔어께 종료



	glPushMatrix(); //Save 오른쪽 골반
	glTranslated(-15, -40, 0);
	glRotatef(right_leg_x, 1, 0, 0);
	glRotatef(right_leg_y, 0, 1, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_leg_top_object[0]);
	drawBoxBack(30, true, zombie_leg_top_object[1]);
	drawBoxLeft(30, true, zombie_leg_top_object[2]);
	drawBoxRight(30, true, zombie_leg_top_object[3]);
	drawBoxTop(30, true, zombie_leg_top_object[4]);
	drawBoxBottom(30, true, zombie_leg_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 오른쪽 무릎
	glTranslated(0, -45, 0);
	glRotatef(right_knee_x, 1, 0, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_leg_bottom_object[0]);
	drawBoxBack(30, true, zombie_leg_bottom_object[1]);
	drawBoxLeft(30, true, zombie_leg_bottom_object[2]);
	drawBoxRight(30, true, zombie_leg_bottom_object[3]);
	drawBoxTop(30, true, zombie_leg_top_object[4]);
	drawBoxBottom(30, true, zombie_leg_top_object[5]);
	glPopMatrix();//오른쪽 무릎 종료

	glPopMatrix();//오른쪽 골반 종료


	glPushMatrix(); //Save 왼쪽 골반
	glTranslated(15, -40, 0);
	glRotatef(left_leg_x, 1, 0, 0);
	glRotatef(left_leg_y, 0, 1, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_leg_top_object[0]);
	drawBoxBack(30, true, zombie_leg_top_object[1]);
	drawBoxLeft(30, true, zombie_leg_top_object[2]);
	drawBoxRight(30, true, zombie_leg_top_object[3]);
	drawBoxTop(30, true, zombie_leg_top_object[4]);
	drawBoxBottom(30, true, zombie_leg_top_object[5]);
	glScaled(2, 1.333333, 2);

	glPushMatrix(); //Save 왼쪽 무릎
	glTranslated(0, -45, 0);
	glRotatef(left_knee_x, 1, 0, 0);
	glScaled(0.5, 0.75, 0.5);
	drawBoxFront(30, true, zombie_leg_bottom_object[0]);
	drawBoxBack(30, true, zombie_leg_bottom_object[1]);
	drawBoxLeft(30, true, zombie_leg_bottom_object[2]);
	drawBoxRight(30, true, zombie_leg_bottom_object[3]);
	drawBoxTop(30, true, zombie_leg_top_object[4]);
	drawBoxBottom(30, true, zombie_leg_top_object[5]);
	glPopMatrix(); //왼쪽 무릎 종료

	glPopMatrix(); //왼쪽골반 종료
}

void animationChararm()
{
	switch (character_up_state)
	{
	case 2://권총
		//left_sholder_x = 0;
		left_sholder_y = 70;
		left_sholder_z = -75;
		//right_sholder_x = -60;
		right_sholder_y = 30;
		right_sholder_z = 0;
		left_elbow_x = 25;
		right_elbow_x = -30;
		break;
	case 1://라이플
		//left_sholder_x = 0;
		left_sholder_y = 70;
		left_sholder_z = -75;
		//right_sholder_x = -60;
		right_sholder_y = 30;
		right_sholder_z = 0;
		left_elbow_x = 25;
		right_elbow_x = -30;
		break;
	case 0://기본자세
		//left_sholder_x = -90;
		left_sholder_y = 0;
		left_sholder_z = 0;
		//right_sholder_x = 0;
		right_sholder_y = 0;
		right_sholder_z = 0;
		left_elbow_x = 0;
		right_elbow_x = 0;
		break;
	}
}

void animationCharleg()
{
	switch (character_down_state){
	case 0://가만히 서있을 때
		left_leg_x = 0;
		left_leg_y = 0;
		left_leg_z = 0;
		left_knee_x = 0;
		right_leg_x = 0;
		right_leg_y = 0;
		right_leg_z = 0;
		right_knee_x = 0;
		break;
	case 1://전진,후진
		left_leg_z = 0;
		right_leg_z = 0;
		if (legtimer < 150)
		{
			legtimer += 2;
			if (legtimer < 75)
			{
				left_leg_x = legtimer - 45;
				right_leg_x = 30 - legtimer;
				left_knee_x = 0;
				if (legtimer < 38)
					right_knee_x = legtimer;
				else
					right_knee_x = 38 - (legtimer - 38);
			}
			else
			{
				left_leg_x = 105 - legtimer;
				right_leg_x = legtimer - 120;
				right_knee_x = 0;

				if (legtimer < 113)
					left_knee_x = legtimer - 75;
				else
					left_knee_x = 38 - (legtimer - 113);
			}
		}
		else
			legtimer = 0;
		break;
	case 2://좌우이동
		break;
	case 3://앉기
		left_leg_x = -75;
		left_leg_y = 0;
		left_leg_z = 40;
		left_knee_x = 130;
		right_leg_x = -75;
		right_leg_y = 0;
		right_leg_z = -15;
		right_knee_x = 130;
		break;
	}
}

void init_Texture(){
	nomal_Texture(block_Nomal_object);
	tree_Texture(block_Tree_object);
	brick_Texture(block_Brick_object);
	wooden_Texture(block_Wooden_object);
	stone_Texture(block_Stone_object);

	head_Texture(character_head_object);
	body_Texture(character_body_object);
	arm_Texture(character_arm_top_object, character_arm_bottom_object);
	leg_Texture(character_leg_top_object, character_leg_bottom_object);

	zombie_head_Texture(zombie_head_object);
	zombie_body_Texture(zombie_body_object);
	zombie_arm_Texture(zombie_arm_top_object, zombie_arm_bottom_object);
	zombie_leg_Texture(zombie_leg_top_object, zombie_leg_bottom_object);

	wall_background_Texture(wall_background_object);
}

void Target(int x, int y)
{
	if (RotateCam)
	{
		nx = 400 - x;
		ny = 300 - y;

		player.camxrotate = player.camxrotate + (nx / MouseSens);
		player.camyrotate = player.camyrotate + (ny / MouseSens);
		left_sholder_x = player.camyrotate;
		right_sholder_x = player.camyrotate;

		Viewz = 10000 * sin((player.camyrotate)* 3.141592 / 180) * cos((player.camxrotate)* 3.141592 / 180);
		Viewx = 10000 * sin((player.camyrotate)* 3.141592 / 180) * sin((player.camxrotate)* 3.141592 / 180);
		Viewy = 10000 * cos((player.camyrotate)* 3.141592 / 180);

		Camz = -Camdistance * sin((player.camyrotate)* 3.141592 / 180) * cos((player.camxrotate + 15)* 3.141592 / 180);
		Camx = -Camdistance * sin((player.camyrotate)* 3.141592 / 180) * sin((player.camxrotate + 15)* 3.141592 / 180);
		Camy = -Camdistance * cos((player.camyrotate - 15)* 3.141592 / 180);


		if (player.camxrotate <= -360 || player.camxrotate >= 360)
			player.camxrotate = 0;

		if (player.camyrotate < -179)
			player.camyrotate = -179;
		else if (player.camyrotate > -1)
			player.camyrotate = -1;
		bx = nx;
		by = ny;

	}
}



void Keyinput(int key)
{
	if (key == 'w')
	{
		player.x += Charspeed * cos((-player.camxrotate - 90) * 3.141592 / 180);
		player.z += Charspeed * sin((-player.camxrotate - 90) * 3.141592 / 180);
		if (jump)
			character_down_state = 3;
		else
			character_down_state = 1;
	}
	else if (key == 's')
	{
		player.x -= Charspeed * cos((-player.camxrotate - 90) * 3.141592 / 180);
		player.z -= Charspeed * sin((-player.camxrotate - 90) * 3.141592 / 180);
		if (jump)
			character_down_state = 3;
		else
			character_down_state = 1;
	}
	if (key == 'a')
	{
		player.x -= Charspeed * cos((-player.camxrotate) * 3.141592 / 180);
		player.z -= Charspeed * sin((-player.camxrotate) * 3.141592 / 180);
		if (jump)
			character_down_state = 3;
		else
			character_down_state = 1;
	}
	else if (key == 'd')
	{
		player.x += Charspeed * cos((-player.camxrotate) * 3.141592 / 180);
		player.z += Charspeed * sin((-player.camxrotate) * 3.141592 / 180);
		if (jump)
			character_down_state = 3;
		else
			character_down_state = 1;
	}


	switch (key)
	{
	case '1':
		character_up_state = 1;
		break;
	case '2':
		character_up_state = 2;
		break;
	case '3':
		character_up_state = 0;
		break;
	case 'l':
		if (turn_Light == false){
			turn_Light = true;
		}
		else{
			turn_Light = false;
		}
		Keybuffer[key] = false;
		break;
	case 'c':
		glutSetCursor(GLUT_CURSOR_NONE);
		if (RotateCam)
			RotateCam = false;
		else
			RotateCam = true;
		Keybuffer[key] = false;
		break;
	case 'f':
		if (FirstPersonView){
			FirstPersonView = false;
			sight = false;
			turn_Light = true;
		}
		else{
			FirstPersonView = true;
			turn_Light = true;
		}
		Keybuffer[key] = false;
		break;
	case 'x':
		character_down_state = 3;
		Charspeed = 4;
		crouch = true;
		break;
	case '+':
		Camdistance--;
		if (Camdistance < 50)
			Camdistance = 50;
		break;
	case '-':
		Camdistance++;
		if (Camdistance > 1000)
			Camdistance = 1000;
		break;
	case '[':
		MouseSens++;
		if (MouseSens < 1)
			MouseSens = 1;
		printf("현재 마우스 감도는 %d 입니다.(default : 25)\n", MouseSens);
		Keybuffer[key] = false;
		break;
	case ']':
		MouseSens--;
		if (MouseSens > 100)
			MouseSens = 100;
		printf("현재 마우스 감도는 %d 입니다.(default : 25)\n", MouseSens);
		Keybuffer[key] = false;
		break;

	case 27:
		exit(0);
		break;
	}
}

void drawHud()
{
	glPushMatrix();

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);      // 조명 활성화
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_NORMALIZE);

	draw_Crosshair(sight);// 크로스 헤어 구현

	draw_MiniMap(player.x, player.z);// 미니맵 구현

	//draw_MiniMap( server_data.player[0].socket.x, server_data.player[0].socket.z );// 미니맵 구현

	glPushMatrix();//총알 표시
	glColor3f(0, 1, 0);
	glTranslatef(680, 570, 0);
	glRasterPos2f(0.0, 0.0);
	sprintf(ammo, "%d / %d", rifleload, rifleammo);
	int len = (int)strlen(ammo);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ammo[i]);
	glPopMatrix();

	glPushMatrix();//체력 표시
	glColor3f(1, 0, 0);
	glTranslatef(20, 570, 0);
	glRasterPos2f(0.0, 0.0);
	sprintf(health, "%d", hp);
	len = (int)strlen(ammo);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, health[i]);
	glPopMatrix();


	glPushMatrix();//FPS 표시
	glColor3f(1, 0, 0);
	glTranslatef(640, 25, 0);
	glRasterPos2f(0.0, 0.0);
	sprintf(draw_FPS, "FPS : %f", fps);
	len = (int)strlen(draw_FPS);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, draw_FPS[i]);
	glPopMatrix();

	glPopMatrix();
}

void drawPistol()
{
	glPushMatrix();
	if (sight){
		glTranslatef(0, 100, 0);
	}
	else{
		glTranslatef(0, 25, 0);
	}
	glRotatef(-right_sholder_x - 90, 1, 0, 0);
	if (sight){ glTranslatef(0, -15, 50); }
	else{ glTranslatef(-20, 10, 100); }

	glColor3f(0.1, 0.1, 0.1);

	glPushMatrix();//손잡이
	glTranslatef(0, -10, 0);
	glScalef(0.5, 1.0, 0.5);
	glRotatef(15, 1, 0, 0);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();//총아래부분
	glTranslatef(0, 2, +15);
	glScalef(0.25, 0.1, 1.0);
	glutSolidCube(40);
	glPopMatrix();

	glPushMatrix();//총윗부분
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0, 8, +15);
	glScalef(0.25, 0.2, 1.0);
	glutSolidCube(40);
	glPopMatrix();

	glPushMatrix();//가늠좌
	glColor3f(0.0, 0.8, 0.0);
	glTranslatef(3, 14, 0);
	glutSolidCube(2);
	glTranslatef(-6, 0, 0);
	glutSolidCube(2);
	glPopMatrix();

	if (sight){}
	else{
		glPushMatrix();//가늠쇠
		glColor3f(0.0, 0.0, 1.0);
		glTranslatef(0, 14, 35);
		glutSolidCube(1);
		glPopMatrix();
	}

	glPopMatrix();
}

void drawBullet()
{
	if (Head != NULL)
	{
		Temp = Head;
		while (Temp != NULL)
		{
			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(Temp->x, Temp->y, Temp->z);
			if (FirstPersonView)
				glutSolidSphere(2, 6, 6);
			else
				glutSolidSphere(10, 6, 6);
			glPopMatrix();
			Temp = Temp->next;
		}
	}
}

void crashCheck()
{
	int X = (player.x / 120);
	int Y = (player.y / 120) + 1;
	int Z = (player.z / 120);


	if (player.x < 0)
		X -= 1;
	if (player.z < 0)
		Z -= 1;
	if (player.y < 0)
	{
		player.y = 0;
		gravity = 0;
		jump = false;
	}

	for (int y = Y; y < Y + 2; y++)
	{
		for (int z = Z - 1; z < Z + 2; z++)
		{
			for (int x = X - 1; x < X + 2; x++)
			{
				if (map_DATA[Y - 1][X][Z] == 1)
				{
				}
				if (map_DATA[y][x + 36][z + 13] == 1)
				{
					if ((x == X) && (z == Z))
					{
						if (player.x < x * 120 + 60)
							player.x = x * 120;
						else if (player.x > x * 120 + 60)
							player.x = x * 120 + 120;
						else if (player.z < z * 120 + 60)
							player.z = z * 120;
						else if (player.z > z * 120 + 60)
							player.z = z * 120 + 120;
					}
					else if ((x == X - 1) && (z == Z - 1))
					{
						if ((player.x < ((x * 120) + 120 + crashdist)) && (player.z < ((z * 120) + 120 + crashdist)))
						{
							player.x = ((x * 120) + 120 + crashdist);
							player.z = ((z * 120) + 120 + crashdist);
						}
					}
					else if ((x == X - 1) && (z == Z + 1))
					{
						if ((player.x < ((x * 120) + 120 + crashdist)) && (((z * 120) - crashdist) < player.z))
						{
							player.x = ((x * 120) + 120 + crashdist);
							player.z = ((z * 120) - crashdist);
						}
					}
					else if ((x == X + 1) && (z == Z - 1))
					{
						if ((((x * 120) - crashdist) < player.x) && (player.z < ((z * 120) + 120 + crashdist)))
						{
							player.x = ((x * 120) - crashdist);
							player.z = ((z * 120) + 120 + crashdist);
						}
					}
					else if ((x == X + 1) && (z == Z + 1))
					{
						if ((((x * 120) - crashdist) < player.x) && (((z * 120) - crashdist) < player.z))
						{
							player.x = ((x * 120) - crashdist);
							player.z = ((z * 120) - crashdist);
						}
					}
					else if (x == X - 1)
					{
						if (player.x < ((x * 120) + 120 + crashdist))
							player.x = ((x * 120) + 120 + crashdist);
					}
					else if (X + 1 == x)
					{
						if (((x * 120) - crashdist) < player.x)
							player.x = ((x * 120) - crashdist);
					}
					else if (z == Z - 1)
					{
						if (player.z < ((z * 120) + 120 + crashdist))
							player.z = ((z * 120) + 120 + crashdist);
					}
					else if (Z + 1 == z)
					{
						if (((z * 120) - crashdist) < player.z)
							player.z = ((z * 120) - crashdist);
					}
				}//end of if
			}//end of inner for
		}//end of outer for
	}//end of y for
}

void bulletCreate()
{
	//printf("1");
	Bullet * New = (Bullet*)malloc(sizeof(Bullet));
	if (FirstPersonView)
	{
		if (sight)
		{
			New->x = player.x + 50 * sin((player.camyrotate)* 3.141592 / 180) * sin((player.camxrotate)* 3.141592 / 180);
			New->y = player.y + 50 * cos((player.camyrotate)* 3.141592 / 180) + 165;
			New->z = player.z + 50 * sin((player.camyrotate)* 3.141592 / 180) * cos((player.camxrotate)* 3.141592 / 180);
		}
		else
		{
			New->x = player.x + 30 * sin((player.camyrotate - 20)* 3.141592 / 180) * sin((player.camxrotate - 10)* 3.141592 / 180);
			New->y = player.y + 30 * cos((player.camyrotate - 20)* 3.141592 / 180) + 170;
			New->z = player.z + 30 * sin((player.camyrotate - 20)* 3.141592 / 180) * cos((player.camxrotate - 10)* 3.141592 / 180);
		}
	}
	else
	{
		New->x = player.x + 140 * sin((player.camyrotate - 20)* 3.141592 / 180) * sin((player.camxrotate - 10)* 3.141592 / 180);
		New->y = player.y + 140 * cos((player.camyrotate - 20)* 3.141592 / 180) + 170;
		New->z = player.z + 140 * sin((player.camyrotate - 20)* 3.141592 / 180) * cos((player.camxrotate - 10)* 3.141592 / 180);
	}
	New->fz = 30 * sin((player.camyrotate)* 3.141592 / 180) * cos((player.camxrotate)* 3.141592 / 180);
	New->fx = 30 * sin((player.camyrotate)* 3.141592 / 180) * sin((player.camxrotate)* 3.141592 / 180);
	New->fy = 30 * cos((player.camyrotate)* 3.141592 / 180);
	New->next = NULL;
	//printf("%f, %f, %f\n", New->fx, New->fy, New->fz);
	//printf("Viewx : %f, Viewy : %f, Viewz : %f\n", Viewx, Viewy, Viewz);

	if (Head == NULL)
		Head = New;
	else
	{
		//printf("create\n");
		CTemp = Head;
		while (CTemp != NULL)
		{
			if (CTemp->next != NULL)
				CTemp = CTemp->next;
			else
			{
				CTemp->next = New;
				break;
			}
		}
		//printf("createend\n");
	}
}

void bulletDestroy(Bullet * Del)
{
	DTemp = Head;
	//Bullet * Target;
	//printf("dest\n");
	while (DTemp != NULL)
	{
		if (Head == Del)
		{
			Head = Del->next;
			DTemp = Head;
			free(Del);
			break;
		}
		else if (DTemp->next == Del)
		{
			Del = DTemp->next;
			DTemp->next = Del->next;
			free(Del);
			break;
		}

		else
			DTemp = DTemp->next;
	}
	//printf("destend\n");
}

void update_FPS(){
	static DWORD frameCount = 0;            //프레임 카운트수
	static float timeElapsed = 0.0f;            //흐른 시간
	static DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

	DWORD curTime = timeGetTime();      //현재 시간
	float timeDelta = (curTime - lastTime)*0.001f;        //timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.

	timeElapsed += timeDelta;

	frameCount++;
	

	//printf( "Camxrotate : %f\nCamyrotate : %f\nPlayer->X : %f\nPlayer->Y : %f\nPlayer->Z : %f\n", server_data[0].camxrotate,server_data[0].camyrotate,server_data[0].socket.x,server_data[0].socket.y,server_data[0].socket.z );

	if (timeElapsed >= 1.0f)         //흐른시간이 1초이상이면 내가 하고싶은것 처리
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