/* ��� �������� ������ �ʿ��Ҽ��� �־ backup ������ ���ܵ� */
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
DisplayText( hList, "[%s] %s", msg, (char *)lpMsgBuf );
LocalFree( lpMsgBuf );
}

// ����� ���� ������ ���� �Լ�
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
	//	// Send_Mode �� ó�� ������ Ŭ���� ��� ������, ������ȣ �����͸� �Ѱ��ش�.
	//	init_client[PerHandleData->client_imei] = true;
	//	ClientData cd;
	//	cd.client_imei = PerHandleData->client_imei;
	//	cd.team = PerHandleData->client_imei % 2;

	//	PerIoData->wsaBuf.len = sizeof( ClientData );
	//	PerIoData->wsaBuf.buf = (char*)&cd;
	//	PerIoData->Incoming_data = Recv_Mode;

	//	WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );
	//	DisplayText( hList, "%d Ŭ�� ������, ������ȣ�� ���½��ϴ�..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len );
	//}
	//else {
	// �ѹ� �̻� ������ �Ǿ������ ���� ����ü�� ������.

	PerIoData->wsaBuf.len = sizeof( Server_Player );
	PerIoData->wsaBuf.buf = (char*)&server_data;
	PerIoData->Incoming_data = Recv_Mode;

	WSASend( PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL );
	//DisplayText( hList, "%d Ŭ�� ���� �����͸� ���½��ϴ�..! [%d]", PerHandleData->client_imei, PerIoData->wsaBuf.len, PerIoData->wsaBuf.len );
	/*}*/
}