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
int _GetField(int PosX) {
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
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 2;
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 3;
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 4;
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 5;
  } else  if (PosX <= ColumWidth * 2) {
    FieldX = 6;
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
  };
  if (CheckEnd()) {                // No free coin field remaining?
    return 2;
  }
  return 0;  
}


int SetTile(int FieldX) {
  int FieldY;
  int left;
  int top;
  HDC         hdc;
  HBRUSH hEmptyField;
  HBRUSH hPlayerField;
  PAINTSTRUCT ps;

  hdc = BeginPaint(hwnd, &ps);

  hEmptyField = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0xFF, 0xFF));
  switch(_CurrentPlayer) {
  case 1:
    hPlayerField = (HBRUSH) CreateSolidBrush (RGB(0xFF, 0, 0));
    break;
  case 2:
    hPlayerField = (HBRUSH) CreateSolidBrush (RGB(0, 0xFF, 0));
    break;
  }
  
  left = BORDER_BOARD + ((_TileSize + BORDER_TILE*2) * FieldX);
  top =  BORDER_BOARD;
  FieldY = 0;
  do {
    SelectObject (hdc, hPlayerField);
    Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);

    Sleep(300);

    SelectObject(hdc, hEmptyField);
    Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);

    top += _TileSize + BORDER_TILE*2;
    FieldY++;
  } while (_Field[FieldY][FieldX] == 0);

  SelectObject (hdc, hPlayerField);
  Ellipse(hdc, left, top, left+_TileSize, top+_TileSize);

  FieldY--;


  EndPaint(hwnd, &ps);
  return FieldY;
}

void DisplayWin(void) {
  HDC         hdc;
  PAINTSTRUCT ps;

  hdc = BeginPaint (hwnd, &ps);

  //
  // Display a win message
  //



  EndPaint(hwnd, &ps);
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
  Rectangle(hdc, BORDER_BOARD, BORDER_BOARD, _Board.width, _Board.height);

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
    top = BORDER_BOARD;
    left += _TileSize + BORDER_TILE*2;
  }
  EndPaint(hwnd, &ps);
}




/******* EOF *******************************************************/