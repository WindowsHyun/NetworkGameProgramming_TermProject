#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdlib.h>
#include <atlstr.h> // cstring
#include <Windows.h>
#include <process.h> // 프로세스 개수를 가져온다.
#include "resource.h"
#include "Server.h"

bool client[MAX_Client] = { false }; // 클라이언트가 들어오는 대로 처리한다.
bool init_client[MAX_Client] = { false }; // 클라 처음 시작하는지 안하는지를 판단한다.
char buf[MAX_PATH];
int aTeam = 0, bTeam = 0;
int defaultRoundTime = 60;

// 윈도우 프로시저
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
void CALLBACK TimerProc( HWND, UINT, UINT, DWORD ); //함수 선언


void DisplayText( HWND hDlg, char *fmt, ... );
void DisplayEditText( HWND hDlg, char *fmt, ... );
void clrUser( int ); // 유저가 나가거나 강퇴시 정보를 지운다.
void Kill_Death( int select ); // kd초기화 or 계산

SOCKET sock; // 소켓
HWND shareHwnd, hList, hTextBox[3];
HFONT hFont[3];
HWND hPlayer[8], hPlayerOff[8];
HWND Time_Button[6];
SYSTEM_INFO SystemInfo;

Server_Player server_data;

DWORD WINAPI ProcessClient( LPVOID arg ); // 클라 접속을 위한 스레드
unsigned int __stdcall CompletionThread( LPVOID pComPort );


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	// 소켓 통신 스레드 생성
	CreateThread( NULL, 0, ProcessClient, NULL, 0, NULL );

	SetTimer( shareHwnd, 1, 1024, TimerProc );

	// 대화상자 생성
	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DlgProc );

	// closesocket()
	closesocket( sock );

	// 윈속 종료
	WSACleanup();
	return 0;
}

void CALLBACK TimerProc( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime ) {   //함수 정의
	HDC hdc;
	hdc = GetDC( hWnd );
	if ( server_data.gr.round_wait == true ) {
		if ( server_data.gr.time_wait <= 0 ) {
			// wait 가 모두 끝났을경우
			server_data.gr.round_start = true;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
		}
		server_data.gr.time_wait -= 1;
		DisplayEditText( hTextBox[2], "게임시작 까지 = %d : %d", server_data.gr.time_wait / 60, server_data.gr.time_wait % 60 );
	}

	if ( server_data.gr.round_start == true ) {
		if ( server_data.gr.round_time <= 0 ) {
			// 라운드가 끝났을경우 다음 라운드로 넘겨준다..!
			server_data.gr.round_start = false;
			server_data.gr.GameRound += 1;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
			Kill_Death( 0 );
			Kill_Death( 1 );
		}
		server_data.gr.round_time -= 1;
		if ( server_data.gr.round_time % 60 < 10 ) {
			DisplayEditText( hTextBox[2], "남은시간 = %d : 0%d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );
		}
		else {
			DisplayEditText( hTextBox[2], "남은시간 = %d : %d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );
		}
	}
	else {
		// 라운드가 false일경우 1초마다 체크를 해준다.
		int user = 0;
		for ( int i = 0; i < MAX_Client; ++i ) {
			if ( server_data.player[i].Game_Play == true )
				user++;
		}
		if ( user >= 2 )
			server_data.gr.round_wait = true; // 참여자가 2명 이상이면 time_wait를 true로 하여 전체 대기를 진행한다.
	}
	ReleaseDC( hWnd, hdc );
}


BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	shareHwnd = hDlg;
	switch ( uMsg ) {
	case WM_INITDIALOG:
		//hProgress = GetDlgItem( hDlg, IDC_PROGRESS1 );
		hFont[0] = CreateFont( 20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋음" );
		hFont[1] = CreateFont( 30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋음" );
		hFont[2] = CreateFont( 35, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, "돋음" );

		hList = GetDlgItem( hDlg, IDC_LIST1 );
		hTextBox[0] = GetDlgItem( hDlg, IDC_EDIT1 );
		hTextBox[1] = GetDlgItem( hDlg, IDC_EDIT2 );
		hTextBox[2] = GetDlgItem( hDlg, IDC_EDIT3 );

		Time_Button[0] = GetDlgItem( hDlg, IDC_BUTTON18 ); // 1분
		Time_Button[1] = GetDlgItem( hDlg, IDC_BUTTON19 ); // 2분
		Time_Button[2] = GetDlgItem( hDlg, IDC_BUTTON20 ); // 3분
		Time_Button[3] = GetDlgItem( hDlg, IDC_BUTTON21 ); // 4분
		Time_Button[4] = GetDlgItem( hDlg, IDC_BUTTON22 ); // 5분
		Time_Button[5] = GetDlgItem( hDlg, IDC_BUTTON23 ); // Next Round

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
		//SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)"추가한다." );

		// 줄구분 할때는 : \r\n
		SendMessage( hTextBox[0], WM_SETFONT, (WPARAM)hFont[0], (LPARAM)TRUE );
		SendMessage( hTextBox[1], WM_SETFONT, (WPARAM)hFont[1], (LPARAM)TRUE );
		SendMessage( hTextBox[2], WM_SETFONT, (WPARAM)hFont[2], (LPARAM)TRUE );
		DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
		DisplayEditText( hTextBox[1], "%d vs %d", aTeam, bTeam );
		DisplayEditText( hTextBox[2], "남은시간 = %d : %d", server_data.gr.round_time / 60, server_data.gr.round_time % 60 );

		DisplayText( hList, "%d Thread Created..!", SystemInfo.dwNumberOfProcessors * 2 );
		SendMessage( hList, LB_ADDSTRING, 0, (LPARAM)"Server is Ready..!" );

		return TRUE;


	case WM_COMMAND:

		switch ( LOWORD( wParam ) ) {
		case IDC_LIST1:
			switch ( HIWORD( wParam ) ) {
			case LBN_SELCHANGE:
				int listIndex = SendMessage( hList, LB_GETCURSEL, 0, 0 ); //현재의 리스트 번호를 구한다.
				SendMessage( hList, LB_GETTEXT, listIndex, (LPARAM)buf ); // 구한 번호의 리스트의 내용을 str에 복사한다.
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

		case  IDC_BUTTON18:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 60;
			defaultRoundTime = 60;
			return TRUE;
		case IDC_BUTTON19:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 120;
			defaultRoundTime = 120;
			return TRUE;
		case IDC_BUTTON20:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 180;
			defaultRoundTime = 180;
			return TRUE;
		case IDC_BUTTON21:
			server_data.gr.exit_Round = false;
			server_data.gr.round_time = 240;
			defaultRoundTime = 240;
			return TRUE;
		case IDC_BUTTON22:
			server_data.gr.exit_Round = false;
			server_data.gr.round_start = false;
			server_data.gr.GameRound += 1;
			server_data.gr.round_wait = false;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			Kill_Death( 1 );
			return TRUE;
		case IDC_BUTTON23:
			// 라운드 강제 종료
			server_data.gr.GameRound = 1;
			server_data.gr.round_start = false;
			server_data.gr.round_wait = false;
			server_data.gr.exit_Round = true;
			server_data.gr.time_wait = Game_WaitTime;
			server_data.gr.round_time = defaultRoundTime;
			Kill_Death( 0 );
			Kill_Death( 1 );
			DisplayEditText( hTextBox[0], "Round : %d", server_data.gr.GameRound );
			DisplayEditText( hTextBox[2], "라운드 종료..!" );
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
	// 만들어질 CompletionPort가 전달될 Handle

	/*
	시스템 정보가 전달됨 쓰레드를 생성할때 CPU 의 개수에 따라
	쓰레드를 만들어 줘야 하기 때문에 정보를 얻어옴
	*/
	SOCKADDR_IN servAddr;

	LPPER_IO_DATA PerIoData;
	//위에서구조체로만들어준소켓의버퍼정보

	LPPER_HANDLE_DATA PerHandleData;
	//소켓정보가저장될구조체여기서는소켓핸들과주소를가지고있다.

	int RecvBytes;
	int i, Flags;

	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 ) /* Load Winsock 2.2 DLL */
		DisplayText( hList, "WSAStartup() error!" );

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

	//2. Completion Port 에서 입출력 완료를 대기하는 쓰레드를 CPU 개수 * 2 만큼 생성.
	for ( i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++ )
		_beginthreadex( NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL );
	/*
	CPU의 개수만큼 쓰레드를 만들어 주는 부분이다.
	이때 새로 만들어지는 쓰레드에 미리 만들어둔 Completion Port 를전달하는데
	이를통해 unsigned int __stdcall CompletionThread(LPVOID pComPort); 함수의인자로
	쓰레드로 전달한 Completion Port 가전달된다.
	*/
	bool flag = TRUE;
	sock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	// 소켓을 만들때는 꼭 중첩 입출력 형식으로 만들어 주어야 한다.
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

		DisplayText( hList, "클라이언트 접속: IP 주소=%s, 포트 번호=%d", inet_ntoa( clntAddr.sin_addr ), ntohs( clntAddr.sin_port ) );

		// 연결된클라이언트의소켓핸들정보와주소정보를설정.
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
		*/

		// 연결된 클라이언트를 위한 버퍼를 설정하고 OVERLAPPED 구조체 변수 초기화.
		PerIoData = (LPPER_IO_DATA)malloc( sizeof( PER_IO_DATA ) );
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
		다만 유의해서 볼점은 여섯번째 인자로 전달된PerIoData 구조체에 overlapped 이다.
		조금 있다가 이부분도 다시보게 될것이다.
		*/
	}
	return 0;
}

unsigned int __stdcall CompletionThread( LPVOID pComPort ) {
	HANDLE hCompletionPort = (HANDLE)pComPort;
	/*
	인자로 받은 pComPort 에는 main의 hCompletionPort 가 전달 된다.
	이는 쓰레드를 생성시 main 의 hCompletionPort 를 인자로 전달 했기 때문이다.
	*/
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	Player_Socket temp;
	int sendBytes = 0, player_hp, Attacked_Player;
	int player_kill, player_death;
	bool player_live;

	//WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );

	while ( 1 ) {
		GetQueuedCompletionStatus( hCompletionPort,    // Completion Port
			&BytesTransferred,   // 전송된 바이트수
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED 구조체 포인터.
			INFINITE
		);


		//DisplayText( hList, "%d 사이즈가 도착하였습니다..!", BytesTransferred );
		if ( BytesTransferred == 0 || client[PerHandleData->client_imei] == false ) { //EOF 전송시.
			 // ↓ 서버에서 클라정보를 지운다.
			clrUser( PerHandleData->client_imei );
			client[PerHandleData->client_imei] = false;
			init_client[PerHandleData->client_imei] = false;
			PerIoData->Incoming_data = Recv_Mode;
			// ↓ GUI버튼을 변경한다.
			sprintf( buf, "Player %d : ", PerHandleData->client_imei );
			SendMessage( hPlayer[PerHandleData->client_imei], WM_SETTEXT, 0, (LPARAM)buf );
			// ↓ Display 해준다.
			DisplayText( hList, "%d번 클라이언트가 강제 종료 or 퇴장 되었습니다..!", PerHandleData->client_imei );
			closesocket( PerHandleData->hClntSock );
			free( PerHandleData );
			free( PerIoData );
			continue;
		}

		// 클라이언트 구조체를 받아온다.
		//DisplayText( hList, "%d번 클라이언트 Mode : %d", PerHandleData->client_imei, PerIoData->Incoming_data );
		if ( PerIoData->Incoming_data == Recv_Mode ) {
			PerIoData->wsaBuf.buf[BytesTransferred] = '\0';

			player_hp = server_data.player[PerHandleData->client_imei].hp;
			player_live = server_data.player[PerHandleData->client_imei].live;
			player_kill = server_data.player[PerHandleData->client_imei].kill;
			player_death = server_data.player[PerHandleData->client_imei].death;

			temp = (Player_Socket&)PerIoData->buffer;

			if ( temp.AttackedPlayer >= 19930617 || temp.AttackedPlayer <= -5 ) {
				// 값이 정상적으로 안들어왔을경우 다시 recv_mode로 들어간다.
				DisplayText( hList, "%d 클라이언트 쓰레기값 출연..!", PerHandleData->client_imei );
				PerIoData->Incoming_data = Send_Mode;
			}
			else {
				server_data.player[PerHandleData->client_imei] = (Player_Socket&)PerIoData->buffer;

				server_data.player[PerHandleData->client_imei].live = true;
				server_data.player[PerHandleData->client_imei].team = (bool)(PerHandleData->client_imei % 2);


				if ( init_client[PerHandleData->client_imei] == false ) {
					server_data.player[PerHandleData->client_imei].hp = 100;
					server_data.player[PerHandleData->client_imei].live = true;
				}
				else {
					server_data.player[PerHandleData->client_imei].hp = player_hp;
					server_data.player[PerHandleData->client_imei].live = player_live;
					server_data.player[PerHandleData->client_imei].kill = player_kill;
					server_data.player[PerHandleData->client_imei].death = player_death;
				}

				if ( server_data.player[PerHandleData->client_imei].RespawnTime == Respawn_Complete ) {
					server_data.player[PerHandleData->client_imei].hp = 100;
					server_data.player[PerHandleData->client_imei].live = true;
				}

				if ( server_data.player[PerHandleData->client_imei].AttackedPlayer != Not_Attacked ) {
					// 플레이어가 죽였을경우
					Attacked_Player = server_data.player[PerHandleData->client_imei].AttackedPlayer;
					server_data.player[Attacked_Player].hp -= 40;
					if ( server_data.player[Attacked_Player].hp <= 0 ) {
						server_data.player[PerHandleData->client_imei].kill += 1;
						server_data.player[Attacked_Player].death += 1;
						server_data.player[Attacked_Player].hp = 0;
						server_data.player[Attacked_Player].RespawnTime = 10;
						server_data.player[Attacked_Player].live = false;
						Kill_Death( 0 );
					}
				}


				if ( init_client[PerHandleData->client_imei] == false ) {
					// 처음 접속후 데이터를 받았다. 버튼을 플레이어 이름으로 변경하자.
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
			//DisplayText( hList, "%d 클라 서버 데이터를 보냈습니다..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len, PerIoData->wsaBuf.len );
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

// 리스트에 바로 나올수 있게 작업을 해보자
void DisplayText( HWND hDlg, char *fmt, ... ) {
	va_list arg;
	va_start( arg, fmt );

	char contentBuf[MAX_PATH]; //sprintf 등을 사용할때 쓰자
	vsprintf( contentBuf, fmt, arg );

	SendMessage( hDlg, LB_ADDSTRING, 0, (LPARAM)contentBuf );
}

void DisplayEditText( HWND hDlg, char *fmt, ... ) {
	va_list arg;
	va_start( arg, fmt );

	char contentBuf[MAX_PATH]; //sprintf 등을 사용할때 쓰자
	vsprintf( contentBuf, fmt, arg );

	SendMessage( hDlg, WM_SETTEXT, 0, (LPARAM)contentBuf );
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
	server_data.player[client_imei].Game_Play = false;
	server_data.player[client_imei].team = false;
	server_data.player[client_imei].character_down_state = 0;
	strcpy( server_data.player[client_imei].nickName, "" );
}

void Kill_Death( int select ) {
	switch ( select )
	{
	case 0:
		// A팀 계산
		aTeam = 0;
		for ( int i = 0; i < MAX_Client / 2; ++i ) {
			aTeam += server_data.player[i * 2].kill;
		}
		// B팀 계산
		bTeam = 0;
		for ( int i = 0; i < MAX_Client / 2; ++i ) {
			bTeam += server_data.player[(i * 2) + 1].kill;
		}
		DisplayEditText( hTextBox[1], "%d vs %d", aTeam, bTeam );
		if ( aTeam > bTeam ) {
			server_data.gr.game_result = 0;
		}
		else {
			server_data.gr.game_result = 1;
		}
		break;

	case 1:
		// kd 초기화
		for ( int i = 0; i < MAX_Client; ++i ) {
			server_data.player[i].kill = 0;
			server_data.player[i].death = 0;
		}
		Kill_Death( 0 );
		break;

	default:
		break;
	}

}