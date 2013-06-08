#include <windows.h>
#include <process.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

HWND hwnd;

static int _redraw;
static int _close;
static int _Field[6][7] = {{0,0,0,0,0,0,0}, // _Field[y][x]
                           {0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0},
                           {0,0,0,0,0,0,0},
                           {0,0,1,2,0,0,0},
                          };
static int _CurrentPlayer;  // 1, 2
static int _RoundCount;     // <= 42;





void DrawBoard(void) {
  RECT        rect ;
  HBRUSH      hbr, hbrold; // Handle auf Farb-Pinsel
  HDC         hdc;
  PAINTSTRUCT ps;

  hdc = BeginPaint (hwnd, &ps);
  GetClientRect (hwnd, &rect);

  int x, y;
  int width, height;
  int left, top;
  int fieldwidth, fieldheight;
  
  width = rect.right - rect.left-10;
  height = rect.bottom-rect.top-10;
  
  hbr = (HBRUSH) GetStockObject (BLACK_BRUSH); // liefert ein Handle auf einen schwarzen Pinsel 
  hbrold = (HBRUSH) SelectObject (hdc, hbr);
  Rectangle(hdc, 10, 10, width, height);

  fieldwidth = (width / 7) - 3;
  fieldheight = (height / 6) - 3;

  hbr = (HBRUSH) CreateSolidBrush (RGB(0, 0, 255));
  hbrold = (HBRUSH) SelectObject (hdc, hbr);

  left = 10;
  top = 10;
  for(x = 0; x < 7; x++) {
	  for(y = 0; y < 6; y++) {

      switch (_Field[y][x]) {
      case 0:
        hbr = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0xFF, 0xFF));
        break;
      case 1:
        hbr = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0, 0));
        break;
      case 2:
        hbr = (HBRUSH) CreateSolidBrush (RGB(0, 0xFF, 0));
        break;
      }
      hbrold = (HBRUSH) SelectObject (hdc, hbr);

      Ellipse(hdc, left, top, left+fieldwidth, top+fieldheight);
      top += fieldheight;
    }
    top = 10;
    left += fieldwidth;
  }


  EndPaint(hwnd, &ps);
}

VOID ThreadGUI(PVOID pvoid) {
  do {
    if(_redraw == 1) {
      DrawBoard();
      _redraw = 0;
    }
    Sleep(10);
  } while (_close != 1);
}

VOID ThreadGAME(PVOID pvoid) {
  do {

  } while (_close != 1);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC         hdc ;
  PAINTSTRUCT ps ;
  RECT        rect ;


  switch (message) {
  case WM_LBUTTONDOWN:
    
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



  hwnd = CreateWindow (
    szAppName,                          // Name der Fensterklasse
    L"Erstes Windows Programm",  	      // Fenstertitel
    WS_OVERLAPPEDWINDOW,                // Fensterstil
    CW_USEDEFAULT,                      // X-Position des Fensters
    CW_USEDEFAULT,                      // Y-Position des Fensters
    CW_USEDEFAULT,                      // Fensterbreite
    CW_USEDEFAULT,                      // Fensterhoehe
    NULL,                               // Uebergeordnetes Fenster
    NULL,                               // Menue
    hInstance,                          // Programm-ID
    NULL                                // zusaetzliche Parameter
  ); 

  ShowWindow (hwnd, iCmdShow);
  UpdateWindow (hwnd);
  
  _beginthread(ThreadGUI,0,NULL);
  _beginthread(ThreadGAME,0,NULL);

  while (GetMessage (&msg, NULL, 0, 0)) {
    TranslateMessage (&msg);
    DispatchMessage (&msg);
  }
  return msg.wParam ;
}s