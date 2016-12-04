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
#define No_Connected FALSE

typedef struct Player_Socket {
	// 전송을 위해 사용
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	int hp = 100;
	int AttackedPlayer = Not_Attacked; // 공격 당한 플레이어 번호
	int RespawnTime = Not_Respwan;
	bool live = false;
	bool team = false; // 팀을 1팀 2팀을 T/F로 나눠버림
	char nickName[16];
	int character_down_state;
}Player_Socket;

typedef struct Player {
	//클라 내에서만 사용
	Player_Socket socket;
}Player;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	Player_Socket player[MAX_Client];
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