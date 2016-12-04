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

// ������ ���ν���
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

// ���� ��� �Լ�
void DisplayText( HWND hDlg, char *fmt, ... );

SOCKET sock; // ����
HWND shareHwnd, hList, hTextBox;
HWND hPlayer[8], hPlayerOff[8];
SYSTEM_INFO SystemInfo;

Server_Player server_data;

DWORD WINAPI ProcessClient( LPVOID arg ); // Ŭ�� ������ ���� ������
unsigned int __stdcall CompletionThread( LPVOID pComPort );

void clrUser( int ); // ������ �����ų� ����� ������ �����.


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// ���� ��� ������ ����
	CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );

	// ��ȭ���� ����
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	// closesocket()
	closesocket( sock );

	// ���� ����
	WSACleanup();
	return 0;
}

BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	shareHwnd = hDlg;
	switch ( uMsg ) {
	case WM_INITDIALOG:
		//hProgress = GetDlgItem( hDlg, IDC_PROGRESS1 );
		hList = GetDlgItem( hDlg, IDC_LIST1 );
		hTextBox = GetDlgItem( hDlg, IDC_EDIT1 );
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
		SendMessage( hTextBox, EM_REPLACESEL, 0, (LPARAM)"as\r\nas" );
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
				//DisplayText( hList, "nickName : %s, live : %d", server_data.player[PerHandleData->client_imei].nickName, server_data.player[PerHandleData->client_imei].live );
				//DisplayText( hList, "x : %f, y : %f, nickName : %s", server_data.player[PerHandleData->client_imei].x, server_data.player[PerHandleData->client_imei].y, server_data.player[PerHandleData->client_imei].nickName );

				if ( init_client[PerHandleData->client_imei] == false ) {
					server_data.player[PerHandleData->client_imei].hp = 100;
					server_data.player[PerHandleData->client_imei].live = true;
				}
				else {
					server_data.player[PerHandleData->client_imei].hp = player_hp;
					server_data.player[PerHandleData->client_imei].live = player_live;
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
						server_data.player[Attacked_Player].hp = 0;
						server_data.player[Attacked_Player].RespawnTime = 10;
						server_data.player[Attacked_Player].live = false;
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
	server_data.player[client_imei].team = false;
	server_data.player[client_imei].character_down_state = 0;
	strcpy( server_data.player[client_imei].nickName, "" );
}