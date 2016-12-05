#include "main.h"

// IOCP 관련 전역 변수...
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


	//구조체에 이벤트 핸들 삽입해서 전달
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
				MessageBox( NULL, "서버와의 연결이 끊어졌습니다..!\n이유 : \n 1. 서버가 종료되었습니다\n 2. 서버에 의해 강제퇴장 당하였습니다\n 3. 인터넷 연결이 올바르지 않습니다", "NetworkTermProject", 0 );
				exit( 1 );
			}
		}

		if ( player.AttackedPlayer != Not_Attacked ) {
			player.AttackedPlayer = Not_Attacked;
		}
		////전송완료확인
		//WSAWaitForMultipleEvents( 1, &event, TRUE, WSA_INFINITE, FALSE );  // 데이터전송 끝났는지 확인.  
		//																   //전송된바이트수확인
		//WSAGetOverlappedResult( sock, &overlapped, (LPDWORD)&sendBytes, FALSE, NULL ); // 실제로 전송된 바이트 수 를 얻어낸다.
	}

	if ( dataBuf.Incoming_data == Recv_Mode ) {

		memset( &(overlapped), 0, sizeof( OVERLAPPED ) );
		dataBuf.wsabuf.len = BUFSIZE;
		dataBuf.wsabuf.buf = dataBuf.buf;
		dataBuf.Incoming_data = Send_Mode;

		if ( WSARecv( sock, &dataBuf.wsabuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL ) == SOCKET_ERROR ) {
			if ( WSAGetLastError() != WSA_IO_PENDING ) {
				MessageBox( NULL, "서버와의 연결이 끊어졌습니다..!\n이유 : \n 1. 서버가 종료되었습니다\n 2. 서버에 의해 강제퇴장 당하였습니다\n 3. 인터넷 연결이 올바르지 않습니다", "NetworkTermProject", 0 );
				exit( 1 );
			}
		}

		// 서버에서 클라들의 데이터 정보를 가져온다.
		dataBuf.wsabuf.buf[recvBytes] = '\0';
		spData = (Server_Player*)dataBuf.wsabuf.buf;
		server_data = *spData;

		if ( cd.client_imei == -1 ) {
			// 팀번호를 구별해보자...
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

		// 서버에서 나의 정보를 플레이어에 넣어준다.
		player.hp = server_data.Players[cd.client_imei].hp;
		player.live = server_data.Players[cd.client_imei].live;
		player.kill = server_data.Players[cd.client_imei].kill;
		player.death = server_data.Players[cd.client_imei].death;

	}


}