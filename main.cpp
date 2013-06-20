/********************************************************************
*   File: MAIN.cpp
* 
* Purpose: 
*
*/

#define MAIN

#include "MAIN.h"

VOID ThreadGUI(PVOID pvoid) {
  do {
    if(_redraw == 1) {
      DrawBoard();
      _redraw = 0;
    }
    Sleep(10);
  } while (_close != 1);
}

//
// Initialize all variables and repaint the field
//
void Start() {
  if (_GUIStarted == 0) {
    //
    // Start GUI Thread
    //
    _beginthread(ThreadGUI,0,NULL);	
	  _GUIStarted = 1;
  }
  memset(_Field, 0,sizeof(_Field));
  _close         = 0;
  _Started       = 1;
  _RoundCount    = 0;
  _acMessage[0] = 0;
  //
  // Force the window to send an WM_PAINT message immediately
  //
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int r;
  int RandomFieldX;

  switch (message) {
    case WM_RBUTTONDOWN:
      Start();
    return 0;    
    case WM_CREATE:
      HMENU hMenu, hSubMenu, hSubMenuNET;
	    //
	    // Create handles
	    //
      hMenu       = CreateMenu();
      hSubMenu    = CreatePopupMenu();
      hSubMenuNET = CreatePopupMenu();
	    //
	    // Configure menu entries
	    //
      AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu,       L"N&ew Game");
      AppendMenu(hSubMenu, MF_STRING, ID_NEWGAME_PvsP,              L"Player vs. Player");
      AppendMenu(hSubMenu, MF_STRING, ID_NEWGAME_PvsAI,             L"Player vs. AI");
      AppendMenu(hSubMenu, MF_STRING | MF_POPUP, (UINT)hSubMenuNET, L"Player vs. Player [Network]");
      AppendMenu(hSubMenuNET, MF_STRING, ID_NEWGAME_PvP_NET_Server, L"Start Server");
      AppendMenu(hSubMenuNET, MF_STRING, ID_NEWGAME_PvP_NET_Client, L"Connect to Server");
	    //
	    // Create menu
	    //
      SetMenu(hwnd, hMenu);
    break;
    //
    // WM_COMMAND handles the main menu
    // Here we initialize a new game
    //
    case WM_COMMAND:
      //
      // Set Current Player to the default value of 1, as doing it in Start() would overwrite the setting in NET.
      //
      _CurrentPlayer = 1;
      switch(LOWORD(wParam)) {
        case ID_NEWGAME_PvsP:
          _GameModus = 1;
          break;
        case ID_NEWGAME_PvsAI:
          _GameModus = 2;
		      break;
        case ID_NEWGAME_PvP_NET_Server:
          //
          // Player 1 is always the server.
          //
          _GameModus = 3;
          _CurrentPlayer = 1;
          _beginthread(ThreadNET,0,(void *) NET_MODE_SERVER);	
          break;
        case ID_NEWGAME_PvP_NET_Client:
          //
          // Player 2 is always the client.
          //
          _GameModus = 3;
          _CurrentPlayer = 2;
          _beginthread(ThreadNET,0,(void *) NET_MODE_CLIENT);	
          break;
	    }
      Start();
	  return 0;
    //
    // When we get a WM_LBUTTONDOWN, we have to perform gaming action like
    // Set a tile, calculate winner etc...
    //
    case WM_LBUTTONDOWN:
	  //
	  // Check if a game is started
	  //
    if (_Started == 0) {
      return 0;
    }
	  //
	  // Check if we play "hot seat" (two players, one computer)
	  //
	  if (_GameModus == 1) {
      switch (GetField(GET_X_LPARAM(lParam))) {
        case 0:
          SwitchPlayer();
          break;
        case 1:
          DisplayWin();
          break;
        case 2:
          DisplayEnd();
          break;
        default:
          break;
      }
	  //
	  // Check if we play vs a AI
	  //
	  } else if (_GameModus == 2) {  
      //
      //
      //
      r = GetField(GET_X_LPARAM(lParam));
      switch (r) {
        case 0:
          SwitchPlayer();
          //
          // We have switched to the AI,
          // the AI will attempt to perform a valid move.
          //
          do {
            RandomFieldX = rand() % FIELD_X;
            r = SetTile(RandomFieldX);
          } while (r == -1);
          switch (IncreaseRoundCntCheckEnd(RandomFieldX, r)) {
            case 1:
              DisplayWin();
              break;
            case 2:
              DisplayEnd();
              break;
          }
          SwitchPlayer();
          break;
        case 1:
          DisplayWin();
          break;
        case 2:
          DisplayEnd();
          break;
        default:
        break;
      }
    //
	  // Check if we play via TCP/IP
	  //
	  } else if (_GameModus == 3) {
      if (!NET_TurnComplete && !NET_ServerWaiting) {
        switch (GetField(GET_X_LPARAM(lParam))) {
          case 0:
            NET_TurnComplete = 1;
            break;
          case 1:
            DisplayWin();
            break;
          case 2:
            DisplayEnd();
            break;
          default:
            break;
        }
      }
    }
      return 0;
    case WM_PAINT:
      _redraw = 1;
      return 0;
    case WM_DESTROY:
      _close = 1;
      PostQuitMessage (0);
      return 0;
  }
  return DefWindowProc (hwnd, message, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
  static TCHAR szAppName[] = L"4-Gewinnt";
  MSG          msg ;
  WNDCLASS     wndclass ;
  //
  // Register the window class
  //
  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
  wndclass.lpfnWndProc   = WndProc ;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance ;
  wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject (GRAY_BRUSH );
  wndclass.lpszMenuName  = NULL ;
  wndclass.lpszClassName = szAppName ;
  RegisterClass (&wndclass);
  //
  // Configure windows size and initialize variables by calling Start();
  //
  _Window.width = 725;
  _Window.height = 765;
  _Board.width = 700;
  _Board.height = 600;
  _TileSize = 94;
  //
  // Create window
  //
  hwnd = CreateWindow (
    szAppName,                          // Name der Fensterklasse
    L"4-Gewinnt",                       // Fenstertitel
    WS_OVERLAPPED | WS_SYSMENU,                // Fensterstil
    CW_USEDEFAULT,                      // X-Position des Fensters
    CW_USEDEFAULT,                      // Y-Position des Fensters
    _Window.width,                      // Fensterbreite
    _Window.height,                     // Fensterhoehe
    NULL,                               // Uebergeordnetes Fenster
    NULL,                               // Menue
    hInstance,                          // Programm-ID
    NULL                                // zusaetzliche Parameter
  ); 
  ShowWindow (hwnd, iCmdShow);
  UpdateWindow (hwnd);
  while (GetMessage (&msg, NULL, 0, 0)) {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  return msg.wParam ;
}
/*************************** End of file ****************************/