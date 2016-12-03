#include "main.h"
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
	printf( "[%s] %s", msg, (char *)lpMsgBuf );
	LocalFree( lpMsgBuf );
}

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

SOCKET init_sock() {
	int retval;
	int send_buf = 50;
	int rcv_buf = 300;
	bool flag = TRUE;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 ) {
	}
	// socket()
	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock == INVALID_SOCKET ) err_quit( "socket()" );
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof( rcv_buf ) );
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, (char*)&send_buf, sizeof( send_buf ) );
	setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof( flag ) );

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr( ipAddres );
	serveraddr.sin_port = htons( SERVERPORT );

	retval = connect( sock, (SOCKADDR *)&serveraddr, sizeof( serveraddr ) );
	if ( retval == SOCKET_ERROR ) err_quit( "connect()" );

	printf( "[�˸�] %s:%d ���������� ���� �Ǿ����ϴ�..!\n", ipAddres, SERVERPORT );
	return sock;
}

int get_ClientID( SOCKET sock ) {
	int retval;
	int len;
	retval = recvn( sock, (char *)&len, sizeof( int ), 0 ); // ������ �ޱ�(���� ����)
	if ( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}
	else if ( retval == 0 ) {
	}

	char *buf = new char[len]; // ���۵� ���̸� �˰� ������ ũ�⿡ ���缭 buf�� �÷�����!

	retval = recvn( sock, buf, len, 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}

	if ( atoi( buf ) == -1 ) {
		MessageBox( NULL, "������ �ο��� �� á���ϴ�..!", "�˸�", 0 );
		//err_quit( "���� �ź�" );
		exit( 1 );
		//return -1;
	}
	else {
		printf( "[�˸�] Client ID : %s\n", buf );

		return atoi( buf );
	}
}

void send_Player( SOCKET sock, Player_Socket player ) {
	int retval;
	// ������ ������( ����ü ũ�⸦ ���� ������. )
	int len = sizeof( player );
	retval = send( sock, (char *)&len, sizeof( int ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		exit( 1 );
	}
	//printf( "Send : %d\n", len );

	// ������ ������( ����ü �����͸� ������. )
	retval = send( sock, (char*)&player, sizeof( Player_Socket ), 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "send()" );
		exit( 1 );
	}

}

Server_Player recv_Player( SOCKET sock ) {
	int retval;
	int len;
	retval = recvn( sock, (char *)&len, sizeof( int ), 0 ); // ������ �ޱ�(���� ����)
	if ( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}
	else if ( retval == 0 ) {
	}

	//printf( "Recv : %d\n", len );
	//printf( "������ ũ�� : %d\n", len );

	int GetSize;
	char suBuffer[BUFSIZE];
	Server_Player *player;
	GetSize = recv( sock, suBuffer, len, 0 );
	if ( GetSize == SOCKET_ERROR ) {
		MessageBox( NULL, "�������� ������ ���������ϴ�..!\n���� : \n 1. ������ ����Ǿ����ϴ�\n 2. ������ ���� �������� ���Ͽ����ϴ�\n 3. ���ͳ� ������ �ùٸ��� �ʽ��ϴ�", "NetworkTermProject", 0 );
		exit( 1 );
	}

	suBuffer[GetSize] = '\0';
	player = (Server_Player*)suBuffer;

	//printf( "%d\n", player[0]->camxrotate );


	return *player;
}

int get_ClientTeam( SOCKET sock ) {
	int retval;
	int len;
	retval = recvn( sock, (char *)&len, sizeof( int ), 0 ); // ������ �ޱ�(���� ����)
	if ( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}
	else if ( retval == 0 ) {
	}

	char *buf = new char[len]; // ���۵� ���̸� �˰� ������ ũ�⿡ ���缭 buf�� �÷�����!

	retval = recvn( sock, buf, len, 0 );
	if ( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}

	if ( atoi( buf ) == -1 ) {
		MessageBox( NULL, "������ �ο��� �� á���ϴ�..!", "�˸�", 0 );
		//err_quit( "���� �ź�" );
		exit( 1 );
		//return -1;
	}
	else {
		printf( "[�˸�] Client Team : %s\n", buf );

		return atoi( buf );
	}
}

//void send_nickName( SOCKET sock ) {
//	int retval;
//	// ������ ������( ����ü ũ�⸦ ���� ������. )
//	int len = sizeof( nickName );
//	retval = send( sock, (char *)&len, sizeof( int ), 0 );
//	if ( retval == SOCKET_ERROR ) {
//		err_display( "send()" );
//		exit( 1 );
//	}
//
//	// ������ ������( ����ü �����͸� ������. )
//	retval = send( sock, (char*)&nickName, sizeof( nickName ), 0 );
//	if ( retval == SOCKET_ERROR ) {
//		err_display( "send()" );
//		exit( 1 );
//	}
//
//}
