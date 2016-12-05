#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdlib.h>
#include <atlstr.h> // cstring
#include <Windows.h>
#include <process.h> // ���μ��� ������ �����´�.
#include "resource.h"
#include "Server.h"

bool client[MAX_Client] = { false }; // Ŭ���̾�Ʈ�� ������ ��� ó���Ѵ�.
bool init_client[MAX_Client] = { false }; // Ŭ�� ó�� �����ϴ��� ���ϴ����� �Ǵ��Ѵ�.
char buf[MAX_PATH];
int aTeam = 0, bTeam = 0;
int defaultRoundTime = 60;

// ������ ���ν���
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
void CALLBACK TimerProc( HWND, UINT, UINT, DWORD ); //�Լ� ����


void DisplayText( HWND hDlg, char *fmt, ... );
void DisplayEditText( HWND hDlg, char *fmt, ... );
void clrUser( int ); // ������ �����ų� ����� ������ �����.
void Kill_Death( int select ); // kd�ʱ�ȭ or ���

SOCKET sock; // ����
HWND shareHwnd, hList, hTextBox[3];
HFONT hFont[3];
HWND hPlayer[8], hPlayerOff[8];
HWND Time_Button[6];
SYSTEM_INFO SystemInfo;

Server_Player server_data;

DWORD WINAPI ProcessClient( LPVOID arg ); // Ŭ�� ������ ���� ������
unsigned int __stdcall CompletionThread( LPVOID pComPort );


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// ���� ��� ������ ����
	CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );

	SetTimer( shareHwnd, 1, 1024, TimerProc );

	// ��ȭ���� ����
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	// closesocket()
	closesocket( sock );

	// ���� ����
	WSACleanup();
	return 0;
}

void CALLBACK TimerProc( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime ) {   //�Լ� ����
	HDC hdc;
	hdc = GetDC( hWnd );
	if ( server_data.gr.round_wait == true ) {
		if ( server_data.gr.time_wait <= 0 ) {
			// wait �� ��� ���������
			server_data.gr.round_start = true;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
		}
		server_data.gr.time_wait -= 1;
		DisplayEditText( hTextBox[2], "���ӽ��� ���� = %d : %d", server_data.gr.time_wait / 60, server_data.gr.time_wait % 60 );
	}

	if ( server_data.gr.round_start == true ) {
		if ( server_data.gr.round_time <= 0 ) {
			// ���尡 ��������� ���� ����� �Ѱ��ش�..!
			server_data.gr.round_start = false;
			server_data.gr.GameRound += 1;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
			Kill_Death( 0 );
			Kill_Death( 1 );
		}
		server_data.gr.round_time -= 1;
		if ( server_data.gr.round_time % 60 < 10 ) {
			DisplayEditText( hTextBox[2], "�����ð� = %d : 0%d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );
		}
		else {
			DisplayEditText( hTextBox[2], "�����ð� = %d : %d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );
		}
	}
	else {
		// ���尡 false�ϰ�� 1�ʸ��� üũ�� ���ش�.
		int user = 0;
		for ( int i = 0; i < MAX_Client; ++i ) {
			if ( server_data.player[i].Game_Play == true )
				user++;
		}
		if ( user >= 2 )
			server_data.gr.round_wait = true; // �����ڰ� 2�� �̻��̸� time_wait�� true�� �Ͽ� ��ü ��⸦ �����Ѵ�.
	}
	ReleaseDC( hWnd, hdc );
}


BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	shareHwnd = hDlg;
	switch ( uMsg ) {
	case WM_INITDIALOG:
		//hProgress = GetDlgItem( hDlg, IDC_PROGRESS1 );
		hFont[0] = CreateFont( 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����" );
		hFont[1] = CreateFont( 30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����" );
		hFont[2] = CreateFont( 35, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "����" );

		hList = GetDlgItem( hDlg, IDC_LIST1 );
		hTextBox[0] = GetDlgItem( hDlg, IDC_EDIT1 );
		hTextBox[1] = GetDlgItem( hDlg, IDC_EDIT2 );
		hTextBox[2] = GetDlgItem( hDlg, IDC_EDIT3 );

		Time_Button[0] = GetDlgItem( hDlg, IDC_BUTTON18 ); // 1��
		Time_Button[1] = GetDlgItem( hDlg, IDC_BUTTON19 ); // 2��
		Time_Button[2] = GetDlgItem( hDlg, IDC_BUTTON20 ); // 3��
		Time_Button[3] = GetDlgItem( hDlg, IDC_BUTTON21 ); // 4��
		Time_Button[4] = GetDlgItem( hDlg, IDC_BUTTON22 ); // 5��
		Time_Button[5] = GetDlgItem( hDlg, IDC_BUTTON23 ); // Next Round

		hPlayer[0] = GetDlgItem( hDlg, IDC_BUTTON2 );
		hPlayer[1] = GetDlgItem( hDlg, IDC_BUTTON4 );
		hPlayer[2] = GetDlgItem( hDlg, IDC_BUTTON6 );
		hPlayer[3] = GetDlgItem( hDlg, IDC_BUTTON8 );
		hPlayer[4] = GetDlgItem( hDlg, IDC_BUTTON10 );
		hPlayer[5] = GetDlgItem( hDlg, IDC_BUTTON12 );
		hPlayer[6] = GetDlgItem( hDlg, IDC_BUTTON14 );
		hPlayer[7] = GetDlgItem( hDlg, IDC_BUTTON16 );

		hPlayerOff[0] = GetDlgItem( hDlg, IDC_BUTTON3 );
		hPlayerOff[1] = GetDlgItem( hDlg, IDC_BUTTON5 );
		hPlayerOff[2] = GetDlgItem( hDlg, IDC_BUTTON7 );
		hPlayerOff[3] = GetDlgItem( hDlg, IDC_BUTTON9 );
		hPlayerOff[4] = GetDlgItem( hDlg, IDC_BUTTON11 );
		hPlayerOff[5] = GetDlgItem( hDlg, IDC_BUTTON13 );
		hPlayerOff[6] = GetDlgItem( hDlg, IDC_BUTTON15 );
		hPlayerOff[7] = GetDlgItem( hDlg, IDC_BUTTON17 );
		//SendMessage( hProgress, PBM_SETPOS, 0, 0 );
		//SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)"�߰��Ѵ�." );

		// �ٱ��� �Ҷ��� : \r\n
		SendMessage( hTextBox[0], WM_SETFONT, (WPARAM)hFont[0], (LPARAM)TRUE );
		SendMessage( hTextBox[1], WM_SETFONT, (WPARAM)hFont[1], (LPARAM)TRUE );
		SendMessage( hTextBox[2], WM_SETFONT, (WPARAM)hFont[2], (LPARAM)TRUE );
		DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
		DisplayEditText( hTextBox[1], "%d vs %d", aTeam, bTeam );
		DisplayEditText( hTextBox[2], "�����ð� = %d : %d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );

		DisplayText( hList, "%d Thread Created..!", SystemInfo.dwNumberOfProcessors * 2 );
		SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)"Server is Ready..!" );

		return TRUE;


	case WM_COMMAND:

		switch ( LOWORD( wParam ) ) {
		case IDC_LIST1:
			switch ( HIWORD( wParam ) ) {
			case LBN_SELCHANGE:
				int listIndex = SendMessage( hList, LB_GETCURSEL, 0, 0 ); //������ ����Ʈ ��ȣ�� ���Ѵ�.
				SendMessage( hList, LB_GETTEXT, listIndex, (LPARAM)buf ); // ���� ��ȣ�� ����Ʈ�� ������ str�� �����Ѵ�.
				MessageBox( hList, buf, "NetworkGameTermProject_ServerGUI", 0 );
				break;
			}
			return TRUE;

		case IDC_BUTTON3:
			client[0] = false;
			return TRUE;
		case IDC_BUTTON5:
			client[1] = false;
			return TRUE;
		case IDC_BUTTON7:
			client[2] = false;
			return TRUE;
		case IDC_BUTTON9:
			client[3] = false;
			return TRUE;
		case IDC_BUTTON11:
			client[4] = false;
			return TRUE;
		case IDC_BUTTON13:
			client[5] = false;
			return TRUE;
		case IDC_BUTTON15:
			client[6] = false;
			return TRUE;
		case IDC_BUTTON17:
			client[7] = false;
			return TRUE;

		case  IDC_BUTTON18:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 60;
			defaultRoundTime = 60;
			return TRUE;
		case IDC_BUTTON19:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 120;
			defaultRoundTime = 120;
			return TRUE;
		case IDC_BUTTON20:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 180;
			defaultRoundTime = 180;
			return TRUE;
		case IDC_BUTTON21:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 240;
			defaultRoundTime = 240;
			return TRUE;
		case IDC_BUTTON22:
			server_data.gr.exit_Round = false;
			server_data.gr.round_start = false;
			server_data.gr.GameRound += 1;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			Kill_Death( 1 );
			return TRUE;
		case IDC_BUTTON23:
			// ���� ���� ����
			server_data.gr.GameRound = 1;
			server_data.gr.round_start = false;
			server_data.gr.round_wait = false;
			server_data.gr.exit_Round = true;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			Kill_Death( 0 );
			Kill_Death( 1 );
			DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
			DisplayEditText( hTextBox[2], "���� ����..!" );
			return TRUE;


		case IDCANCEL:
			EndDialog( hDlg, IDCANCEL );
			return TRUE;

		}
		return FALSE;
	}
	return FALSE;
}

DWORD WINAPI ProcessClient( LPVOID arg ) {
	WSADATA wsaData;
	HANDLE hCompletionPort;
	// ������� CompletionPort�� ���޵� Handle

	/*
	�ý��� ������ ���޵� �����带 �����Ҷ� CPU �� ������ ����
	�����带 ����� ��� �ϱ� ������ ������ ����
	*/
	SOCKADDR_IN servAddr;

	LPPER_IO_DATA PerIoData;
	//����������ü�θ�����ؼ����ǹ�������

	LPPER_HANDLE_DATA PerHandleData;
	//��������������ɱ���ü���⼭�¼����ڵ���ּҸ��������ִ�.

	int RecvBytes;
	int i, Flags;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		DisplayText( hList, "WSAStartup() error!" );

	//1. Completion Port ����.
	hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	/*
	CreateIoCompletionPort �� ù��°�� �ش��ϸ�
	Completion Port �� ������ִ� �κ��̴�.
	�����̾߱⸦ ���� ���ڸ� ������ ����� �ִ� �κ��� �ȴ�.
	*/
	GetSystemInfo( &SystemInfo );
	/*
	�ý��� ������ ���´�.
	�̴� �ռ� �������� CPU�� ������ŭ �����带 ����� ���ؼ� �̴�
	�̷��� ���� �ý��� ������ �ɹ����� dwNumberOfProcessors ��CPU�� ����������.
	*/

	//2. Completion Port ���� ����� �ϷḦ ����ϴ� �����带 CPU ���� * 2 ��ŭ ����.
	for ( i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++ )
		_beginthreadex( NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL );
	/*
	CPU�� ������ŭ �����带 ����� �ִ� �κ��̴�.
	�̶� ���� ��������� �����忡 �̸� ������ Completion Port �������ϴµ�
	�̸����� unsigned int __stdcall CompletionThread(LPVOID pComPort); �Լ������ڷ�
	������� ������ Completion Port �����޵ȴ�.
	*/
	bool flag = TRUE;
	sock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	// ������ ���鶧�� �� ��ø ����� �������� ����� �־�� �Ѵ�.
	setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof( flag ) );
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	servAddr.sin_port = htons( SERVERPORT );
	bind( sock, (SOCKADDR*)&servAddr, sizeof( servAddr ) );
	listen( sock, 5 );

	while ( TRUE ) {
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof( clntAddr );

		hClntSock = accept( sock, (SOCKADDR*)&clntAddr, &addrLen );

		DisplayText( hList, "Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d", inet_ntoa( clntAddr.sin_addr ), ntohs( clntAddr.sin_port ) );

		// �����Ŭ���̾�Ʈ�Ǽ����ڵ��������ּ�����������.
		PerHandleData = (LPPER_HANDLE_DATA)malloc( sizeof( PER_HANDLE_DATA ) );
		for ( int i = 0; i < MAX_Client; ++i ) {
			if ( client[i] == false ) {
				client[i] = true;
				PerHandleData->client_imei = i;
				DisplayText( hList, "Client ID : %d\n", PerHandleData->client_imei );
				DisplayText( hList, "Client Team : %d\n", PerHandleData->client_imei % 2 );
				break;
			}
		}

		sprintf( buf, "Player %d : %s:%d [ Team : %d ]", PerHandleData->client_imei, inet_ntoa( clntAddr.sin_addr ), ntohs( clntAddr.sin_port ), PerHandleData->client_imei % 2 );
		SendMessage( hPlayer[PerHandleData->client_imei], WM_SETTEXT, 0, (LPARAM)buf );

		PerHandleData->hClntSock = hClntSock;
		memcpy( &(PerHandleData->clntAddr), &clntAddr, addrLen );
		/*
		PerHandleData�� ����� Ŭ���̾�Ʈ�� �������� �����Ѵ�.
		�̶� ���� �Ҵ����� ������ ���� �ϴµ� ���� �Ҵ��� �𸥴ٸ� �����ϰ� ������ ����
		*/

		//3. Overlapped ���ϰ� CompletionPort�� ����.
		CreateIoCompletionPort( (HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0 );
		/*
		CreateIoCompletionPort �� �ι�° ��ɿ� �ش� �Ѵ�.
		���� ����� Ŭ���̾�Ʈ�� ������ CompletionPort ������Ʈ�� ���� ���ش�.
		����° ���ڷ� ������ Ŭ���̾�Ʈ ������ �����ߴ� PerHandleData�� �Ѱ� �ش�.
		�̸� ���ؼ� ������� �Ϸ�� Ŭ���̾�Ʈ�� ������ ��°��� ���� �ϴ�.
		����� �Ʒ����� �̺κ��� �ú���� �����ϰ� �ɰ� �̴�.
		*/

		// ����� Ŭ���̾�Ʈ�� ���� ���۸� �����ϰ� OVERLAPPED ����ü ���� �ʱ�ȭ.
		PerIoData = (LPPER_IO_DATA)malloc( sizeof( PER_IO_DATA ) );
		memset( &(PerIoData->overlapped), 0, sizeof( OVERLAPPED ) );
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;
		PerIoData->Incoming_data = Recv_Mode;

		/*
		PerIoData �� ����� �ְ� �ʱ�ȭ �մϴ�.
		�̴� ���� ���������� ���ϴ� ����ü �Դϴ�.
		*/
		Flags = 0;

		//4. ��ø�� �������Է�.
		WSARecv( PerHandleData->hClntSock, // ������ �Է¼���.
			&(PerIoData->wsaBuf),  // ������ �Է� ����������.
			1,       // ������ �Է� ������ ��.
			(LPDWORD)&RecvBytes,
			(LPDWORD)&Flags,
			&(PerIoData->overlapped), // OVERLAPPED ����ü ������.
			NULL
		);
		/*
		�ٸ� �����ؼ� ������ ������° ���ڷ� ���޵�PerIoData ����ü�� overlapped �̴�.
		���� �ִٰ� �̺κе� �ٽú��� �ɰ��̴�.
		*/
	}
	return 0;
}

unsigned int __stdcall CompletionThread( LPVOID pComPort ) {
	HANDLE hCompletionPort = (HANDLE)pComPort;
	/*
	���ڷ� ���� pComPort ���� main�� hCompletionPort �� ���� �ȴ�.
	�̴� �����带 ������ main �� hCompletionPort �� ���ڷ� ���� �߱� �����̴�.
	*/
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	Player_Socket temp;
	int sendBytes = 0, player_hp, Attacked_Player;
	int player_kill, player_death;
	bool player_live;

	//WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );

	while ( 1 ) {
		GetQueuedCompletionStatus( hCompletionPort,    // Completion Port
			&BytesTransferred,   // ���۵� ����Ʈ��
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED ����ü ������.
			INFINITE
		);


		//DisplayText( hList, "%d ����� �����Ͽ����ϴ�..!", BytesTransferred );
		if ( BytesTransferred == 0 || client[PerHandleData->client_imei] == false ) { //EOF ���۽�.
			 // �� �������� Ŭ�������� �����.
			clrUser( PerHandleData->client_imei );
			client[PerHandleData->client_imei] = false;
			init_client[PerHandleData->client_imei] = false;
			PerIoData->Incoming_data = Recv_Mode;
			// �� GUI��ư�� �����Ѵ�.
			sprintf( buf, "Player %d : ", PerHandleData->client_imei );
			SendMessage( hPlayer[PerHandleData->client_imei], WM_SETTEXT, 0, (LPARAM)buf );
			// �� Display ���ش�.
			DisplayText( hList, "%d�� Ŭ���̾�Ʈ�� ���� ���� or ���� �Ǿ����ϴ�..!", PerHandleData->client_imei );
			closesocket( PerHandleData->hClntSock );
			free( PerHandleData );
			free( PerIoData );
			continue;
		}

		// Ŭ���̾�Ʈ ����ü�� �޾ƿ´�.
		//DisplayText( hList, "%d�� Ŭ���̾�Ʈ Mode : %d", PerHandleData->client_imei, PerIoData->Incoming_data );
		if ( PerIoData->Incoming_data == Recv_Mode ) {
			PerIoData->wsaBuf.buf[BytesTransferred] = '\0';

			player_hp = server_data.player[PerHandleData->client_imei].hp;
			player_live = server_data.player[PerHandleData->client_imei].live;
			player_kill = server_data.player[PerHandleData->client_imei].kill;
			player_death = server_data.player[PerHandleData->client_imei].death;

			temp = (Player_Socket&)PerIoData->buffer;

			if ( temp.AttackedPlayer >= 19930617 || temp.AttackedPlayer <= -5 ) {
				// ���� ���������� �ȵ�������� �ٽ� recv_mode�� ����.
				DisplayText( hList, "%d Ŭ���̾�Ʈ �����Ⱚ �⿬..!", PerHandleData->client_imei );
				PerIoData->Incoming_data = Send_Mode;
			}
			else {
				server_data.player[PerHandleData->client_imei] = (Player_Socket&)PerIoData->buffer;

				server_data.player[PerHandleData->client_imei].live = true;
				server_data.player[PerHandleData->client_imei].team = (bool)(PerHandleData->client_imei % 2);


				if ( init_client[PerHandleData->client_imei] == false ) {
					server_data.player[PerHandleData->client_imei].hp = 100;
					server_data.player[PerHandleData->client_imei].live = true;
				}
				else {
					server_data.player[PerHandleData->client_imei].hp = player_hp;
					server_data.player[PerHandleData->client_imei].live = player_live;
					server_data.player[PerHandleData->client_imei].kill = player_kill;
					server_data.player[PerHandleData->client_imei].death = player_death;
				}

				if ( server_data.player[PerHandleData->client_imei].RespawnTime == Respawn_Complete ) {
					server_data.player[PerHandleData->client_imei].hp = 100;
					server_data.player[PerHandleData->client_imei].live = true;
				}

				if ( server_data.player[PerHandleData->client_imei].AttackedPlayer != Not_Attacked ) {
					// �÷��̾ �׿������
					Attacked_Player = server_data.player[PerHandleData->client_imei].AttackedPlayer;
					server_data.player[Attacked_Player].hp -= 40;
					if ( server_data.player[Attacked_Player].hp <= 0 ) {
						server_data.player[PerHandleData->client_imei].kill += 1;
						server_data.player[Attacked_Player].death += 1;
						server_data.player[Attacked_Player].hp = 0;
						server_data.player[Attacked_Player].RespawnTime = 10;
						server_data.player[Attacked_Player].live = false;
						Kill_Death( 0 );
					}
				}


				if ( init_client[PerHandleData->client_imei] == false ) {
					// ó�� ������ �����͸� �޾Ҵ�. ��ư�� �÷��̾� �̸����� ��������.
					sprintf( buf, "Player %d : %s [ Team : %d ]", PerHandleData->client_imei, server_data.player[PerHandleData->client_imei].nickName, PerHandleData->client_imei % 2 );
					SendMessage( hPlayer[PerHandleData->client_imei], WM_SETTEXT, 0, (LPARAM)buf );
					init_client[PerHandleData->client_imei] = true;
				}

				PerIoData->Incoming_data = Send_Mode;
			}
		}


		if ( PerIoData->Incoming_data == Send_Mode ) {
			PerIoData->wsaBuf.len = sizeof( Server_Player );
			PerIoData->wsaBuf.buf = (char*)&server_data;
			PerIoData->Incoming_data = Recv_Mode;

			WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&sendBytes, 0, NULL, NULL );
			//DisplayText( hList, "%d Ŭ�� ���� �����͸� ���½��ϴ�..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len, PerIoData->wsaBuf.len );
		}

		// RECEIVE AGAIN
		memset( &(PerIoData->overlapped), 0, sizeof( OVERLAPPED ) );
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;

		flags = 0;
		WSARecv( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, &flags, &(PerIoData->overlapped), NULL );

	}
	return 0;
}

// ����Ʈ�� �ٷ� ���ü� �ְ� �۾��� �غ���
void DisplayText( HWND hDlg, char *fmt, ... ) {
	va_list arg;
	va_start( arg, fmt );

	char contentBuf[MAX_PATH]; //sprintf ���� ����Ҷ� ����
	vsprintf( contentBuf, fmt, arg );

	SendMessage( hDlg, LB_ADDSTRING, 0, (LPARAM)contentBuf );
}

void DisplayEditText( HWND hDlg, char *fmt, ... ) {
	va_list arg;
	va_start( arg, fmt );

	char contentBuf[MAX_PATH]; //sprintf ���� ����Ҷ� ����
	vsprintf( contentBuf, fmt, arg );

	SendMessage( hDlg, WM_SETTEXT, 0, (LPARAM)contentBuf );
}

void clrUser( int client_imei ) {
	server_data.player[client_imei].camxrotate = -1000.0f;
	server_data.player[client_imei].camyrotate = -1000.0f;
	server_data.player[client_imei].x = -1000.0f;
	server_data.player[client_imei].y = -1000.0f;
	server_data.player[client_imei].z = -1000.0f;
	server_data.player[client_imei].hp = 100;
	server_data.player[client_imei].AttackedPlayer = Not_Attacked;
	server_data.player[client_imei].RespawnTime = Not_Respwan;
	server_data.player[client_imei].live = false;
	server_data.player[client_imei].Game_Play = false;
	server_data.player[client_imei].team = false;
	server_data.player[client_imei].character_down_state = 0;
	strcpy( server_data.player[client_imei].nickName, "" );
}

void Kill_Death( int select ) {
	switch ( select )
	{
	case 0:
		// A�� ���
		aTeam = 0;
		for ( int i = 0; i < MAX_Client / 2; ++i ) {
			aTeam += server_data.player[i * 2].kill;
		}
		// B�� ���
		bTeam = 0;
		for ( int i = 0; i < MAX_Client / 2; ++i ) {
			bTeam += server_data.player[(i * 2) + 1].kill;
		}
		DisplayEditText( hTextBox[1], "%d vs %d", aTeam, bTeam );
		if ( aTeam > bTeam ) {
			server_data.gr.game_result = 0;
		}
		else {
			server_data.gr.game_result = 1;
		}
		break;

	case 1:
		// kd �ʱ�ȭ
		for ( int i = 0; i < MAX_Client; ++i ) {
			server_data.player[i].kill = 0;
			server_data.player[i].death = 0;
		}
		Kill_Death( 0 );
		break;

	default:
		break;
	}

}