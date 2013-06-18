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

VOID ThreadNET(PVOID pvoid) {
  WORD         wVersionRequested;
  WSADATA      wsaData;
  SOCKET       Sock = 0;
  sockaddr_in  Addr;
  sockaddr     RemoteAddr;
  int          AddrLen;
  char         Mode;
  int          r;

  //
  // Initialize winsock. Required to use TCP/IP.
  //
  wVersionRequested = MAKEWORD(2, 0);
  if (WSAStartup(wVersionRequested, &wsaData) != 0) {
    DisplayErrorMessageBox(MB_OK, L"WSAStartup: Could not init WinSock.");
    goto CleanAndExit;
  }
  //
  // Create a sockt, we need one in any case.
  //
  Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (Sock == -1) {
    DisplayErrorMessageBox(MB_OK, L"socket(): Could not create socket");
    goto CleanAndExit;
  }
  //
  // Check wether we have to act as a server or as a client.
  //
  Mode = (char)pvoid;
  if (Mode == NET_MODE_SERVER) {
    Addr.sin_family      = AF_INET;
    Addr.sin_port        = htons(5555);
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
    //
    // Since accept is blocking and the possibility for the user 
    // to cancel the server mode should still exist a global variable is
    // used in order to signal the main thread to display the appropriate
    // message box and to wait for user input.
    //
    NET_ServerWaiting = 1; // @TODO no idea how to do this cleanly, could just call it from the gui thread
    AddrLen = sizeof(RemoteAddr);
    r = accept(Sock, &RemoteAddr, &AddrLen);
    if (r < 0) {
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
          send(Sock, (char *)&_Field, (sizeof(_Field) * sizeof(_Field[0]) * sizeof(int)), NULL);
          break;
        case 2:
          recv(Sock, (char *)&_Field, (sizeof(_Field) * sizeof(_Field[0]) * sizeof(int)), NULL);
          break;
      }
    } while (r > 0);

    
  } else if (Mode == NET_MODE_CLIENT) {
    Mode--;
  }
CleanAndExit:
  closesocket(Sock);
  WSACleanup();
}


/*************************** End of file ****************************/