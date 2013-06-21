/********************************************************************
*   File: PLAYER.cpp
* 
* Purpose: Provide players functions
*
*/

#include "MAIN.h"

/*********************************************************************
*
*       SwitchPlayer()
*
* Function description:
*   Switch the current player between player 1 and player 2
*
*
*/
void SwitchPlayer(void) {
 
  if (_CurrentPlayer == 2) {
    _CurrentPlayer = 1;
  } else {
    _CurrentPlayer = 2;
  }
}
 
/*********************************************************************
*
*       GetPlayer()
*
* Function description:
*   Returns the current player
*
*
*/
int GetPlayer(void) {
  return _CurrentPlayer;
}
/*************************** End of file ****************************/