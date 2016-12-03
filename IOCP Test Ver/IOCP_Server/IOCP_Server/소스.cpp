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

typedef struct //소켓정보를구조체화.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
	int client_imei = -1;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct // 소켓의버퍼정보를구조체화.
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
	// 전송을 위해 사용
	float x = -1, y = -1, z = -1;
}Player_Socket;

Player_Socket player;

typedef struct Server_Player {
	//플레이어 데이터를 묶어놓은것
	Player_Socket player[MAX_Client];
}Server_Player;
Server_Player server_data;

bool client[MAX_Client] = { false }; // 클라이언트가 들어오는 대로 처리한다.

/*
소켓 버퍼 정보를 구조체로 만드는데 이때 WSABUF 와 overlapped 를 포함한다.
WSABUF 는WSASend 나WSARecv 함수의 인자로 전달되는 버퍼에 사용되는 구조체 이기에 포함 되고
overlapped 구조체 변수를 넣어주는건 현재 완료된 입출력 정보를 얻어 낼때 사용 된다.
*/

unsigned int __stdcall CompletionThread( LPVOID pComPort );
/*
완료된 쓰레드에 관한 처리를 해주는 함수 이다.
*/

void ErrorHandling( char *message );

int main( int argc, char** argv )
{
	WSADATA wsaData;
	HANDLE hCompletionPort;
	// 만들어질 CompletionPort가 전달될 Handle

	SYSTEM_INFO SystemInfo;
	/*
	시스템 정보가 전달됨 쓰레드를 생성할때 CPU 의 개수에 따라
	쓰레드를 만들어 줘야 하기 때문에 정보를 얻어옴
	*/
	SOCKADDR_IN servAddr;

	LPPER_IO_DATA PerIoData;
	//위에서구조체로만들어준소켓의버퍼정보

	LPPER_HANDLE_DATA PerHandleData;
	//소켓정보가저장될구조체여기서는소켓핸들과주소를가지고있다.

	SOCKET hServSock;
	int RecvBytes;
	int i, Flags;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		ErrorHandling( "WSAStartup() error!" );

	//1. Completion Port 생성.
	hCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 0 );
	/*
	CreateIoCompletionPort 의 첫번째에 해당하며
	Completion Port 를 만들어주는 부분이다.
	은행이야기를 예로 들자면 은행을 만들어 주는 부분이 된다.
	*/

	GetSystemInfo( &SystemInfo );
	/*
	시스템 정보를 얻어온다.
	이는 앞서 말했지만 CPU의 개수만큼 쓰레드를 만들기 위해서 이다
	이렇게 얻어온 시스템 정보는 맴버변수 dwNumberOfProcessors 에CPU의 개수가들어간다.
	*/

	//2. Completion Port 에서 입출력 완료를 대기하는 쓰레드를 CPU 개수만큼 생성.
	for ( i = 0; i < SystemInfo.dwNumberOfProcessors; i++ )
		_beginthreadex( NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL );
	/*
	CPU의 개수만큼 쓰레드를 만들어 주는 부분이다.
	이때 새로 만들어지는 쓰레드에 미리 만들어둔 Completion Port 를전달하는데
	이를통해 unsigned int __stdcall CompletionThread(LPVOID pComPort); 함수의인자로
	쓰레드로 전달한 Completion Port 가전달된다.

	은행을 예로들자면 은행원을 고용하는 부분 입니다.
	은행 크기에 맞게 은행원을 고용하게됩니다
	또한 은행원의 소속을 현재 만들어준 은행이라고 나타내는 부분입니다.
	*/

	hServSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	// 소켓을 만들때는 꼭 중첩 입출력 형식으로 만들어 주어야 한다.
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

		// 연결된클라이언트의소켓핸들정보와주소정보를설정.
		PerHandleData = (LPPER_HANDLE_DATA)malloc( sizeof( PER_HANDLE_DATA ) );
		PerHandleData->hClntSock = hClntSock;
		memcpy( &(PerHandleData->clntAddr), &clntAddr, addrLen );
		/*
		PerHandleData에 연결된 클라이언트의 정보들을 저장한다.
		이때 동적 할당으로 정보를 저장 하는데 동적 할당을 모른다면 공부하고 오도록 하자
		*/

		//3. Overlapped 소켓과 CompletionPort의 연결.
		CreateIoCompletionPort( (HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0 );
		/*
		CreateIoCompletionPort 의 두번째 기능에 해당 한다.
		현재 연결된 클라이언트와 만들어둔 CompletionPort 오브젝트를 연결 해준다.
		세번째 인자로 위에서 클라이언트 정보를 저장했던 PerHandleData를 넘겨 준다.
		이를 통해서 입출력이 완료된 클라이언트의 정보를 얻는것이 가능 하다.
		잠시후 아래에서 이부분을 시보라고 기제하게 될것 이다.

		은행으로 치자면
		은행에 손님이 들어오게 되는 부분 입니다.
		*/

		// 연결된 클라이언트를 위한 버퍼를 설정하고 OVERLAPPED 구조체 변수 초기화.
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
		PerIoData 를 만들어 주고 초기화 합니다.
		이는 소켓 버퍼정보를 지니는 구조체 입니다.
		*/
		Flags = 0;

		//4. 중첩된 데이터입력.
		WSARecv( PerHandleData->hClntSock, // 데이터 입력소켓.
			&(PerIoData->wsaBuf),  // 데이터 입력 버퍼포인터.
			1,       // 데이터 입력 버퍼의 수.
			(LPDWORD)&RecvBytes,
			(LPDWORD)&Flags,
			&(PerIoData->overlapped), // OVERLAPPED 구조체 포인터.
			NULL
		);
		/*
		이부분은 중첩입출력 부분에서 배웠으므로 자세히 설명하지는 않도록 하겠다.
		다만 유의해서 볼점은 여섯번째 인자로 전달된PerIoData 구조체에 overlapped 이다.
		조금 있다가 이부분도 다시보게 될것이다.
		*/
	}
	return 0;
}

//입출력 완료에 따른 쓰레드의 행동 정의
unsigned int __stdcall CompletionThread( LPVOID pComPort ){
	HANDLE hCompletionPort = (HANDLE)pComPort;
	/*
	인자로 받은 pComPort 에는 main의 hCompletionPort 가 전달 된다.
	이는 쓰레드를 생성시 main 의 hCompletionPort 를 인자로 전달 했기 때문이다.
	*/

	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	int test_data = 0;

	while ( 1 )
	{
		// 5. 입출력이 완료된 소켓의 정보 얻음.
		GetQueuedCompletionStatus( hCompletionPort,    // Completion Port
			&BytesTransferred,   // 전송된 바이트수
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED 구조체 포인터.
			INFINITE
		);
		/*
		첫번째 인자로 전달된 hCompletionPort 의연결된 소켓들 중에
		입출력을 완료한 소켓이 있다면 리턴 한다.
		그렇지 않다면 입출력의 완료가 될때까지 기다린다.
		이때 세번째 인자로 전달된 PerHandleData 은입출력이 완료된 소켓이
		hCompletionPort 와 연결 할때 같이 세번째 인자로 전달했던 클라이언트 정보가 전달 된다.
		위에가서 다시보고 오도록 하자.
		네번째 인자로 전달된 PerIoData 에는Send 나Recv 시에 전달했던
		overapped 구조체 변수의 포인터를 얻기 위해사용된다.

		위로 올라가 Recv 함수 호출시 어떻게 overlapped 구조체 변수의 주소를 전달했는지 보고 오도록하자.
		여기서 우리가 LPPER_IO_DATA 구조체로 overlapped 구조체 정보를 얻어올때 Recv 시 전달했던
		데이터 정보들도 같이 받아오게 된다.
		이는 받아오게 되는 overlapped 구조체 주소가 실제로는 LPPER_IO_DATA 구조체의 주소이기 때문이다.
		LPPER_IO_DATA 구조체를 만들어 줄때 가장 먼저 포함 시킨 맴버가 무엇인지 보고 오도록 하자.
		LPPER_IO_DATA 구조체의 시작주소 와 overlapped 구조체의 시작주소는 같기 때문에 가능하다.
		*/

		if ( BytesTransferred == 0 ){
			printf( "- 클라 종료 -\n" );
			client[PerHandleData->client_imei] = false;
			closesocket( PerHandleData->hClntSock );
			free( PerHandleData );
			free( PerIoData );
			/*
			클라이언트의 연결 종료시 처리를 해준다.
			free 가 이해가 안된다면 동적할당을 배우고 오도록..
			*/
			continue;
		}

		printf( "-- %d --\n", PerIoData->Incoming_data );
		
		if ( PerIoData->Incoming_data == Recv_Mode ) {
			PerIoData->wsaBuf.buf[BytesTransferred] = '\0';
			server_data.player[PerHandleData->client_imei] = (Player_Socket&)PerIoData->buffer;
			printf( "Recv : x : %f, y: %f, z: %f\n", server_data.player[PerHandleData->client_imei].x, server_data.player[PerHandleData->client_imei].y, server_data.player[PerHandleData->client_imei].z );
			PerIoData->Incoming_data = Send_Mode; // 읽기모드로 데이터를 읽어왔으니, 이제 쓰기모드로 변경을 한다.
		}

		if ( PerIoData->Incoming_data == Send_Mode ) {
			test_data++;
			
			PerIoData->wsaBuf.len = sizeof( Server_Player );
			PerIoData->wsaBuf.buf = (char*)&server_data;
			PerIoData->Incoming_data = Recv_Mode;

			printf( "Send : 서버 구조체를 보낸다..!\n");
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