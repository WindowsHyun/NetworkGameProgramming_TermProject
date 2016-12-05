#include "main.h"

// IOCP ���� ���� ����...
SOCKETINFO dataBuf;
int sendBytes = 0;
int recvBytes = 0;
int flags = 0;
WSAEVENT event;
WSAOVERLAPPED overlapped;
ClientData *cdData;
Server_Player * spData;

void ErrorHandling( char *message ) {
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

SOCKET init_sock() {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN recvAddr;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		ErrorHandling( "WSAStartup() error!" );

	hSocket = WSASocket( PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if ( hSocket == INVALID_SOCKET )
		ErrorHandling( "socket() error" );

	memset( &recvAddr, 0, sizeof( recvAddr ) );
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_addr.s_addr = inet_addr( ipAddres );
	recvAddr.sin_port = htons( atoi( "9000" ) );

	if ( connect( hSocket, (SOCKADDR*)&recvAddr, sizeof( recvAddr ) ) == SOCKET_ERROR )
		ErrorHandling( "connect() error!" );


	//����ü�� �̺�Ʈ �ڵ� �����ؼ� ����
	event = WSACreateEvent();
	memset( &overlapped, 0, sizeof( overlapped ) );

	overlapped.hEvent = event;
	dataBuf.Incoming_data = Send_Mode;

	return hSocket;
}

void iocp_Communication( SOCKET sock, Player_Socket &player, ClientData &cd, Server_Player &server_data ) {
	if ( dataBuf.Incoming_data == Send_Mode ) {
		dataBuf.wsabuf.len = sizeof( Player_Socket );
		dataBuf.wsabuf.buf = (char*)&player;
		dataBuf.Incoming_data = Recv_Mode;

		if ( WSASend( sock, &dataBuf.wsabuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL ) == SOCKET_ERROR ) {
			if ( WSAGetLastError() != WSA_IO_PENDING ) {
				MessageBox( NULL, "�������� ������ ���������ϴ�..!\n���� : \n 1. ������ ����Ǿ����ϴ�\n 2. ������ ���� �������� ���Ͽ����ϴ�\n 3. ���ͳ� ������ �ùٸ��� �ʽ��ϴ�", "NetworkTermProject", 0 );
				exit( 1 );
			}
		}

		if ( player.AttackedPlayer != Not_Attacked ) {
			player.AttackedPlayer = Not_Attacked;
		}
		////���ۿϷ�Ȯ��
		//WSAWaitForMultipleEvents( 1, &event, TRUE, WSA_INFINITE, FALSE );  // ���������� �������� Ȯ��.  
		//																   //���۵ȹ���Ʈ��Ȯ��
		//WSAGetOverlappedResult( sock, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL ); // ������ ���۵� ����Ʈ �� �� ����.
	}

	if ( dataBuf.Incoming_data == Recv_Mode ) {

		memset( &(overlapped), 0, sizeof( OVERLAPPED ) );
		dataBuf.wsabuf.len = BUFSIZE;
		dataBuf.wsabuf.buf = dataBuf.buf;
		dataBuf.Incoming_data = Send_Mode;

		if ( WSARecv( sock, &dataBuf.wsabuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL ) == SOCKET_ERROR ) {
			if ( WSAGetLastError() != WSA_IO_PENDING ) {
				MessageBox( NULL, "�������� ������ ���������ϴ�..!\n���� : \n 1. ������ ����Ǿ����ϴ�\n 2. ������ ���� �������� ���Ͽ����ϴ�\n 3. ���ͳ� ������ �ùٸ��� �ʽ��ϴ�", "NetworkTermProject", 0 );
				exit( 1 );
			}
		}

		// �������� Ŭ����� ������ ������ �����´�.
		dataBuf.wsabuf.buf[recvBytes] = '\0';
		spData = (Server_Player*)dataBuf.wsabuf.buf;
		server_data = *spData;

		if ( cd.client_imei == -1 ) {
			// ����ȣ�� �����غ���...
			for ( int i = 0; i < MAX_Client; ++i ) {
				if ( !strcmp( server_data.Players[i].nickName, nickName ) ) {
					cd.client_imei = i;
					cd.team = (bool)(i % 2);
					player.team = (bool)(i % 2);
					printf( "Client_imei : %d, Team : %d, nickName : %s\n", cd.client_imei, cd.team, nickName );
					break;
				}
			}
		}

		// �������� ���� ������ �÷��̾ �־��ش�.
		player.hp = server_data.Players[cd.client_imei].hp;
		player.live = server_data.Players[cd.client_imei].live;
		player.kill = server_data.Players[cd.client_imei].kill;
		player.death = server_data.Players[cd.client_imei].death;

	}


}