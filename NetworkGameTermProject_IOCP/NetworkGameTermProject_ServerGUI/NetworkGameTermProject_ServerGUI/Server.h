#pragma once

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 8

// IOCP 관련 모드 설정
#define Recv_Mode 0
#define Send_Mode 1
#define Not_Attacked 19930616
#define Respawn_Complete 19940106
#define Not_Respwan 1234
#define Game_WaitTime 10
#define No_Connected FALSE

typedef struct Player_Socket {
	// 전송을 위해 사용
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	int hp = 100;
	int AttackedPlayer = Not_Attacked; // 공격 당한 플레이어 번호
	int RespawnTime = Not_Respwan;
	bool live = false;
	bool Game_Play = false;
	bool team = false; // 팀을 1팀 2팀을 T/F로 나눠버림
	char nickName[16];
	int character_down_state;
	int kill = 0, death = 0;//스코어정보저장
}Player_Socket;

typedef struct Player {
	//클라 내에서만 사용
	Player_Socket socket;
}Player;

typedef struct Game_Round
{
	int GameRound = 1; // 게임 라운드
	int round_time = 600; // 게임 시간
	bool round_start = false; // 게임 시작 or 정지
	int time_wait = Game_WaitTime; // 전체 게임 시작전 대기
	bool round_wait = false; // 전체 게임 시작전 bool
	bool exit_Round = false; // 라운드를 강제로 종료하기
	int game_result = 3; // 게임 승패 여부를 보내줌
}Game_Round;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	Player_Socket player[MAX_Client];
	Game_Round gr;
}Server_Player;


// IOCP 관련 구조체
typedef struct {
	//소켓정보를구조체화.
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
	int client_imei = 0;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {
	// 소켓의버퍼정보를구조체화.
	OVERLAPPED overlapped;
	char buffer[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

typedef struct ClientData {
	int client_imei;
	bool team;
}ClientData;