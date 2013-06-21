/********************************************************************
*   File: NET.cpp
* 
* Purpose: 
*
*/
#define WIN32_LEAN_AND_MEAN
#include "MAIN.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/*********************************************************************
*
*       _Displayerror()
*
*/
static void _DisplayError(char Type) {
  TCHAR        ac[100];
  if (Type == 1) {
    wsprintf(ac, L"send function failed with error: %ld\n", WSAGetLastError());
  } else {
    wsprintf(ac, L"recv function failed with error: %ld\n", WSAGetLastError());
  }
  DisplayErrorMessageBox(MB_OK, ac);
}

/*********************************************************************
*
*       _Receive()
*
*/
static int _Receive(SOCKET Sock, char * acBuf, int NumBytes){
  int r;
  // @TODO Status bar: waiting for other player
  r = recv(Sock, acBuf, NumBytes, NULL);
  if (r < 0) {
    _DisplayError(0);
    return -1;
  }
  _RoundCount++;
  //
  // The other player has also finished his turn, reset the flag now.
  //
  SwitchPlayer();
  NET_TurnComplete = 0;
  return r;
}

/*********************************************************************
*
*       _HandleWait()
*
*/
static int _HandleWait(SOCKET Sock, char * acBuf, int NumBytes) {
  int r;
  r = _Receive(Sock, acBuf, NumBytes);
  memcpy(_Field, &acBuf[1], sizeof(_Field));
  if ((acBuf[0] & ~NET_END_FLAG_VAL) > 0) {
    NET_EndWinFlag = acBuf[0] & ~NET_END_FLAG_VAL;
    DisplayWin();
    return -1;
  } else if ((acBuf[0] & NET_END_FLAG_VAL) > 0) {
    DisplayEnd();
    return -1;
  } else {
    //
    // Redraw the field according to the new array.
    //
    InvalidateRect(hwnd, NULL,NULL);
    UpdateWindow(hwnd);
  }
  return r;
}

/*********************************************************************
*
*       _Send()
*
*/
static int _Send(SOCKET Sock, char * acBuf, int NumBytes){
  int r;
  //
  // The first value deceides whether we are transfering data or a control message
  //
  r = send(Sock, acBuf, NumBytes, NULL);
  if (r < 0) {
    _DisplayError(1);
    return -1;
  }
  SwitchPlayer();
  return r;
}

/*********************************************************************
*
*       _NET_Discover()
*
* Function description:
*   Function responsible for the initial communication using
*   UDP broadcast.
*
*   Return values:
*      0: Success
*   != 0: Error
*/
static int _NET_Discover(sockaddr_in * RemoteAddr) {
  SOCKET          Sock = 0;
  char            Opt;
  char            acBuf[9] = "Discover";
  int             Len;
  int             r;
  sockaddr_in     LAddr;
  sockaddr_in     BAddr;

  //
  // Create a socket with broadcast capabilities.
  //
  Sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (Sock == -1) {
    DisplayErrorMessageBox(MB_OK, L"socket(): Could not create socket");
    return -1;
  }
  Opt = 1;
  setsockopt(Sock, SOL_SOCKET, SO_BROADCAST, &Opt, sizeof(Opt));
  //
  // Bind the socket
  //
  memset(&LAddr, 0, sizeof(LAddr));
  LAddr.sin_family      = AF_INET;
  LAddr.sin_port        = htons(NET_UDP_PORT);
  LAddr.sin_addr.s_addr = INADDR_ANY;
  r = bind(Sock, (SOCKADDR *)&LAddr, sizeof(LAddr));
  if (r < 0) {
    _DisplayError(1);
    DisplayErrorMessageBox(MB_OK, L"bind(): Could not bind.");
    return -1;
  }
  //
  // Send the Discover.
  //
  BAddr.sin_family = AF_INET;
  BAddr.sin_port = htons(NET_UDP_PORT);
  BAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
  r = sendto(Sock, acBuf, sizeof(acBuf), 0, (SOCKADDR*)&BAddr, sizeof(BAddr));
  if (r < 0) {
    _DisplayError(1);
    return r;
  }
  
  BAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //
  // There seems to be some strange loopback issue on our development machine (and others?)
  // where we will receive the sent packet (sendto) in our recvfrom routine.
  // To avoid this we check the message.
  //
  do {
    memset(acBuf, 0, sizeof(acBuf));
    Len = sizeof(BAddr);
    r = recvfrom(Sock, &acBuf[0], sizeof(acBuf), 0, (SOCKADDR*)&BAddr, &Len);
  } while(memcmp(acBuf, "Discover\0", sizeof(acBuf)) == 0);

  if (r < 0 || memcmp(acBuf, "Serverxx\0", sizeof(acBuf))) {
    return -1;
  } else {
    memcpy( RemoteAddr, &BAddr, Len);
  }
  return 0;
}

/*********************************************************************
*
*       ThreadNET()
*
* Function description:
*   Thread for network communication and handling
*   in network game modus
*
*/
VOID ThreadNET(PVOID pvoid) {
  WORD            wVersionRequested;
  WSADATA         wsaData;
  SOCKET          Sock = 0;
  SOCKET          DiscoverSock = 0;
  struct timeval  tVal;
  fd_set          readfds;
  sockaddr_in     Addr;
  sockaddr_in     RemoteAddr;
  char *          acBuf;
  int             BufLen;
  int             AddrLen;
  char            Mode;
  char            acBufDiscover[9];
  int             Len;
  int             r;

  //
  // Initialize winsock. Required to use TCP/IP.
  //
  wVersionRequested = MAKEWORD(2, 0);
  if (WSAStartup(wVersionRequested, &wsaData) != 0) {
    DisplayErrorMessageBox(MB_OK, L"WSAStartup: Could not init WinSock.");
    goto CleanAndExit;
  }
  //
  // Create a socket, we need one in any case.
  //
  Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (Sock == -1) {
    DisplayErrorMessageBox(MB_OK, L"socket(): Could not create socket");
    goto CleanAndExit;
  }
  BufLen = sizeof(_Field) + 1;
  acBuf = new char[BufLen];
  memset(acBuf, 0, BufLen);
  //
  // Check wether we have to act as a server or as a client.
  //
  Mode = (char)pvoid;
  if (Mode == NET_MODE_SERVER) {
    //
    // Handle server tasks, starting with a UDP socket for the Discover.
    //
    DiscoverSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (DiscoverSock == -1) {
      DisplayErrorMessageBox(MB_OK, L"socket(): Could not create socket");
      goto CleanAndExit;
    }
    RemoteAddr.sin_family = AF_INET;
    RemoteAddr.sin_port = htons(NET_UDP_PORT);
    RemoteAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    r = bind(DiscoverSock, (SOCKADDR *) &RemoteAddr, sizeof (RemoteAddr));
    //
    // Set the timeout for select()
    //
    tVal.tv_sec  = NET_DISCOVER_TIMEOUT_S;
    tVal.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(DiscoverSock, &readfds);
    //
    // Wait for data until we hit the timeout
    //
    SetMessage("Waiting for clients.....");
    r = select(0, &readfds, NULL, NULL, &tVal);
    if (r > 0) {
      Len = sizeof(RemoteAddr);
      r = recvfrom(DiscoverSock, acBufDiscover, sizeof(acBufDiscover), 0, (SOCKADDR*)&RemoteAddr, &Len);
    } else {
      goto CleanAndExit;
    }
    if (memcmp(acBufDiscover, "Discover\0", sizeof(acBufDiscover)) == 0) {
      memcpy(acBufDiscover, "Serverxx\0", sizeof(acBufDiscover));
      r = sendto(DiscoverSock, acBufDiscover, sizeof(acBufDiscover), 0, (SOCKADDR*)&RemoteAddr, sizeof(RemoteAddr));
    } else {
      goto CleanAndExit;
    }
    ClearMessage();
    Addr.sin_family      = AF_INET;
    Addr.sin_port        = htons(NET_TCP_PORT);
    Addr.sin_addr.s_addr = INADDR_ANY;
    r = bind(Sock, (struct sockaddr *)&Addr, sizeof(Addr));
    if (r < 0) {
      DisplayErrorMessageBox(MB_OK, L"bind(): Could not bind.");
      goto CleanAndExit;
    }
    r = listen(Sock, 1);
    if (r < 0) {
      DisplayErrorMessageBox(MB_OK, L"listen(): Listen failed.");
      goto CleanAndExit;
    }
    NET_ServerWaiting = 1;
    AddrLen = sizeof(RemoteAddr);
    Sock = accept(Sock, (struct sockaddr *)&RemoteAddr, &AddrLen);
    NET_ServerWaiting = 0;
    if (Sock == SOCKET_ERROR) {
      DisplayErrorMessageBox(MB_OK, L"accept(): Accept failed.");
      goto CleanAndExit;
    }
    //
    // Connection established, we are ready to send and receive data from the client.
    //
    do {
      //
      // It is defined that Player 1 is the host (server), while player 2 is the client.
      //
      switch (_CurrentPlayer) {
        case 1:
          //
          // Wait until we receive a turn complete signal in the form of a global
          // variable being set to 1.
          //
          while (!NET_TurnComplete) {
            Sleep(50);
          }
          memset(acBuf, 0, BufLen);
          acBuf[0] |= NET_EndWinFlag;
          memcpy(&acBuf[1], _Field, sizeof(_Field));
          r = _Send(Sock, acBuf, BufLen);
          break;
        case 2:
          r = _HandleWait(Sock, acBuf, BufLen);
          break;
      }
    } while (r > 0);

    
  } else if (Mode == NET_MODE_CLIENT) {
    SetMessage("Discovering  a server on the network....");
    r = _NET_Discover(&Addr);
    if (r != 0) {
      SetMessage("A server was not found.");
      goto CleanAndExit;
    }
    ClearMessage();
    Addr.sin_port = htons(NET_TCP_PORT);
    r = connect(Sock, (SOCKADDR *) &Addr, sizeof (Addr));
    if (r == SOCKET_ERROR) {
        DisplayErrorMessageBox(MB_OK, L"accept(): Accept failed.");
        goto CleanAndExit;
    }
    //
    // Connection established, we are ready to send and receive data from the server.
    //
    do {
      //
      // It is defined that Player 1 is the host (server), while player 2 is the client.
      //
      switch (_CurrentPlayer) {
        case 1:
          r = _HandleWait(Sock, acBuf, BufLen);
          break;
        case 2:
          //
          // Wait until we receive a turn complete signal in the form of a global
          // variable being set to 1.
          //
          while (!NET_TurnComplete) {
            Sleep(50);
          }
          memset(acBuf, 0, BufLen);
          acBuf[0] |= NET_EndWinFlag;
          memcpy(&acBuf[1], _Field, sizeof(_Field));
          r = _Send(Sock, acBuf, BufLen);
          break;
      }
    } while (r > 0);
  }
CleanAndExit:
  delete acBuf;
  closesocket(Sock);
  WSACleanup();
}


/*************************** End of file ****************************/