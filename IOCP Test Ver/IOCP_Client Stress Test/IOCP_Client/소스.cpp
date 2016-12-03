#pragma comment(lib, "ws2_32.lib") 

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling( char *message );
#define BUFSIZE    512

#define Recv_Mode 0
#define Send_Mode 1
#define MAX_Client 4

int intt = 0;

struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsabuf;
};

typedef struct Player_Socket {
	// 전송을 위해 사용
	float x = 6, y = 16, z = 1993;
}Player_Socket;
Player_Socket player;

typedef struct ClientData {
	int client_imei = -1;
	bool team = -1;
}ClientData;
ClientData cd;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	Player_Socket player[MAX_Client];
}Server_Player;
Server_Player server_data;

int main() {
	WSADATA wsaData;
	SOCKET hSocket[100000];
	SOCKADDR_IN recvAddr;

	SOCKETINFO dataBuf;

	WSAEVENT event;
	WSAOVERLAPPED overlapped;

	for ( int i = 0; i < 100000; ++i ) {
		char message[1024] = { 0, };
		int sendBytes = 0;
		int recvBytes = 0;
		int flags = 0;

		if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
			ErrorHandling( "WSAStartup() error!" );

		hSocket[i] = WSASocket( PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
		if ( hSocket[i] == INVALID_SOCKET )
			ErrorHandling( "socket() error" );

		memset( &recvAddr, 0, sizeof( recvAddr ) );
		recvAddr.sin_family = AF_INET;
		recvAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		recvAddr.sin_port = htons( atoi( "9000" ) );

		if ( connect( hSocket[i], (SOCKADDR*)&recvAddr, sizeof( recvAddr ) ) == SOCKET_ERROR )
			ErrorHandling( "connect() error!" );

		printf( "%d 연결 완료..!\n", i );
	}





	while ( 1 ) {

	}
	return 0;
}

void ErrorHandling( char *message ){
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&lpMsgBuf, 0, NULL );

	fputs( message, stderr );
	printf( "%s", (char *)lpMsgBuf );
	fputc( '\n', stderr );
	exit( 1 );
}