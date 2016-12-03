#pragma once

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 8

// IOCP ���� ��� ����
#define Recv_Mode 0
#define Send_Mode 1

typedef struct Player_Socket {
	// ������ ���� ���
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	bool live = false;
	bool team = false; // ���� 1�� 2���� T/F�� ��������
	char nickName[16];
	int character_down_state;
	bool ClientData = false;
}Player_Socket;

typedef struct Player {
	//Ŭ�� �������� ���
	Player_Socket socket;
}Player;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
	Player_Socket player[MAX_Client];
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