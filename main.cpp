/********************************************************************
*   File: MAIN.cpp
* 
* Purpose: 
*   Application entry,
*   includes message loop, main thread and GUI thread
*
*/

#define MAIN                                                          // Indicate globals to be included here

#include "MAIN.h"

/*********************************************************************
*
*       ThreadGUI()
*
* Function description:
*   GUI thread to redraw the board if neccessary.
*
*
*/
VOID ThreadGUI(PVOID pvoid) {
  do {
    if(_redraw == 1) {
      DrawBoard();
      _redraw = 0;
    }
    Sleep(10);
  } while (_close != 1);
}

/*********************************************************************
*
*       Start()
*
* Function description:
*   Initializes all variables on start of new game
*   Starts GUI thread, if not already done
*   Updates window to paint the board
*
*/
void Start() {
  if (_GUIStarted == 0) {
    _beginthread(ThreadGUI,0,NULL);	
	  _GUIStarted = 1;
  }
  memset(_Field, 0,sizeof(_Field));
  _close         = 0;
  _CurrentPlayer = 1;
  _Started       = 1;
  _RoundCount    = 0;
  _acMessage[0] = 0;
  //
  // Force the window to send an WM_PAINT message immediately
  //
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);
}

/*********************************************************************
*
*       WndProc()
*
* Function description:
*   Callback function for main thread
*   Handles all messages and calls functionalities
*
*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  int r;
  int pX;
  int pY;

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
        _beginthread(ThreadNET,0,(void *) NET_MODE_SERVER);	
        break;
      case ID_NEWGAME_PvP_NET_Client:
        //
        // Player 2 is always the client, therfore we block the ability to set
        // tiles, as the server (Player 1) is always first.
        //
        _GameModus = 3;
        NET_TurnComplete = 1;
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
      if (_Started == 0) {                                            // Check if a game is started
        return 0;
      }
	    if (_GameModus == 1) {                                          // Check if we play "hot seat" (two players, one computer)
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
	    } else if (_GameModus == 2) {                                   // Check if we play vs a AI
        r = GetField(GET_X_LPARAM(lParam));
        switch (r) {
        case 0:
          SwitchPlayer();
          //
          // We have switched to the AI,
          // the AI will attempt to perform a valid move.
          //
          if (CheckNumInARow(3, &pX, &pY, _CurrentPlayer)){         // May 'I' win?
            r = SetTile(pX);
          } else if (CheckNumInARow(3,  &pX,  &pY, 1)) {            // Can the enemy win?
            r = SetTile(pX);
          } else if (Check2Tiels3Spaces(&pX, 1)) {                  // Can the enemy create a pat situation by creating a 3 tile row with free fields to either side?
            r = SetTile(pX);
          } else if (CheckNumInARow(2,  &pX,  &pY, 2)) {            // May I create a row of 3?
            r = SetTile(pX);
          } else if(CheckNumInARow(1, &pX,  &pY, 2)) {              // May I create a row of 2?
            r = SetTile(pX);
          } else {                                                  // Set random tile
            srand(_RoundCount);
	          pX = rand() % FIELD_X;
            r = SetTile(	rand() % FIELD_X);
          }
          do {
            if (r == -1) {
              srand(_RoundCount);
	            pX = rand() % FIELD_X;
              r = SetTile(	rand() % FIELD_X);      
            }              
          } while(r == -1);
          switch (IncreaseRoundCntCheckEnd(pX, r)) {
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
            NET_TurnComplete = 1;
            NET_EndWinFlag |= _CurrentPlayer;
            DisplayWin();
            break;
          case 2:
            NET_TurnComplete = 1;
            NET_EndWinFlag |= NET_END_FLAG_VAL;
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

/*********************************************************************
*
*       WinMain()
*
* Function description:
*   Application entry
*   Create window,
*   run message loop
*
*/
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
  _Window.width = 735;
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
