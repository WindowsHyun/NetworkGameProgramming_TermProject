#pragma once

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 4

typedef struct Player_Socket {
	// ������ ���� ���
	float x = -1000, y = -1000, z = -1000;
	float camxrotate = -1000, camyrotate = -1000;
	bool live = false;
	bool team = false; // ���� 1�� 2���� T/F�� ��������
	char nickName[16];
	int character_down_state;
	/*
	���ۻ���  : �ȱ�, ����, ����

	*/
}Player_Socket;

typedef struct Player {
	//Ŭ�� �������� ���
	Player_Socket socket;
}Player;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
	Player_Socket player[MAX_Client];
}Server_Player;