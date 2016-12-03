/* 사용 안하지만 언젠가 필요할수도 있어서 backup 용으로 남겨둠 */
// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

// 사용자 정의 데이터 수신 함수
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

//-------------------------------------------------------------------------------------------

if ( PerIoData->Incoming_data == Send_Mode ) {
	//if ( init_client[PerHandleData->client_imei] == false ) {
	//	// Send_Mode 중 처음 접속한 클라의 경우 팀정보, 고유번호 데이터를 넘겨준다.
	//	init_client[PerHandleData->client_imei] = true;
	//	ClientData cd;
	//	cd.client_imei = PerHandleData->client_imei;
	//	cd.team = PerHandleData->client_imei % 2;

	//	PerIoData->wsaBuf.len = sizeof( ClientData );
	//	PerIoData->wsaBuf.buf = (char*)&cd;
	//	PerIoData->Incoming_data = Recv_Mode;

	//	WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );
	//	DisplayText( hList, "%d 클라 팀정보, 고유번호를 보냈습니다..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len );
	//}
	//else {
	// 한번 이상 전송이 되었을경우 서버 구조체를 보낸다.

	PerIoData->wsaBuf.len = sizeof( Server_Player );
	PerIoData->wsaBuf.buf = (char*)&server_data;
	PerIoData->Incoming_data = Recv_Mode;

	WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );
	//DisplayText( hList, "%d 클라 서버 데이터를 보냈습니다..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len, PerIoData->wsaBuf.len );
	/*}*/
}