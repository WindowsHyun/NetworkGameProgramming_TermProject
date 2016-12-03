#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib") 

#define BUFSIZE 1024
#define MAX_Client 4
#define Recv_Mode 0
#define Send_Mode 1

typedef struct //��������������üȭ.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
	int client_imei = -1;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct // �����ǹ�������������üȭ.
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	bool Incoming_data;
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	bool Incoming_data;
	WSABUF wsabuf;
};

typedef struct Player_Socket {
	// ������ ���� ���
	float x = -1, y = -1, z = -1;
}Player_Socket;

Player_Socket player;

typedef struct Server_Player {
	//�÷��̾� �����͸� ���������
	Player_Socket player[MAX_Client];
}Server_Player;
Server_Player server_data;

bool client[MAX_Client] = { false }; // Ŭ���̾�Ʈ�� ������ ��� ó���Ѵ�.

/*
���� ���� ������ ����ü�� ����µ� �̶� WSABUF �� overlapped �� �����Ѵ�.
WSABUF ��WSASend ��WSARecv �Լ��� ���ڷ� ���޵Ǵ� ���ۿ� ���Ǵ� ����ü �̱⿡ ���� �ǰ�
overlapped ����ü ������ �־��ִ°� ���� �Ϸ�� ����� ������ ��� ���� ��� �ȴ�.
*/

unsigned int __stdcall CompletionThread( LPVOID pComPort );
/*
�Ϸ�� �����忡 ���� ó���� ���ִ� �Լ� �̴�.
*/

void ErrorHandling( char *message );

int main( int argc, char** argv )
{
	WSADATA wsaData;
	HANDLE hCompletionPort;
	// ������� CompletionPort�� ���޵� Handle

	SYSTEM_INFO SystemInfo;
	/*
	�ý��� ������ ���޵� �����带 �����Ҷ� CPU �� ������ ����
	�����带 ����� ��� �ϱ� ������ ������ ����
	*/
	SOCKADDR_IN servAddr;

	LPPER_IO_DATA PerIoData;
	//����������ü�θ�����ؼ����ǹ�������

	LPPER_HANDLE_DATA PerHandleData;
	//��������������ɱ���ü���⼭�¼����ڵ���ּҸ��������ִ�.

	SOCKET hServSock;
	int RecvBytes;
	int i, Flags;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		ErrorHandling( "WSAStartup() error!" );

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

	//2. Completion Port ���� ����� �ϷḦ ����ϴ� �����带 CPU ������ŭ ����.
	for ( i = 0; i < SystemInfo.dwNumberOfProcessors; i++ )
		_beginthreadex( NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL );
	/*
	CPU�� ������ŭ �����带 ����� �ִ� �κ��̴�.
	�̶� ���� ��������� �����忡 �̸� ������ Completion Port �������ϴµ�
	�̸����� unsigned int __stdcall CompletionThread(LPVOID pComPort); �Լ������ڷ�
	������� ������ Completion Port �����޵ȴ�.

	������ ���ε��ڸ� ������� ����ϴ� �κ� �Դϴ�.
	���� ũ�⿡ �°� ������� ����ϰԵ˴ϴ�
	���� ������� �Ҽ��� ���� ������� �����̶�� ��Ÿ���� �κ��Դϴ�.
	*/

	hServSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	// ������ ���鶧�� �� ��ø ����� �������� ����� �־�� �Ѵ�.
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
	servAddr.sin_port = htons( atoi( "9000" ) );

	bind( hServSock, (SOCKADDR*)&servAddr, sizeof( servAddr ) );
	listen( hServSock, 5 );

	while ( TRUE )
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof( clntAddr );

		hClntSock = accept( hServSock, (SOCKADDR*)&clntAddr, &addrLen );

		// �����Ŭ���̾�Ʈ�Ǽ����ڵ��������ּ�����������.
		PerHandleData = (LPPER_HANDLE_DATA)malloc( sizeof( PER_HANDLE_DATA ) );
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

		�������� ġ�ڸ�
		���࿡ �մ��� ������ �Ǵ� �κ� �Դϴ�.
		*/

		// ����� Ŭ���̾�Ʈ�� ���� ���۸� �����ϰ� OVERLAPPED ����ü ���� �ʱ�ȭ.
		PerIoData = (LPPER_IO_DATA)malloc( sizeof( PER_IO_DATA ) );
		for ( int i = 0; i < MAX_Client; ++i ) {
			if ( client[i] == false ) {
				client[i] = true;
				PerHandleData->client_imei = i;
				printf( "Client ID : %d\n", PerHandleData->client_imei );
				break;
			}
		}
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
		�̺κ��� ��ø����� �κп��� ������Ƿ� �ڼ��� ���������� �ʵ��� �ϰڴ�.
		�ٸ� �����ؼ� ������ ������° ���ڷ� ���޵�PerIoData ����ü�� overlapped �̴�.
		���� �ִٰ� �̺κе� �ٽú��� �ɰ��̴�.
		*/
	}
	return 0;
}

//����� �Ϸῡ ���� �������� �ൿ ����
unsigned int __stdcall CompletionThread( LPVOID pComPort ){
	HANDLE hCompletionPort = (HANDLE)pComPort;
	/*
	���ڷ� ���� pComPort ���� main�� hCompletionPort �� ���� �ȴ�.
	�̴� �����带 ������ main �� hCompletionPort �� ���ڷ� ���� �߱� �����̴�.
	*/

	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	int test_data = 0;

	while ( 1 )
	{
		// 5. ������� �Ϸ�� ������ ���� ����.
		GetQueuedCompletionStatus( hCompletionPort,    // Completion Port
			&BytesTransferred,   // ���۵� ����Ʈ��
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED ����ü ������.
			INFINITE
		);
		/*
		ù��° ���ڷ� ���޵� hCompletionPort �ǿ���� ���ϵ� �߿�
		������� �Ϸ��� ������ �ִٸ� ���� �Ѵ�.
		�׷��� �ʴٸ� ������� �Ϸᰡ �ɶ����� ��ٸ���.
		�̶� ����° ���ڷ� ���޵� PerHandleData ��������� �Ϸ�� ������
		hCompletionPort �� ���� �Ҷ� ���� ����° ���ڷ� �����ߴ� Ŭ���̾�Ʈ ������ ���� �ȴ�.
		�������� �ٽú��� ������ ����.
		�׹�° ���ڷ� ���޵� PerIoData ����Send ��Recv �ÿ� �����ߴ�
		overapped ����ü ������ �����͸� ��� ���ػ��ȴ�.

		���� �ö� Recv �Լ� ȣ��� ��� overlapped ����ü ������ �ּҸ� �����ߴ��� ���� ����������.
		���⼭ �츮�� LPPER_IO_DATA ����ü�� overlapped ����ü ������ ���ö� Recv �� �����ߴ�
		������ �����鵵 ���� �޾ƿ��� �ȴ�.
		�̴� �޾ƿ��� �Ǵ� overlapped ����ü �ּҰ� �����δ� LPPER_IO_DATA ����ü�� �ּ��̱� �����̴�.
		LPPER_IO_DATA ����ü�� ����� �ٶ� ���� ���� ���� ��Ų �ɹ��� �������� ���� ������ ����.
		LPPER_IO_DATA ����ü�� �����ּ� �� overlapped ����ü�� �����ּҴ� ���� ������ �����ϴ�.
		*/

		if ( BytesTransferred == 0 ){
			printf( "- Ŭ�� ���� -\n" );
			client[PerHandleData->client_imei] = false;
			closesocket( PerHandleData->hClntSock );
			free( PerHandleData );
			free( PerIoData );
			/*
			Ŭ���̾�Ʈ�� ���� ����� ó���� ���ش�.
			free �� ���ذ� �ȵȴٸ� �����Ҵ��� ���� ������..
			*/
			continue;
		}

		printf( "-- %d --\n", PerIoData->Incoming_data );
		
		if ( PerIoData->Incoming_data == Recv_Mode ) {
			PerIoData->wsaBuf.buf[BytesTransferred] = '\0';
			server_data.player[PerHandleData->client_imei] = (Player_Socket&)PerIoData->buffer;
			printf( "Recv : x : %f, y: %f, z: %f\n", server_data.player[PerHandleData->client_imei].x, server_data.player[PerHandleData->client_imei].y, server_data.player[PerHandleData->client_imei].z );
			PerIoData->Incoming_data = Send_Mode; // �б���� �����͸� �о������, ���� ������� ������ �Ѵ�.
		}

		if ( PerIoData->Incoming_data == Send_Mode ) {
			test_data++;
			
			PerIoData->wsaBuf.len = sizeof( Server_Player );
			PerIoData->wsaBuf.buf = (char*)&server_data;
			PerIoData->Incoming_data = Recv_Mode;

			printf( "Send : ���� ����ü�� ������..!\n");
			WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );
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

void ErrorHandling( char *message )
{
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit( 1 );
}