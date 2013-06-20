/********************************************************************
*   File: GAME.cpp
* 
* Purpose: Provide game logic
*
*/

#include "MAIN.h"


//
// Process the static field array and check if there
// is space left to place tiles.
//
bool CheckEnd(void) {
  if ((FIELD_X * FIELD_Y) > _RoundCount) {
    return false;
  } else {
    return true;
  }
}

//
// Process the static field array and check if there
// are 4 tiles of same colour in a row.
//
bool CheckWin(int NewTileX, int NewTileY) {
  int y;
  int x;
  
  //
  // Check vertical.
  //
  for (y = 0; y < FIELD_Y-3; y++) {
    if ((_Field[y    ][NewTileX] == _CurrentPlayer) &&
        (_Field[y + 1][NewTileX] == _CurrentPlayer) &&
        (_Field[y + 2][NewTileX] == _CurrentPlayer) &&
        (_Field[y + 3][NewTileX] == _CurrentPlayer)
       ) {
      return true;
    }
  }
  
  //
  // Check horizontal.
  //
  for (x = 0; x < FIELD_X-3; x++) {
    if ((_Field[NewTileY][x    ] == _CurrentPlayer) &&
        (_Field[NewTileY][x + 1] == _CurrentPlayer) &&
        (_Field[NewTileY][x + 2] == _CurrentPlayer) &&
        (_Field[NewTileY][x + 3] == _CurrentPlayer)
       ) {
      return true;
    }
  }
  //
  // Check diagonal - left lower to right upper.
  //
  x = NewTileX;
  y = NewTileY;
  while ((x != 0) && (y != 0)) {
    x--;
    y--;
  }
  //
  // Check the diagonal by going from left to right.
  //
  while (((x + 3) < FIELD_X) && ((y + 3) < FIELD_Y)) {
    if ((_Field[y    ][x    ] == _CurrentPlayer) &&
        (_Field[y + 1][x + 1] == _CurrentPlayer) &&
        (_Field[y + 2][x + 2] == _CurrentPlayer) &&
        (_Field[y + 3][x + 3] == _CurrentPlayer)
       ) {
      return true;
    }
    x++;
    y++;
  }
  //
  // Check diagonal - right upper to left lower.
  //
  x = NewTileX;
  y = NewTileY;
  while ((x < FIELD_X) && (y != 0)) {
    x++;
    y--;
  }
  //
  // Check the diagonal by going from right to left.
  //
  while ((x > 0) && ((y + 3) < FIELD_Y)) {
    if ((_Field[y    ][x    ] == _CurrentPlayer) &&
        (_Field[y + 1][x - 1] == _CurrentPlayer) &&
        (_Field[y + 2][x - 2] == _CurrentPlayer) &&
        (_Field[y + 3][x - 3] == _CurrentPlayer)
       ) {
      return true;
    }
    x--;
    y++;
  }
  return false;
}

/*********************************************************************
*
*       IncreaseRoundCntCheckEnd
*
*  Function description:
*    Increases the static round count and checks if the game is 
*    finished or if there is a winner.
*
*  Parameters:
*    FieldX - Command that shall be executed.
*    FieldY - Pointer to command specific pData.
*
*  Return value:
*    0 - Neither condition has occured.
*    1 - Win
*    2 - End
*
*  Note:
*  This function is global because it is also called from 
*  the WndProc callback when AI is used.
*/
int IncreaseRoundCntCheckEnd(int FieldX, int FieldY) {
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

/*************************** End of file ****************************/