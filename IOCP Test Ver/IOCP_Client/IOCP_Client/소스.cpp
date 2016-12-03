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
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;

	SOCKETINFO dataBuf;
	char message[1024] = { 0, };
	int sendBytes = 0;
	int recvBytes = 0;
	int flags = 0;

	WSAEVENT event;
	WSAOVERLAPPED overlapped;


	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		ErrorHandling( "WSAStartup() error!" );

	hSocket = WSASocket( PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if ( hSocket == INVALID_SOCKET )
		ErrorHandling( "socket() error" );

	memset( &recvAddr, 0, sizeof( recvAddr ) );
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	recvAddr.sin_port = htons( atoi( "9000" ) );

	if ( connect( hSocket, (SOCKADDR*)&recvAddr, sizeof( recvAddr ) ) == SOCKET_ERROR )
		ErrorHandling( "connect() error!" );


	//구조체에이벤트핸들삽입해서전달
	event = WSACreateEvent();
	memset( &overlapped, 0, sizeof( overlapped ) );

	overlapped.hEvent = event;
	dataBuf.Incoming_data = Send_Mode;


	//전송할데이터
	while ( true )
	{
		flags = 0;
		intt++;
		//printf( "전송할데이터(종료를원할시exit)\n:" );
		//scanf( "%s", message );

		//if ( !strcmp( message, "exit" ) ) break;


		//printf( "-- %d --\n", dataBuf.Incoming_data );

		if ( dataBuf.Incoming_data == Send_Mode ) {
			player.x += 1, player.y += 2;
			printf( "Send : x: %f, y : %f, z: %f\n", player.x, player.y, player.z );
			dataBuf.wsabuf.len = sizeof( Player_Socket );
			dataBuf.wsabuf.buf = (char*)&player;
			dataBuf.Incoming_data = Recv_Mode;

			if ( WSASend( hSocket, &dataBuf.wsabuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL ) == SOCKET_ERROR ) {
				if ( WSAGetLastError() != WSA_IO_PENDING )
					ErrorHandling( "WSASend() error" );
			}
			//전송완료확인
			WSAWaitForMultipleEvents( 1, &event, TRUE, WSA_INFINITE, FALSE ); //데이터전송끝났는지확인. 
			//전송된바이트수확인
			WSAGetOverlappedResult( hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL );//실지로전송된바이트수를얻어낸다.
		}


		if ( dataBuf.Incoming_data == Recv_Mode ) {

			memset( &(overlapped), 0, sizeof( OVERLAPPED ) );
			dataBuf.wsabuf.len = BUFSIZE;
			dataBuf.wsabuf.buf = dataBuf.buf;
			dataBuf.Incoming_data = Send_Mode;

			if ( WSARecv( hSocket, &dataBuf.wsabuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL ) == SOCKET_ERROR ) {
				if ( WSAGetLastError() != WSA_IO_PENDING )
					ErrorHandling( "WSASend() error" );
			}
				// 한번 이상 받은경우 서버 구조체를 받는다.
				dataBuf.wsabuf.buf[recvBytes] = '\0';
				Server_Player * data;
				data = (Server_Player*)dataBuf.wsabuf.buf;
				server_data = *data;
				printf( "Recv : x: %f, y : %f, z: %f\n", server_data.player[0].x, server_data.player[0].y, server_data.player[0].z );
				//printf( "x: %f, y : %f, z : %f [%d]\n", server_data.player[0].x, server_data.player[0].y, server_data.player[0].z, recvBytes );
		}




		//printf( "전송된바이트수: %d \n", sendBytes );
		//if ( WSARecv( hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL ) == SOCKET_ERROR )
		//{
		//	if ( WSAGetLastError() != WSA_IO_PENDING )
		//		ErrorHandling( "WSASend() error" );
		//}
		//printf( "Recv[%s]\n", dataBuf.buf );




	}

	closesocket( hSocket );
	WSACleanup();

	return 0;
}

void ErrorHandling( char *message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	printf( "전송 횟수 : %d\n", intt );
	exit( 1 );
}