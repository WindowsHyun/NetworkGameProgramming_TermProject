#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdlib.h>
#include <atlstr.h> // cstring
#include <Windows.h>
#include "resource.h"
#include "Server.h"

bool client[MAX_Client] = { false }; //Ŭ���̾�Ʈ�� ������ ��� ó���Ѵ�.
char buf[MAX_PATH];

// ������ ���ν���
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

// ���� ��� �Լ�
void err_quit( char *msg );
void err_display( char *msg );
void DisplayText( HWND hDlg, char *fmt, ... );

SOCKET listen_sock; // ����
HWND shareHwnd, hList, hTextBox;
HWND hPlayer[7], hPlayerOff[7];

Server_Player server_data;

DWORD WINAPI ProcessClient( LPVOID arg ); // Ŭ�� ������ ���� ������
DWORD WINAPI Thread_Server( LPVOID arg ); // ������ ���� ������ �ޱ����� ������
DWORD WINAPI Draw_Map( LPVOID arg ); // ���� GUI�� ���� �׷��ֱ� ���Ͽ�?
int recvn( SOCKET s, char *buf, int len, int flags );

void clrUser( int ); // ������ �����ų� ����� ������ �����.


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// ���� ��� ������ ����
	CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );

	// ��ȭ���� ����
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	// closesocket()
	closesocket( listen_sock );

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
	int retval;
	int send_buf = 200;
	int rcv_buf = 50;
	bool flag = TRUE;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return 1;

	// socket()
	listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listen_sock == INVALID_SOCKET ) err_quit( "socket()" );
	setsockopt( listen_sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof( rcv_buf ) );
	setsockopt( listen_sock, SOL_SOCKET, SO_SNDBUF, (char*)&send_buf, sizeof( send_buf ) );
	setsockopt( listen_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof( flag ) );

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = bind( listen_sock, (SOCKADDR *)&serveraddr, sizeof( serveraddr ) );
	if ( retval == SOCKET_ERROR ) err_quit( "bind()" );

	// listen()
	retval = listen( listen_sock, SOMAXCONN );
	if ( retval == SOCKET_ERROR ) err_quit( "listen()" );

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;


	while ( 1 ) {
		// accept()
		addrlen = sizeof( clientaddr );
		client_sock = accept( listen_sock, (SOCKADDR *)&clientaddr, &addrlen );
		if ( client_sock == INVALID_SOCKET ) {
			err_display( "accept()" );
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		DisplayText( hList, "Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );
		hThread = CreateThread( NULL, 0, Draw_Map, (LPVOID)client_sock, 0, NULL );


		// ������ ����
		hThread = CreateThread( NULL, 0, Thread_Server, (LPVOID)client_sock, 0, NULL );
		if ( hThread == NULL ) {
			closesocket( client_sock );
		}
		else {
			CloseHandle( hThread );
		}

	}
	// closesocket()
	closesocket( listen_sock );

	// ���� ����
	WSACleanup();
	return 0;
}

DWORD WINAPI Thread_Server( LPVOID arg ) {
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;
	int len;
	int client_imei = -1;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof( clientaddr );
	getpeername( client_sock, (SOCKADDR *)&clientaddr, &addrlen );
	for ( int i = 0; i < MAX_Client; ++i ) {
		if ( client[i] == false ) {
			client[i] = true;
			client_imei = i;
			DisplayText( hList, "Client ID : %d\n", client_imei );
			DisplayText( hList, "Client Team : %d\n", client_imei % 2 );
			break;
		}
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Ŭ�� ���� �������� ID�� ������� Ȯ���� �����ش�.
	char client_imei_c[10];
	itoa( client_imei, client_imei_c, 10 );
	len = sizeof( client_imei_c );
	retval = send( client_sock, (char *)&len, sizeof( int ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		//exit( 1 );
	}

	// ������ ������( ����ü �����͸� ������. )
	retval = send( client_sock, (char*)&client_imei_c, sizeof( client_imei_c ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		//exit( 1 );
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Ŭ�� ���� ���� ������ �����ش�.
	char client_team[10];
	itoa( client_imei % 2, client_team, 10 );
	len = sizeof( client_team );
	retval = send( client_sock, (char *)&len, sizeof( int ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		//exit( 1 );
	}

	// ������ ������( ����ü �����͸� ������. )
	retval = send( client_sock, (char*)&client_team, sizeof( client_team ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		//exit( 1 );
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if ( client_imei == -1 ) {
		// �ο��� ���� ���̻� ����� �´�.
		closesocket( client_sock );
		DisplayText( hList, "Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d [�ο� �ʰ�]\n", inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ) );
		return 0;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// char nickName �޾ƿ���
	//retval = recvn( sock, (char *)&len, sizeof( int ), 0 ); // ������ �ޱ�(���� ����)
	//if ( retval == SOCKET_ERROR ) {
	//	err_display( "recv()" );
	//}
	//else if ( retval == 0 ) {
	//}

	//char *buf = new char[len]; // ���۵� ���̸� �˰� ������ ũ�⿡ ���缭 buf�� �÷�����!

	//retval = recvn( sock, buf, len, 0 );
	//if ( retval == SOCKET_ERROR ) {
	//	err_display( "recv()" );
	//}


	//for ( int i = 0; i < MAX_Client; ++i ) {
	//	if ( strcmp( buf, server_data.player[i].nickName) != 0 ) {

	//	}

	//}

	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// �� client_imei �� �������� ��ư�� (������:��Ʈ) �� �־��ش�.
	bool getnickName = false; // ĳ���� ������ ������� ��ü�� �ϱ� ���Ͽ�
	sprintf( buf, "Player %d : %s:%d [ Team : %d ]", client_imei, inet_ntoa( clientaddr.sin_addr ), ntohs( clientaddr.sin_port ), client_imei % 2 );
	SendMessage( hPlayer[client_imei], WM_SETTEXT, 0, (LPARAM)buf );
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while ( 1 ) {
		retval = recvn( client_sock, (char *)&len, sizeof( int ), 0 ); // ������ �ޱ�(���� ����)
		if ( retval == SOCKET_ERROR ) {
			err_display( "recv()" );
			break;
		}
		else if ( retval == 0 )
			break;
		//printf( "������ ũ�� : %d\n", len );

		int GetSize;
		char suBuffer[BUFSIZE];
		Player *player;
		GetSize = recv( client_sock, suBuffer, len, 0 );

		suBuffer[GetSize] = '\0';
		player = (Player*)suBuffer;

		// ������ �ޱ�(���� ����)
		if ( retval == SOCKET_ERROR ) {
			err_display( "recv()" );
			break;
		}
		else if ( retval == 0 )
			break;

		//printf( "------------------------------------------\n" );
		//printf( "Client ID : %d\n", client_imei );
		//printf( "Camxrotate : %f\nCamyrotate : %f\nPlayer->X : %f\nPlayer->Y : %f\nPlayer->Z : %f\n", player->camxrotate, player->camyrotate, player->socket.x, player->socket.y, player->socket.z );
		//printf( "------------------------------------------\n" );

		server_data.player[client_imei].camxrotate = player->socket.camxrotate;
		server_data.player[client_imei].camyrotate = player->socket.camyrotate;
		server_data.player[client_imei].x = player->socket.x;
		server_data.player[client_imei].y = player->socket.y;
		server_data.player[client_imei].z = player->socket.z;
		server_data.player[client_imei].team = player->socket.team;
		server_data.player[client_imei].live = player->socket.live;
		server_data.player[client_imei].character_down_state = player->socket.character_down_state;
		strcpy( server_data.player[client_imei].nickName, player->socket.nickName );
		if ( getnickName == false && strcmp( server_data.player[client_imei].nickName, "") != 0 ) {
			sprintf( buf, "Player %d : %s [ Team : %d ]", client_imei, server_data.player[client_imei].nickName, client_imei % 2 );
			SendMessage( hPlayer[client_imei], WM_SETTEXT, 0, (LPARAM)buf );
			getnickName = true;
		}

		//printf( "Xrotate : %f\n", server_data.player[0].camxrotate );

		//int retval;
		// ������ ������( ����ü ũ�⸦ ���� ������. )
		int len = sizeof( server_data );
		retval = send( client_sock, (char *)&len, sizeof( int ), 0 );
		if ( retval == SOCKET_ERROR ) {
			err_display( "send()" );
			//exit( 1 );
		}
		//DisplayText( hList, "send : %d", len );

		// ������ ������( ����ü �����͸� ������. )
		retval = send( client_sock, (char*)&server_data, sizeof( Server_Player ), 0 );
		if ( retval == SOCKET_ERROR ) {
			err_display( "send()" );
			//exit( 1 );
		}

		if ( client[client_imei] == false ) {
			// ������ �������� ���� ��ų���
			closesocket( client_sock );
			clrUser( client_imei );
			DisplayText( hList, "%d�� Ŭ���̾�Ʈ�� ���� ���� ���׽��ϴ�..!", client_imei );
			sprintf( buf, "Player %d : " );
			SendMessage( hPlayer[client_imei], WM_SETTEXT, 0, (LPARAM)buf );
			break; // break�� while���� ������ ���������Ƿ� �������ᰡ ����������.
		}

	}
	closesocket( client_sock );
	clrUser( client_imei );
	sprintf( buf, "Player %d : ", client_imei );
	SendMessage( hPlayer[client_imei], WM_SETTEXT, 0, (LPARAM)buf );
	client[client_imei] = false;

	return 0;
}

DWORD WINAPI Draw_Map( LPVOID arg ) {



	//SendMessage( hTextBox, WM_SETTEXT, 0, (LPARAM)buf );
	return 0;
}

// ���� �Լ� ���� ��� �� ����
void err_quit( char *msg ) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&lpMsgBuf, 0, NULL );
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR );
	LocalFree( lpMsgBuf );
	exit( 1 );
}

// ���� �Լ� ���� ���
void err_display( char *msg ) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&lpMsgBuf, 0, NULL );
	DisplayText( hList, "[%s] %s", msg, (char *)lpMsgBuf );
	LocalFree( lpMsgBuf );
}

// ����Ʈ�� �ٷ� ���ü� �ְ� �۾��� �غ���
void DisplayText( HWND hDlg, char *fmt, ... ) {
	va_list arg;
	va_start( arg, fmt );

	char contentBuf[MAX_PATH]; //sprintf ���� ����Ҷ� ����
	vsprintf( contentBuf, fmt, arg );

	SendMessage( hDlg, LB_ADDSTRING, 0, (LPARAM)contentBuf );
}

// ����� ���� ������ ���� �Լ�
int recvn( SOCKET s, char *buf, int len, int flags ) {
	int received;
	char *ptr = buf;
	int left = len;

	while ( left > 0 ) {
		received = recv( s, ptr, left, flags );
		if ( received == SOCKET_ERROR )
			return SOCKET_ERROR;
		else if ( received == 0 )
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

void clrUser( int client_imei ) {
	server_data.player[client_imei].camxrotate = -1000.0f;
	server_data.player[client_imei].camyrotate = -1000.0f;
	server_data.player[client_imei].x = -1000.0f;
	server_data.player[client_imei].y = -1000.0f;
	server_data.player[client_imei].z = -1000.0f;
	server_data.player[client_imei].live = false;
	server_data.player[client_imei].team = false;
	server_data.player[client_imei].character_down_state = 0;
	strcpy( server_data.player[client_imei].nickName, "" );
}