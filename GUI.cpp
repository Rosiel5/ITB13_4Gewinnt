/********************************************************************
*   File: GUI.cpp
* 
* Purpose: 
*
*/

#include "MAIN.h"

extern HWND hwnd;

/********************************************
//
//  _GetField()
//
//  return values:
//     0 succes (stone is set)
//    -1 selectet row is full
//     1 win
//     2 end
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
  //
  // When the stone is set we have to check for win, end and increment the round count.
  //
  _RoundCount++;
  if (CheckWin(FieldX, FieldY)) {  // Do we have a winner?
    return 1;  
  }
  if (CheckEnd()) {                // No free coin field remaining?
    return 2;
  }
  return 0;  
}


int SetTile(int FieldX) {
  int FieldY;
  int left;
  int top;

  HDC     hdc;
  HBRUSH  hEmptyTile;
  HBRUSH  hPlayer;
  
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
  top =  BORDER_BOARD + BORDER_TILE;
  FieldY = 0;
  while (_Field[FieldY][FieldX] == 0 && FieldY < 6) {
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
  top -= _TileSize + BORDER_TILE*2;
  SelectObject (hdc, hPlayer);
  Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);
  ReleaseDC(hwnd, hdc);

  return FieldY;
}

void DisplayWin(void) {
  HDC         hdc;

  _Started = 0;
  hdc = GetDC(hwnd);
  TextOut(hdc, _Window.width/3, _Window.height/3, L"Ein Sieg! Ein Sieg! Ein Sieg!", 29);
  ReleaseDC(hwnd, hdc);
}

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
  Rectangle(hdc, rect.left + BORDER_BOARD, rect.top + BORDER_BOARD, rect.left + _Board.width + BORDER_BOARD, rect.top + _Board.height + BORDER_BOARD);
  left = BORDER_BOARD + BORDER_TILE;
  top =  BORDER_BOARD + BORDER_TILE;
  for(x = 0; x < 7; x++) {      // Draw the row
	  for(y = 0; y < 6; y++) {    // Draw the column
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
    top = BORDER_BOARD + BORDER_TILE;
    left += _TileSize + BORDER_TILE*2;
  }
  EndPaint(hwnd, &ps);
}


static int _ChangeSize;

void SetWindowSize(void) {
  RECT rect;
  int width;
  int height;

  GetClientRect (hwnd, &rect);
  width  = rect.right  - rect.left;
  height = rect.bottom - rect.top;
  //
  // Does the width of the windows has been changed?
  //
  if (_Board.width != width) {
    _Board.width  = width;
    _Board.height = ((width) / 7 ) * 6;
    _TileSize = ((width - BORDER_BOARD*2) / 7) - 6;
  } else if (_Board.height != height) {
    _Board.height = height;
    _Board.width  = ((height) / 6) * 7;
    _TileSize = ((height - BORDER_BOARD*2) / 6) - 6;
  }

}

void CalculateField(void) {}
void DisplayEnd(void) {
  _Started = 0;
}

/******* EOF *******************************************************/