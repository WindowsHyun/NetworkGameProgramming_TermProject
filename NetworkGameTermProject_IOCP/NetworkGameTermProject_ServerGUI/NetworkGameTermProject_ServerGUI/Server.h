#pragma once

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 8

// IOCP ���� ��� ����
#define Recv_Mode 0
#define Send_Mode 1
#define Not_Attacked 19930616
#define Respawn_Complete 19940106
#define Not_Respwan 1234
#define Game_WaitTime 10
#define No_Connected FALSE

typedef struct Player_Socket {
	// ������ ���� ���
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	int hp = 100;
	int AttackedPlayer = Not_Attacked; // ���� ���� �÷��̾� ��ȣ
	int RespawnTime = Not_Respwan;
	bool live = false;
	bool Game_Play = false;
	bool team = false; // ���� 1�� 2���� T/F�� ��������
	char nickName[16];
	int character_down_state;
	int kill = 0, death = 0;//���ھ���������
}Player_Socket;

typedef struct Player {
	//Ŭ�� �������� ���
	Player_Socket socket;
}Player;

typedef struct Game_Round
{
	int GameRound = 1; // ���� ����
	int round_time = 600; // ���� �ð�
	bool round_start = false; // ���� ���� or ����
	int time_wait = Game_WaitTime; // ��ü ���� ������ ���
	bool round_wait = false; // ��ü ���� ������ bool
	bool exit_Round = false; // ���带 ������ �����ϱ�
	int game_result = 3; // ���� ���� ���θ� ������
}Game_Round;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
	Player_Socket player[MAX_Client];
	Game_Round gr;
}Server_Player;


// IOCP ���� ����ü
typedef struct {
	//��������������üȭ.
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
	int client_imei = 0;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct {
	// �����ǹ�������������üȭ.
	OVERLAPPED overlapped;
	char buffer[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

typedef struct ClientData {
	int client_imei;
	bool team;
}ClientData;