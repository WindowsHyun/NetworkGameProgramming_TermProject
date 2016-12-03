#ifndef __MAIN_LOAD_H
#define __MAIN_LOAD_H

#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <GL/glut.h> // include glut.h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>
#include <math.h>
#include "resource.h" // 리소스 헤더 추가

#include "OpenGL_Aux/GLAUX.H"
// OpenGL Aux 사용으로 인한 모듈 추가
#pragma comment(lib, "./OpenGL_Aux/GLAUX")
// OpenGL Aux 사용으로 인한 모듈 추가 종료
#pragma comment(lib, "winmm.lib") 
// 배경음악을 구현하기 위한 모듈 추가.


//서버통신
#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 4


// 클라이언트 윈도우 크기
#define Window_Width 800
#define Window_Height 600

//상체 팔동작
#define CHARACTER_NOWEAPON 0
#define CHARACTER_PISTOL 1
//#define CHARACTER_RIFLE 2

//하체 움직임
#define CHARACTER_STAND 0
#define CHARACTER_WALK 1
#define CHARACTER_CROUCH_JUMP 3

typedef struct Player_Animation
{
	int character_up_state = CHARACTER_PISTOL, legtimer = 0;//up_state는 상체, down_state는 하체
	float head_angle_x = 0;//머리회전
	float left_leg_x = 0, left_leg_y = 0, left_knee_x = 0, right_leg_x = 0, right_leg_y = 0, right_knee_x = 0, left_leg_z = 0, right_leg_z = 0;//다리회전
	bool sight = false, jump = false, crouch = false;//정조준, 점프상태, 앉기상태
}Player_Animation;

typedef struct Bullet
{
	float x, y, z;
	float fx, fy, fz;
	Bullet * next;
}Bullet;

typedef struct Player_Socket
{
	float x, y, z;
	float camxrotate, camyrotate;
	bool live;
	bool team;
	char nickName[16];
	int character_down_state;
}Player_Socket;

typedef struct Server_Player
{
	Player_Socket Players[MAX_Client];
}Player_Other;

extern Server_Player server_data;

typedef struct Player
{
	float Viewx, Viewy, Viewz;
	float Camx, Camy, Camz;
	Player_Animation Ani;
	int character_down_state;
}Player;




//UI 그리기
void drawHud( int w, int h ); // 윈도우 크기를 받아오게 한다.

//서버관련
void err_quit( char *msg );
void err_display( char *msg );
SOCKET init_sock();
int get_ClientID( SOCKET sock );
void send_Player( SOCKET sock, Player_Socket player );
Server_Player recv_Player( SOCKET sock );
int get_ClientTeam( SOCKET sock );

//맵 그리기
void DrawMap();

//애니메이션 동작
void PlayAnimation(Player_Animation &Ani, const int character_down_state);

//박스그리기
void drawBoxFront( int, bool, GLuint );//박스 그리는 함수
void drawBoxBack( int, bool, GLuint );
void drawBoxTop( int, bool, GLuint );
void drawBoxBottom( int, bool, GLuint );
void drawBoxRight( int, bool, GLuint );
void drawBoxLeft( int, bool, GLuint );

//바닥 그리기 관련
void init_Map_Texture();
static GLuint map_object[1];

//캐릭터 그리기 관련
static GLuint character_head_object[6];
static GLuint character_body_object[6];
static GLuint character_arm_top_object[6];
static GLuint character_arm_bottom_object[6];
static GLuint character_leg_top_object[6];
static GLuint character_leg_bottom_object[6];

//좀비 그리기 관련
static GLuint zombie_head_object[6];
static GLuint zombie_body_object[6];
static GLuint zombie_arm_top_object[6];
static GLuint zombie_arm_bottom_object[6];
static GLuint zombie_leg_top_object[6];
static GLuint zombie_leg_bottom_object[6];

//텍스쳐 불러오기
void init_Character_Texture();
void head_Texture( GLuint object[] );
void body_Texture( GLuint object[] );
void arm_Texture( GLuint object[], GLuint object2[] );
void leg_Texture( GLuint object[], GLuint object2[] );

void init_Zombie_Texture();
void zombie_head_Texture( GLuint[] );
void zombie_body_Texture( GLuint[] );
void zombie_arm_Texture( GLuint[], GLuint[] );
void zombie_leg_Texture( GLuint[], GLuint[] );

void drawCharacter(Player_Socket *player_socket, Player_Animation *Ani);
void drawZombie(Player_Socket *player_socket, Player_Animation *Ani);


//BMP파일 불러오기
void Load_TextureBMP( GLuint object[], int num, char *imagedata );
AUX_RGBImageRec *LoadBMP( char *Filename );

// 다이얼로그 박스 불러오기 관련 함수
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern char ipAddres[MAX_PATH];
extern char nickName[16]; // 클라 닉네임 최대 10
extern bool GamePlayWait; // True : 게임을 진행 못함, False : 게임을 진행할수 있음

// FPS 측정을 위한 변수
extern float fps;
extern int sendPacket;
void update_FPS(); // FPS를 지속적으로 측정한다

#endif
