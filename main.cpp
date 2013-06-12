/********************************************************************
*   File: MAIN.cpp
* 
* Purpose: 
*
*/

#define MAIN

#include "MAIN.h"

#include <windowsx.h>
#include <process.h>

VOID ThreadGUI(PVOID pvoid) {
  do {
    if(_redraw == 1) {
      DrawBoard();
      _redraw = 0;
    }
    Sleep(10);
  } while (_close != 1);
}

void Start() {

}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

  switch (message) {
    
  case WM_LBUTTONDOWN:
    if (_Started == 0) {
      return 0;
    }
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
    return 0;
    
  case WM_PAINT:
    _redraw = 1;
    return 0;
  case WM_SIZE:
    RECT rect;
    //SetWindowSize();
    GetClientRect (hwnd, &rect);
    //SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.left+_Board.width, rect.top+_Board.height, 0);
    return TRUE;
  case WM_SIZING:
    //SetWindowSize();
    return TRUE;

  case WM_DESTROY:
    _close = 1;
    PostQuitMessage (0);
    return 0;
  }
  return DefWindowProc (hwnd, message, wParam, lParam);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
  static TCHAR szAppName[] = L"HalloWindows";
  MSG          msg ;
  WNDCLASS     wndclass ;

  wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
  wndclass.lpfnWndProc   = WndProc ;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance ;
  wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
  wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH );
  wndclass.lpszMenuName  = NULL ;
  wndclass.lpszClassName = szAppName ;

  RegisterClass (&wndclass);

  _Window.width = 800;
  _Window.height = 700;
  _Board.width = 700;
  _Board.height = 600;
  _TileSize = 94;
  _CurrentPlayer = 1;
  _RoundCount = 0;
  _Started = 1;

  hwnd = CreateWindow (
    szAppName,                          // Name der Fensterklasse
    L"Erstes Windows Programm",  	      // Fenstertitel
    WS_OVERLAPPEDWINDOW,                // Fensterstil
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
  
  _beginthread(ThreadGUI,0,NULL);
  //SetWindowSize();
  //CalculateField();
  Start();

  while (GetMessage (&msg, NULL, 0, 0)) {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  return msg.wParam ;
}