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
	// ������ ���� ���
	float x = 6, y = 16, z = 1993;
}Player_Socket;
Player_Socket player;

typedef struct ClientData {
	int client_imei = -1;
	bool team = -1;
}ClientData;
ClientData cd;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
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


	//����ü���̺�Ʈ�ڵ�����ؼ�����
	event = WSACreateEvent();
	memset( &overlapped, 0, sizeof( overlapped ) );

	overlapped.hEvent = event;
	dataBuf.Incoming_data = Send_Mode;


	//�����ҵ�����
	while ( true )
	{
		flags = 0;
		intt++;
		//printf( "�����ҵ�����(���Ḧ���ҽ�exit)\n:" );
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
			//���ۿϷ�Ȯ��
			WSAWaitForMultipleEvents( 1, &event, TRUE, WSA_INFINITE, FALSE ); //���������۳�������Ȯ��. 
			//���۵ȹ���Ʈ��Ȯ��
			WSAGetOverlappedResult( hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL );//���������۵ȹ���Ʈ��������.
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
				// �ѹ� �̻� ������� ���� ����ü�� �޴´�.
				dataBuf.wsabuf.buf[recvBytes] = '\0';
				Server_Player * data;
				data = (Server_Player*)dataBuf.wsabuf.buf;
				server_data = *data;
				printf( "Recv : x: %f, y : %f, z: %f\n", server_data.player[0].x, server_data.player[0].y, server_data.player[0].z );
				//printf( "x: %f, y : %f, z : %f [%d]\n", server_data.player[0].x, server_data.player[0].y, server_data.player[0].z, recvBytes );
		}




		//printf( "���۵ȹ���Ʈ��: %d \n", sendBytes );
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
	printf( "���� Ƚ�� : %d\n", intt );
	exit( 1 );
}