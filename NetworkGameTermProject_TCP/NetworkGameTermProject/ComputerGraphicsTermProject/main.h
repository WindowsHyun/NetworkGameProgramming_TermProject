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
#include "resource.h" // ���ҽ� ��� �߰�

#include "OpenGL_Aux/GLAUX.H"
// OpenGL Aux ������� ���� ��� �߰�
#pragma comment(lib, "./OpenGL_Aux/GLAUX")
// OpenGL Aux ������� ���� ��� �߰� ����
#pragma comment(lib, "winmm.lib") 
// ��������� �����ϱ� ���� ��� �߰�.


//�������
#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 4


// Ŭ���̾�Ʈ ������ ũ��
#define Window_Width 800
#define Window_Height 600

//��ü �ȵ���
#define CHARACTER_NOWEAPON 0
#define CHARACTER_PISTOL 1
//#define CHARACTER_RIFLE 2

//��ü ������
#define CHARACTER_STAND 0
#define CHARACTER_WALK 1
#define CHARACTER_CROUCH_JUMP 3

typedef struct Player_Animation
{
	int character_up_state = CHARACTER_PISTOL, legtimer = 0;//up_state�� ��ü, down_state�� ��ü
	float head_angle_x = 0;//�Ӹ�ȸ��
	float left_leg_x = 0, left_leg_y = 0, left_knee_x = 0, right_leg_x = 0, right_leg_y = 0, right_knee_x = 0, left_leg_z = 0, right_leg_z = 0;//�ٸ�ȸ��
	bool sight = false, jump = false, crouch = false;//������, ��������, �ɱ����
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




//UI �׸���
void drawHud( int w, int h ); // ������ ũ�⸦ �޾ƿ��� �Ѵ�.

//��������
void err_quit( char *msg );
void err_display( char *msg );
SOCKET init_sock();
int get_ClientID( SOCKET sock );
void send_Player( SOCKET sock, Player_Socket player );
Server_Player recv_Player( SOCKET sock );
int get_ClientTeam( SOCKET sock );

//�� �׸���
void DrawMap();

//�ִϸ��̼� ����
void PlayAnimation(Player_Animation &Ani, const int character_down_state);

//�ڽ��׸���
void drawBoxFront( int, bool, GLuint );//�ڽ� �׸��� �Լ�
void drawBoxBack( int, bool, GLuint );
void drawBoxTop( int, bool, GLuint );
void drawBoxBottom( int, bool, GLuint );
void drawBoxRight( int, bool, GLuint );
void drawBoxLeft( int, bool, GLuint );

//�ٴ� �׸��� ����
void init_Map_Texture();
static GLuint map_object[1];

//ĳ���� �׸��� ����
static GLuint character_head_object[6];
static GLuint character_body_object[6];
static GLuint character_arm_top_object[6];
static GLuint character_arm_bottom_object[6];
static GLuint character_leg_top_object[6];
static GLuint character_leg_bottom_object[6];

//���� �׸��� ����
static GLuint zombie_head_object[6];
static GLuint zombie_body_object[6];
static GLuint zombie_arm_top_object[6];
static GLuint zombie_arm_bottom_object[6];
static GLuint zombie_leg_top_object[6];
static GLuint zombie_leg_bottom_object[6];

//�ؽ��� �ҷ�����
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


//BMP���� �ҷ�����
void Load_TextureBMP( GLuint object[], int num, char *imagedata );
AUX_RGBImageRec *LoadBMP( char *Filename );

// ���̾�α� �ڽ� �ҷ����� ���� �Լ�
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern char ipAddres[MAX_PATH];
extern char nickName[16]; // Ŭ�� �г��� �ִ� 10
extern bool GamePlayWait; // True : ������ ���� ����, False : ������ �����Ҽ� ����

// FPS ������ ���� ����
extern float fps;
extern int sendPacket;
void update_FPS(); // FPS�� ���������� �����Ѵ�

#endif
