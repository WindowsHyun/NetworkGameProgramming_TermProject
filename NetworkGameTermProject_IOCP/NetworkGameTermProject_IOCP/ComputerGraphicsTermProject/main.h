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
#define MAX_Client 8

// IOCP 관련 define
#define Recv_Mode 0
#define Send_Mode 1
#define Not_Attacked 19930616
#define Respawn_Complete 19940106
#define Not_Respwan 1234
#define No_Connected FALSE

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

//박스갯수
#define BOX_COUNT 38


//3차원 벡터 구조체
typedef struct vector3 {
	double x, y, z;
}vector3;

//2차원 벡터 구조체
typedef struct vector2 {
	double x, y;
	//생성자
	vector2() {}
	vector2( double _x, double _y ) {
		x = _x, y = _y;
	}
	//외적
	double cross( const vector2& other ) const {
		return x*other.y - y*other.x;
	}

	/* 연산자 오버로딩을 통해 실제 벡터의 연산을 구현합니다. */

	//벡터의 실수배
	vector2 operator * ( double r ) const {
		return vector2( x*r, y*r );
	}
	//벡터의 덧셈
	vector2 operator + ( vector2 other ) const {
		return vector2( x + other.x, y + other.y );
	}
	//벡터의 뺄셈
	vector2 operator - ( vector2 other ) const {
		return vector2( x - other.x, y - other.y );
	}
	//두 벡터의 비교
	bool operator == ( vector2 other ) const {
		return x == other.x && y == other.y;
	}
	bool operator < ( vector2 other ) const {
		return x < other.x && y < other.y;
	}
}vector2;

//장애물 박스 정보 구조체
typedef struct Box
{
	//opengl
	int x, y, z;//박스위치
	int size; //박스크기
	int image;
}Box;

//플레이어 애니메이션 구조체
//어떤 자세인지, 팔다리 등의 회전상태값을 저장
typedef struct Player_Animation
{
	int character_up_state = CHARACTER_PISTOL, legtimer = 0;//up_state는 상체, down_state는 하체
	float head_angle_x = 0;//머리회전
	float left_leg_x = 0, left_leg_y = 0, left_knee_x = 0, right_leg_x = 0, right_leg_y = 0, right_knee_x = 0, left_leg_z = 0, right_leg_z = 0;//다리회전
	bool sight = false, jump = false, crouch = false;//정조준, 점프상태, 앉기상태
}Player_Animation;

//외부로 전송하기위한 플레이어 정보를 담은 구조체
typedef struct Player_Socket
{
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = 90.0f, camyrotate = 90.0f;
	int hp = 100;
	int AttackedPlayer = Not_Attacked; // 공격 당한 플레이어 번호
	int RespawnTime = Not_Respwan;
	bool live = true;
	bool Game_Play = false;
	bool team;
	char nickName[16];
	int character_down_state;
	int kill = 0, death = 0;//스코어정보저장
}Player_Socket;

typedef struct Game_Round
{
	int GameRound;
	int round_time;
	bool round_start = false;
	int time_wait;
	bool round_wait = false; // 전체 게임 시작전 bool
	bool exit_Round;
	int game_result;
}Game_Round;

//서버로 데이터를 전송및 수신할때 사용하는 구조체
typedef struct Server_Player
{
	Player_Socket Players[MAX_Client];
	Game_Round gr;
}Server_Player;
//공용 전역변수 서버데이터
//다른cpp에서 중복 선언 안되도록 extern으로 선언
extern Server_Player server_data;

//클라이언트 에서만 사용하는 캐릭터 정보 구조체
typedef struct Player
{
	float Viewx, Viewy, Viewz;
	float Camx, Camy, Camz;
	Player_Animation Ani;
	int character_down_state;
	vector3 RayPos, RayDir;
}Player;

//-------------------------------------------------------------------------------------
// IOCP 관련 소켓 정보 구조체
struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsabuf;
};

// 클라이언트 고유번호, 팀번호 구조체
typedef struct ClientData {
	int client_imei = -1;
	bool team = -1;
}ClientData;

extern ClientData client_data;
//-------------------------------------------------------------------------------------

//벽충돌
void WallCollision( Player_Socket *player_socket );

//UI 그리기
void drawHud(int w, int h, Player_Socket player, int time);// 윈도우 크기를 받아오게 한다.
void draw_score(int w, int h, Server_Player server_data);

//무기그리기 + 총구화염
void drawGun( Player_Socket *player_socket, bool &gunfire );

//서버관련
SOCKET init_sock();
void iocp_Communication( SOCKET sock, Player_Socket &player, ClientData &cd, Server_Player &server_data );

//맵 그리기
//void DrawMap();
//void DrawMap( Box *box );
void DrawMap( );
void DrawBox( Box* box );


//애니메이션 동작
void PlayAnimation( Player_Animation &Ani, const int character_down_state );

//박스그리기
void drawBoxFront( int, bool, GLuint );//박스 그리는 함수
void drawBoxBack( int, bool, GLuint );
void drawBoxTop( int, bool, GLuint );
void drawBoxBottom( int, bool, GLuint );
void drawBoxRight( int, bool, GLuint );
void drawBoxLeft( int, bool, GLuint );

//바닥 그리기 관련
void init_Map_Texture();
void Mapping_Box( Box &box, int x, int y, int z , int img_num );
static GLuint map_object[1];
static GLuint stone_object[4];

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

//캐릭터 그리기
void drawCharacter( Player_Socket *player_socket, Player_Animation *Ani, bool &gunfire );
void drawZombie( Player_Socket *player_socket, Player_Animation *Ani );


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

//총알 충돌체크 관련 함수
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

//VS2015버전에서 오류날 시 legacy_stdio_definitions.lib  추가 (프로젝트 옵션 -> 링커 -> 입력)
#endif