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
#define MAX_Client 8

// IOCP ���� define
#define Recv_Mode 0
#define Send_Mode 1
#define Not_Attacked 19930616
#define Respawn_Complete 19940106
#define Not_Respwan 1234
#define No_Connected FALSE

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

//�ڽ�����
#define BOX_COUNT 38


//3���� ���� ����ü
typedef struct vector3 {
	double x, y, z;
}vector3;

//2���� ���� ����ü
typedef struct vector2 {
	double x, y;
	//������
	vector2() {}
	vector2( double _x, double _y ) {
		x = _x, y = _y;
	}
	//����
	double cross( const vector2& other ) const {
		return x*other.y - y*other.x;
	}

	/* ������ �����ε��� ���� ���� ������ ������ �����մϴ�. */

	//������ �Ǽ���
	vector2 operator * ( double r ) const {
		return vector2( x*r, y*r );
	}
	//������ ����
	vector2 operator + ( vector2 other ) const {
		return vector2( x + other.x, y + other.y );
	}
	//������ ����
	vector2 operator - ( vector2 other ) const {
		return vector2( x - other.x, y - other.y );
	}
	//�� ������ ��
	bool operator == ( vector2 other ) const {
		return x == other.x && y == other.y;
	}
	bool operator < ( vector2 other ) const {
		return x < other.x && y < other.y;
	}
}vector2;

//��ֹ� �ڽ� ���� ����ü
typedef struct Box
{
	//opengl
	int x, y, z;//�ڽ���ġ
	int size; //�ڽ�ũ��
	int image;
}Box;

//�÷��̾� �ִϸ��̼� ����ü
//� �ڼ�����, �ȴٸ� ���� ȸ�����°��� ����
typedef struct Player_Animation
{
	int character_up_state = CHARACTER_PISTOL, legtimer = 0;//up_state�� ��ü, down_state�� ��ü
	float head_angle_x = 0;//�Ӹ�ȸ��
	float left_leg_x = 0, left_leg_y = 0, left_knee_x = 0, right_leg_x = 0, right_leg_y = 0, right_knee_x = 0, left_leg_z = 0, right_leg_z = 0;//�ٸ�ȸ��
	bool sight = false, jump = false, crouch = false;//������, ��������, �ɱ����
}Player_Animation;

//�ܺη� �����ϱ����� �÷��̾� ������ ���� ����ü
typedef struct Player_Socket
{
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = 90.0f, camyrotate = 90.0f;
	int hp = 100;
	int AttackedPlayer = Not_Attacked; // ���� ���� �÷��̾� ��ȣ
	int RespawnTime = Not_Respwan;
	bool live = true;
	bool Game_Play = false;
	bool team;
	char nickName[16];
	int character_down_state;
	int kill = 0, death = 0;//���ھ���������
}Player_Socket;

typedef struct Game_Round
{
	int GameRound;
	int round_time;
	bool round_start = false;
	int time_wait;
	bool round_wait = false; // ��ü ���� ������ bool
	bool exit_Round;
	int game_result;
}Game_Round;

//������ �����͸� ���۹� �����Ҷ� ����ϴ� ����ü
typedef struct Server_Player
{
	Player_Socket Players[MAX_Client];
	Game_Round gr;
}Server_Player;
//���� �������� ����������
//�ٸ�cpp���� �ߺ� ���� �ȵǵ��� extern���� ����
extern Server_Player server_data;

//Ŭ���̾�Ʈ ������ ����ϴ� ĳ���� ���� ����ü
typedef struct Player
{
	float Viewx, Viewy, Viewz;
	float Camx, Camy, Camz;
	Player_Animation Ani;
	int character_down_state;
	vector3 RayPos, RayDir;
}Player;

//-------------------------------------------------------------------------------------
// IOCP ���� ���� ���� ����ü
struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsabuf;
};

// Ŭ���̾�Ʈ ������ȣ, ����ȣ ����ü
typedef struct ClientData {
	int client_imei = -1;
	bool team = -1;
}ClientData;

extern ClientData client_data;
//-------------------------------------------------------------------------------------

//���浹
void WallCollision( Player_Socket *player_socket );

//UI �׸���
void drawHud(int w, int h, Player_Socket player, int time);// ������ ũ�⸦ �޾ƿ��� �Ѵ�.
void draw_score(int w, int h, Server_Player server_data);

//����׸��� + �ѱ�ȭ��
void drawGun( Player_Socket *player_socket, bool &gunfire );

//��������
SOCKET init_sock();
void iocp_Communication( SOCKET sock, Player_Socket &player, ClientData &cd, Server_Player &server_data );

//�� �׸���
//void DrawMap();
//void DrawMap( Box *box );
void DrawMap( );
void DrawBox( Box* box );


//�ִϸ��̼� ����
void PlayAnimation( Player_Animation &Ani, const int character_down_state );

//�ڽ��׸���
void drawBoxFront( int, bool, GLuint );//�ڽ� �׸��� �Լ�
void drawBoxBack( int, bool, GLuint );
void drawBoxTop( int, bool, GLuint );
void drawBoxBottom( int, bool, GLuint );
void drawBoxRight( int, bool, GLuint );
void drawBoxLeft( int, bool, GLuint );

//�ٴ� �׸��� ����
void init_Map_Texture();
void Mapping_Box( Box &box, int x, int y, int z , int img_num );
static GLuint map_object[1];
static GLuint stone_object[4];

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

//ĳ���� �׸���
void drawCharacter( Player_Socket *player_socket, Player_Animation *Ani, bool &gunfire );
void drawZombie( Player_Socket *player_socket, Player_Animation *Ani );


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

//�Ѿ� �浹üũ ���� �Լ�
double ccw( vector2 a, vector2 b );
double ccw( vector2 p, vector2 a, vector2 b );
void swap( vector2 &a, vector2 &b );
bool paralleSegments( vector2 a, vector2 b, vector2 c, vector2 d, vector2& p );
bool inBoundingRectangle( vector2 p, vector2 a, vector2 b );
bool lineIntersection( vector2 a, vector2 b, vector2 c, vector2 d, vector2& x );
bool segmentIntersection( vector2 a, vector2 b, vector2 c, vector2 d, vector2& p );
bool sementIntersects( vector2 a, vector2 b, vector2 c, vector2 d );
void BulletCollision( Box box[] );
double Bullet( Box *box );
double PlaneCollision( Box *box, const vector3 *Pos, const vector3 *Dir );

//VS2015�������� ������ �� legacy_stdio_definitions.lib  �߰� (������Ʈ �ɼ� -> ��Ŀ -> �Է�)
#endif