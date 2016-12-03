#pragma once

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 4

typedef struct Player_Socket {
	// 전송을 위해 사용
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	bool live = false;
	bool team = false; // 팀을 1팀 2팀을 T/F로 나눠버림
	char nickName[16];
	int character_down_state;
	/*
	동작상태  : 걷기, 정지, 점프

	*/
}Player_Socket;

typedef struct Player {
	//클라 내에서만 사용
	Player_Socket socket;
}Player;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	Player_Socket player[MAX_Client];
}Server_Player;