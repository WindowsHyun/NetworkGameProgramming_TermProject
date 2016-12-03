# NetworkGameProgramming_TermProject

----------
1. 애플리케이션 기획
----------
저희 팀이 만들게임은 OpenGL 기반의 3D FPS/TPS 게임이며 4:4 멀티플레이를 지원합니다. 서버는 TCP 방식의 멀티스레드 서버입니다.

간단하게 클라이언트와 서버의 흐름을 설명해드리겠습니다.
우선 서버를 실행시키면 윈속 초기화, listen 소켓생성, listen 소켓의 지역 IP주소와 포트번호 결정, listen 소켓의 포트상태를 LISTENING으로 변경 하는 클라이언트와 통신을 하기 위한 기본 작업들을 하고 클라이언트의 신호를 기다립니다.

플레이어가 클라이언트를 실행시키면 자동으로 서버로 접속을 시도합니다. 서버의 accept 함수를 통해 신호를 받고 클라이언트를 위한 스레드를 생성해줍니다.
접속에 성공한 클라이언트는 A팀 B팀 선택을 할 수 있고 선택한 정보를 서버로 전송하면 서버에서 선택한 팀의 인원수를 카운트하고 본격적으로 게임 데이터를 주고받으며 게임을 진행합니다.

게임이 시작되면 서버는 플레이어들의 위치와 공격(사격)에 대한 정보를 수집하고 충돌체크를 처리합니다. 별다른 입력이 없어도 플레이어들의 위치와 상태정보는 끊임없이 클라이언트와 주고받으며, 공격(사격)에 관한 데이터를 받으면 충돌체크를 한 후 플레이어 사망 시 사망 정보를 전송합니다.

아래는 먼저 개발했던 게임의 스크린샷을 첨부하였습니다.

![Image01](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname01.png)

3인칭 구현 및 UI, 미니맵 구현

![Image02](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname02.png)

1인칭화면에서 조준 한 상태

![Image03](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname03.png)

전체 맵을 촬영한 이미지

이전에 개발한 부분에서 오류가 많이 발생했던 충돌체크부분과 맵 구조부분을 이번 네트워크 게임 프로그래밍 프로젝트에 맞게 수정할 계획입니다.

클라이언트 수정 부분에서 많은 시간을 빼앗길 것 같아 많은 기능을 넣지 않았으나 개발 일정에 따라 클라이언트의 로비구현, 채팅창 구현, 게임 승패 구현 등을 할 계획입니다.

----------
2. High-Level 디자인
----------

![Image04](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname04.png)

----------
3. Low-Level 디자인
----------
3-1. Low-Level 디자인 - Client
----------

![Image05](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname05.png)

----------
3-2. Low-Level 디자인 – Server
----------

![Image06](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname06.png)

----------
3-3. Low-Level 디자인 – 패킷
----------

![Image07](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname07.png)

----------
4. 팀원별 역할 분담
----------

![Image08](https://github.com/WindowsHyun/NetworkGameProgramming_TermProject/blob/master/Document/image/noname08.png)

----------
5. 개발 환경
----------
   + Visual Studio 2015
   + openGL
   + Photoshop
   - WinSock
   - Windows OS
