/********************************************************************
*   File: GUI.cpp
* 
* Purpose: 
*
*/

#include "MAIN.h"


/*********************************************************************
*
*       _GetField()
*
*  Function description
*
*
*  Return values
*     0 succes (stone is set)
*    -1 selectet row is full
*     1 win
*     2 end
*/
int GetField(int PosX) {
  int ColumWidth;
  int FieldX;
  int FieldY;
 
  //
  // Calculate X field number
  //
  ColumWidth = _TileSize + BORDER_TILE*2;
  if (PosX <= ColumWidth * 1) {
    FieldX = 0;
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 1;
  } else  if (PosX <= ColumWidth * 3) {
    FieldX = 2;
  } else  if (PosX <= ColumWidth * 4) {
    FieldX = 3;
  } else  if (PosX <= ColumWidth * 5) {
    FieldX = 4;
  } else  if (PosX <= ColumWidth * 6) {
    FieldX = 5;
  } else  if (PosX <= ColumWidth * 7) {
    FieldX = 6;
  } else {
    return -1;
  }
  //
  // Call animation function which returns Y field number
  //
  FieldY = SetTile(FieldX);
  if (FieldY < 0) {                // Column full?
    return -1;
  }
  return IncreaseRoundCntCheckEnd(FieldX, FieldY);
}


/*********************************************************************
*
*       SetTile
*
*  Function description
*
*
*  Return values
*    0:   
*/
int SetTile(int FieldX) {
  int FieldY;
  int left;
  int top;

  HDC     hdc;
  HBRUSH  hEmptyTile;
  HBRUSH  hPlayer;
  
  //
  // Check whether the column is full.
  //
  for (FieldY = 0; FieldY < FIELD_Y; FieldY++) {
    if (_Field[FieldY][FieldX] == 0) {
      break;
    }
  }
  if (FieldY == FIELD_Y && _Field[FieldY-1][FieldX] != 0) {
    return -1;
  }

  hEmptyTile = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0xFF, 0xFF));
  switch (_CurrentPlayer) {
    case 1:
      hPlayer = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0, 0));
      break;
    case 2:
      hPlayer = (HBRUSH) CreateSolidBrush (RGB(0, 0xFF, 0));
      break;
  }
  hdc = GetDC(hwnd);
  left = BORDER_BOARD + BORDER_TILE + ((_TileSize + BORDER_TILE*2) * FieldX);
  top =  BORDER_BOARD + BORDER_TOP + BORDER_TILE;
  FieldY = 0;
  while (_Field[FieldY][FieldX] == 0 && FieldY < FIELD_Y) {
    SelectObject (hdc, hPlayer);
    Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);
    Sleep(50);
    SelectObject (hdc, hEmptyTile);
    Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);
    top += _TileSize + BORDER_TILE*2;
    FieldY++;
  }
  FieldY--;
  _Field[FieldY][FieldX] = _CurrentPlayer;
  //top -= _TileSize + BORDER_TILE*2;
  //SelectObject (hdc, hPlayer);
  //Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);
  ReleaseDC(hwnd, hdc);
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);

  return FieldY;
}

void DisplayWin(void) {
  sprintf_s(_acMessage, "Player %d has won. Start a new game for revange.", _CurrentPlayer);
  _Started = 0;
  //
  // Force the window to send an WM_PAINT message immediately
  //
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);
}

void DisplayEnd(void) {
  sprintf_s(_acMessage, "The game has ended after %d moves. Start a new game for revange.", _RoundCount);
  _Started = 0;
  //
  // Force the window to send an WM_PAINT message immediately
  //
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);
}

void SetMessage(const char* acIn) {
  sprintf_s(_acMessage, acIn);
  //
  // Force the window to send an WM_PAINT message immediately
  //
  InvalidateRect(hwnd, NULL,NULL);
  UpdateWindow(hwnd);
}

void ClearMessage(void) {
  _acMessage[0] = 0;
}

/*********************************************************************
*
*       DrawBoard
*
*  Function description
*    
*
*/
void DrawBoard(void) {
  HBRUSH    hField;
  HBRUSH    hEmptyTile;
  HBRUSH    hPlayerOne;
  HBRUSH    hPlayerTwo;
  HDC         hdc;
  PAINTSTRUCT ps;
  int left;
  int top;
  int x;
  int y;

  RECT rect;

  GetClientRect(hwnd, &rect);
  hdc = BeginPaint (hwnd, &ps);
  //
  // Set brush colors
  //
  hField = (HBRUSH) CreateSolidBrush (RGB(0, 0, 0xFF));
  hEmptyTile = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0xFF, 0xFF));
  hPlayerOne = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0, 0));
  hPlayerTwo = (HBRUSH) CreateSolidBrush (RGB(0, 0xFF, 0));
  //
  // Draw the field background
  //
  SelectObject (hdc, hField);
  Rectangle(hdc, rect.left + BORDER_BOARD, rect.top + BORDER_BOARD + BORDER_TOP, rect.left + _Board.width + BORDER_BOARD, rect.top + _Board.height + BORDER_BOARD + BORDER_TOP);
  //
  // Draw the status messages
  //
  SelectObject (hdc, hEmptyTile);
  Rectangle(hdc, rect.left + BORDER_BOARD, rect.top + BORDER_BOARD, rect.left + _Board.width + BORDER_BOARD, rect.top + BORDER_BOARD + BORDER_TOP);
  if (_acMessage[0] != 0) {
    TextOutA(hdc, rect.left+30, rect.top+20, _acMessage, strlen(_acMessage));
  } else {
    sprintf_s(_acMessage, "Player %d! It is your turn.", _CurrentPlayer);
    TextOutA(hdc, rect.left+30, rect.top+20, _acMessage, strlen(_acMessage));
    _acMessage[0] = 0;
  }
  //
  //Draw the tiles
  //
  left = BORDER_BOARD + BORDER_TILE;
  top =  BORDER_BOARD + BORDER_TOP + BORDER_TILE;
  for(x = 0; x < FIELD_X; x++) {      // Draw the row
	  for(y = 0; y < FIELD_Y; y++) {    // Draw the column
      switch (_Field[y][x]) {
        case 0:
          SelectObject (hdc, hEmptyTile);
          break;
        case 1:
          SelectObject (hdc, hPlayerOne);
          break;
        case 2:
          SelectObject (hdc, hPlayerTwo);
          break;
      }
      Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);
      top += _TileSize + BORDER_TILE*2;
    }
    top = BORDER_BOARD + BORDER_TOP + BORDER_TILE;
    left += _TileSize + BORDER_TILE*2;
  }
  EndPaint(hwnd, &ps);
}

/*************************** End of file ****************************/