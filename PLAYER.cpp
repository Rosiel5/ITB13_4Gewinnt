/********************************************************************
*   File: PLAYER.cpp
* 
* Purpose: Provide players fucntions
*
*/

#include "MAIN.h"

void SwitchPlayer(void) {
 
  if (_CurrentPlayer == 2) {
    _CurrentPlayer = 1;
  } else {
    _CurrentPlayer = 2;
  }
}
 
// returns player 1 or 2
int GetPlayer(void) {
  return _CurrentPlayer;
}; 
/*************************** End of file ****************************/