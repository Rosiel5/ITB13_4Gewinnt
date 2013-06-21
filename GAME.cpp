/********************************************************************
*   File: GAME.cpp
* 
* Purpose: Provide game logic
*
*/

#include "MAIN.h"

/*********************************************************************
*
*       CheckEnd()
*
* Function description:
*   Check if the board is filled up
*   This is the case after 42 rounds.
*
*/
bool CheckEnd(void) {
  if ((FIELD_X * FIELD_Y) > _RoundCount) {
    return false;
  } else {
    return true;
  }
}

/*********************************************************************
*
*       CheckWin()
*
* Function description:
*   Check if the last set tile forms a row of 4
*   Return true to indicate the current player has won
*
*/
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
  while ((x > 0) && (y != FIELD_Y-1)) {
    x--;
    y++;
  }
  while (((x + 3) < FIELD_X) && ((y - 3) >= 0)) {
    if ((_Field[y    ][x    ] == _CurrentPlayer) &&
        (_Field[y - 1][x + 1] == _CurrentPlayer) &&
        (_Field[y - 2][x + 2] == _CurrentPlayer) &&
        (_Field[y - 3][x + 3] == _CurrentPlayer)
       ) {
      return true;
    }
    x++;
    y--;
  }
  //
  // Check diagonal - right lower to left upper.
  //
  x = NewTileX;
  y = NewTileY;
  while ((x < FIELD_X-1) && (y != FIELD_Y-1)) {
    x++;
    y++;
  }
  while (((x - 3) > 0) && ((y - 3) >= 0)) {
    if ((_Field[y    ][x    ] == _CurrentPlayer) &&
        (_Field[y - 1][x - 1] == _CurrentPlayer) &&
        (_Field[y - 2][x - 2] == _CurrentPlayer) &&
        (_Field[y - 3][x - 3] == _CurrentPlayer)
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
*       IncreaseRoundCntCheckEnd()
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
  if (CheckWin(FieldX, FieldY)) {                                     // Do we have a winner?
    return 1;  
  }
  if (CheckEnd()) {                                                   // No free coin field remaining?
    return 2;
  }
  return 0;  
}

/*************************** End of file ****************************/