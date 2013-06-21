/********************************************************************
*   File: PLAYER.cpp
* 
* Purpose: Provide players fucntions
*
*/

#include "MAIN.h"

/*********************************************************************
*
*       CheckIf4TiesCanBeSetCrossLineRightLeft()
*
* Function description:
*   This function checks weather theare are enough spaces to 
*   be able to win the game (set 4ties in a cross row)
*   We can be sure, that xPos is the "outtest" tie
*   This means for left to right check: the riggest one
*               right to left check: the leftest one
*
*/
int CheckIf4TiesCanBeSetCrossLineRightLeft(int xPos, int yPos, int RowWithSpace, int CurrentPlayer) {
  int cnt = 0;
  int OppositePlayer;
  int SpacePosY = 0;
  int SpacePosX = 0;
  int FirstTieY = 0;
  int FirstTieX = 0;

  if (CurrentPlayer==1) {
    OppositePlayer = 2;
  } else {
    OppositePlayer = 1;
  }
  //
  // Determine first tie
  //
  do {
    xPos++;
    yPos++;
    //
    // Check if we are out of the board
    //
    if(yPos >= 6)  {
      xPos--;
      yPos--;
      FirstTieX = xPos;
      FirstTieY = yPos;
      break;
    }
    if(_Field[yPos][xPos] != CurrentPlayer) {
      if(RowWithSpace && (_Field[yPos][xPos] == 0)) {
        RowWithSpace = 0;
        SpacePosY = yPos;
        SpacePosX = xPos;
      } else {
        //
        // break if we have reached the first tie
        //
        xPos--;
        yPos--;
        FirstTieX = xPos;
        FirstTieY = yPos;
        break;
      }
    }
  } while(1);
  //
  // Count spaces or own ties until opposite tie or end of board
  // 
  do {
    if ((_Field[yPos][xPos] == OppositePlayer) || (xPos <0 ) || (yPos < 0)) {
      break;
    } 
    xPos--;
    yPos--;
    cnt++;
  } while(1);
  if (cnt >= 4) {
    //
    // We are able to set 4 ties in this row. Now we are going to calculate the best position to set the tie
    // 
    //
    // If we have a RowWithSpace, check if tie can be set in this space.
    // If there is already a tie in the field under the space we are done here
    //
    if (RowWithSpace && _Field[SpacePosY+1][SpacePosX]) {
      return SpacePosX;
    } else {
      //
      // Try to set the tie at the beginning (1xright, 1xdown) of the row
      //
      if (_Field[FirstTieY+1][FirstTieX+1] == 0)  {
        if ((_Field[FirstTieY+2][FirstTieX+1]) || ((FirstTieY+2) == 5))  // Check if there is a tie or the end of the board under the empty field.
        return FirstTieX+1;
      } else {
        //
        // Try to set the tie at the end (1xleft, 1xup) of the row.
        // First go backward until the "leftest" stone reached. 
        //
        xPos = FirstTieX;
        yPos = FirstTieY;
        do {
          xPos--;
          yPos--;
        } while (_Field[yPos][xPos] == CurrentPlayer);
        if ((!((xPos < 0) || (yPos < 0))) && _Field[yPos+1][xPos]) {
          return xPos;        
        } else {
          return 0;
        }
      }
    }
  } else {
    return 0;
  }  
}

/*********************************************************************
*
*       CheckIf4TiesCanBeSetCrossLineLeftRight()
*
* Function description:
*   This function checks weather theare are enough spaces to 
*   be able to win the game (set 4ties in a cross row)
*   We can be sure, that xPos is the "outtest" tie
*   This means for left to right check: the riggest one
*               right to left check: the leftest one
*
*/
int CheckIf4TiesCanBeSetCrossLineLeftRight(int xPos, int yPos, int RowWithSpace, int CurrentPlayer) {
  int cnt = 0;
  int OppositePlayer;
  int SpacePosY = 0;
  int SpacePosX = 0;
  int FirstTieY = 0;
  int FirstTieX = 0;
  
  if (CurrentPlayer==1) {
    OppositePlayer = 2;
  } else {
    OppositePlayer = 1;
  }
  //
  // Determine first tie
  //
  do {
    xPos--;
    yPos++;
    //
    // Check if we are out of the board
    //
    if(yPos >= 6) {
      xPos++;
      yPos--;
      FirstTieX = xPos;
      FirstTieY = yPos;
      break;
    }
    if(_Field[yPos][xPos] != CurrentPlayer) {
      if(RowWithSpace && (_Field[yPos][xPos] == 0)) {
        RowWithSpace = 0;
        SpacePosY = yPos;
        SpacePosX = xPos;
        
      } else {
        //
        // break if we have reached the first tie
        //
        xPos++;
        yPos--;
        FirstTieX = xPos;
        FirstTieY = yPos;
        break;
      }
    }
  } while(1);
  //
  // Count spaces or own ties until opposite tie or end of board
  // 
  do {
    if ((_Field[yPos][xPos] == OppositePlayer) || (xPos > 6) || (yPos > 5)) {
      break;
    } 
    xPos++;
    yPos--;
    cnt++;
  } while(1);
  if (cnt >= 4) {
    //
    // We are able to set 4 ties in this row. Now we are going to calculate the best position to set the tie
    // 
    //
    // If we have a RowWithSpace, check if tie can be set in this space.
    // If there is already a tie in the field under the space we are done here
    //
    if (RowWithSpace && _Field[SpacePosY+1][SpacePosX]) {
      return SpacePosX;
    } else {
      //
      // Try to set the tie at the beginning of the row
      //
      if (_Field[FirstTieY+1][FirstTieX-1] == 0)  {
        if ((_Field[FirstTieY+2][FirstTieX-1]) || ((FirstTieY+2) == 5))  // Check if there is a tie or the end of the board under the empty field.
        return FirstTieX-1;
      } else {
        //
        // Try to set the tie at the end of the row.
        // First go forward until the last stone reached. 
        //
        xPos = FirstTieX;
        yPos = FirstTieY;
        do {
          xPos++;
          yPos--;
        } while (_Field[yPos][xPos] == CurrentPlayer);
        if ((!((xPos > 6) || (yPos > 5))) && _Field[yPos+1][xPos]) {
          return xPos;        
        }
        else {
          return 0;
        }
      }
    }
  } else {
    return 0;
  }  
}


/*********************************************************************
*
*       CheckIf4TiesCanBeSetCrossLineLeftRight()
*
* Function description:
*   This functions checks wheater there are enough spaces to 
*   be able to win the game (set 4 ties in this row/column)
*   We can be sure, that xPos is the last tie of the row to be checked
*   
*   
*
*/

int CheckIf4TiesCanBeSetVertical(int xPos, int yPos, int RowWithSpace, int CurrentPlayer) {
  int cnt = 0;
  int OppositePlayer;
  
  if (CurrentPlayer==1) {
    OppositePlayer = 2;
  } else {
    OppositePlayer = 1;
  }
  //
  // Determine first tie
  //
  do {
    if(_Field[yPos][xPos] != CurrentPlayer) {
      if(RowWithSpace && (_Field[yPos][xPos] == 0)) {
        RowWithSpace = 0;
      } else {
        //
        // break if we have reached the first tie
        //
		xPos++;
        break;
      }
    }
    xPos--;
    //
    // Check if we are out of the board
    //
    if((xPos < 0) || (yPos >= 6)) {
      break;
    }
  } while(1);
  //
  // Go back until we reached the first opposite tie or the beginning of the board
  //
  do {
    xPos--;
    if ((_Field[yPos][xPos] == OppositePlayer) || (xPos < 0)) {
      xPos++;
      break;
    }
  } while(1);
  //
  // Count spaces or own ties until opposite tie or end of board
  // 
  do {
    if ((_Field[yPos][xPos] == OppositePlayer) || (xPos > 6)) {
      break;
    } 
    xPos++;
    cnt++;
  } while(1);
  if (cnt >= 4) {
	//
	//
	//
    return 1;
  } else {
    return 0;
  }
}


/*********************************************************************
*
*       CheckIf4TiesCanBeSetCrossLineLeftRight()
*
* Function description:
*   This function go through the whole field array
*   and looks if there are NumToBeChecked tiles in a "row".
*   If so, it checks wheater there is a possibility for the opposite
*   to set the fourth tile in order to win. If both conditions are true
*   the function returns an intenger where the first byte contains X value
*   and the second byte contains Y value
*
*   Parameters:
*     int NumToBeChecked: Number of tiles to be checked. (1-3)
*     int Player: Specifies the player to be checked
*
*   return values:
*      0: No possible field is available
*      1: Possible field is available
*                          and Bit[8:15] contains Y value
*
*/

int CheckNumInARow(int NumToBeChecked, int *pX, int *pY, int PlayerToBeChecked) {
  int cnt;
  int x;
  int y;
  int RecentTileSet;
  int AllowSpace;
  int SpacePos;
  int NextX;
  int NextY;
  int r;

  cnt = 0;
  RecentTileSet = 0;
  AllowSpace = 0;
  SpacePos = 0;
  //
  // Check the diagonal by going from left to right.
  //
  NextX = 0;
  NextY = 3;
  for (int i = 0; i < 6; i++) {
    x = NextX;
    y = NextY;   
	do {
      if ((_Field[y][x] == PlayerToBeChecked) && (RecentTileSet == 0)) {  // This is the first tie in this colum
        cnt = 1;
        RecentTileSet = 1;
        AllowSpace = 1;
      } else if (_Field[y][x] == PlayerToBeChecked) {                     // This is the next tie in this colum
        cnt++;
      } else {                                                         // Reset values when we do not hit a tie of current player
        if (AllowSpace && (_Field[y][x] == 0)) {                          // We allow 1 space between the ties (e.g.: tie player 1, no tie, tie player 1, tie player 1)
          AllowSpace = 0;
          SpacePos = x;
        } else {
          SpacePos = 0;
          cnt = 0;
          RecentTileSet = 0;
        }
      }
      y--;
      x++;    
      //
      // If cnt is equal to number of tiles to be checked, we check if there is empty
      // field on the top.
      //
      if (cnt == NumToBeChecked) {
        r = CheckIf4TiesCanBeSetCrossLineLeftRight(x,y, SpacePos, PlayerToBeChecked);
        if (r) {
          *pX = r;
          return 1;
        } else {  // we are not able to extend the existing row so we have to go on here...
          SpacePos = 0;
          cnt = 0;
          RecentTileSet = 0;           
        } 
      }
    } while(y>0);
    if (i <= 2) {            // We have to increment y the first 3 cycles
      NextY++;
    } else {                 // We have to increment x the remaining cycles
      NextX++;
    }
  }
  //
  // Check the diagonal by going from right to left
  //
  NextX = 3;
  NextY = 5;
  for (int i = 0; i < 6; i++) {
    x = NextX;
    y = NextY;  
    do {
      if ((_Field[y][x] == PlayerToBeChecked) && (RecentTileSet == 0)) { // This is the first tie in this colum
        cnt = 1;
        RecentTileSet = 1;
        AllowSpace = 1;
      } else if (_Field[y][x] == PlayerToBeChecked) {                    // This is the next tie in this colum
        cnt++;
      } else {                                                           // Reset values when we do not hit a tie of current player
        if (AllowSpace && (_Field[y][x] == 0)) {                         // We allow 1 space between the ties (e.g.: tie player 1, no tie, tie player 1, tie player 1)
          AllowSpace = 0;
          SpacePos = x;
        } else {
          SpacePos = 0;
          cnt = 0;
          RecentTileSet = 0;
        }
      }
      y--;
      x--;    
      //
      // If cnt is equal to number of tiles to be checked, we check if there is empty
      // field on the top.
      //
      if (cnt == NumToBeChecked) {
        r = CheckIf4TiesCanBeSetCrossLineRightLeft(x, y, SpacePos, PlayerToBeChecked);
        if (r) {
          *pX = r;
          return 1;
        } else {  // we are not able to extend the existing row so we have to go on here...
          SpacePos = 0;
          cnt = 0;
          RecentTileSet = 0;           
        } 
      } 
    } while(x>0);
    if (i <= 3) {
      NextX++;
    } else {
      NextY++;
    }
  }
  //
  // Check horizontal
  //
  for (y = 0; (y < FIELD_Y); y++) {               // go through each column
    for (x = 0; (x < FIELD_X); x++) {             // Go through each row
      if ((_Field[y][x] == PlayerToBeChecked) && (RecentTileSet == 0)) {  // This is the first tie in this colum
        AllowSpace = 1;
        cnt = 1;
        RecentTileSet = 1;
      } else if (_Field[y][x] == PlayerToBeChecked) {                     // This is the next tie in this colum
        cnt++;
      } else {                                                            // Reset values when we do not hit a tie of current player
        if (AllowSpace && (_Field[y][x] == 0)) {                          // We allow 1 space between the ties (e.g.: tie player 1, no tie, tie player 1, tie player 1)
          AllowSpace = 0;
          SpacePos = x;
        } else {
          SpacePos = 0;
          cnt = 0;
          RecentTileSet = 0;
        }
      }
      //
      // If cnt is equal to number of tiles to be checked, we check if there is empty
      // field on left or right of the ties
      //
      if (cnt == NumToBeChecked) {
        //
        // First check if it's possible to win with this row. (possiblility to set 4 ties?
        //
        if (CheckIf4TiesCanBeSetVertical(x,y,SpacePos,PlayerToBeChecked)) {
          if (SpacePos && (_Field[y+1][SpacePos])) {                           // Do we have a row with a space between
            *pX = SpacePos;
            *pY = y;
            return 1;          
          } else {
            if (((x-NumToBeChecked) >= 0) && (_Field[y][x-NumToBeChecked] == 0) && ((_Field[y+1][x-NumToBeChecked]) || y == 5)) { // Field left empty and on the board?
              *pX = x-NumToBeChecked;
              *pY = y;
              return 1;
            } else if  (_Field[y][x+1] == 0 && ((x+1) <= 6) && ((_Field[y+1][x+1]) || y == 5)) {      // Field right empty and on the board?
              *pX = x+1;
              *pY = y;
              return 1;
            } else {                                // No possibility to set a tie
              cnt = 0;
              RecentTileSet = 0;
            }
          }
          cnt = 0;
          RecentTileSet = 0;
        }
      }
    }
    SpacePos = 0;
    cnt = 0;
    RecentTileSet = 0;   
  }
  //
  // Check vertical
  //
  for (x = 0; (x < (FIELD_X - 1)); x++) {         // go through each row
    for (y = 0; (y < FIELD_Y); y++) {  // Go through each column
      if ((_Field[y][x] == PlayerToBeChecked) && (RecentTileSet == 0)) {  // This is the first tie in this colum
        cnt = 1;
        RecentTileSet = 1;
      } else if (_Field[y][x] == PlayerToBeChecked) {                     // This is the next tie in this colum
        cnt++;
      } else {                                                         // Reset values when we do not hit a tie of current player
        cnt = 0;
        RecentTileSet = 0;
      }
      //
      // If cnt is equal to number of tiles to be checked, we check if there is empty
      // field on the top.
      //
      if (cnt == NumToBeChecked) {
        if ((_Field[y-NumToBeChecked][x] == 0) && ((y-NumToBeChecked) >= 0)) { // Field is empty and on the board?
          *pX = x;
          *pY = y-NumToBeChecked;
          return 1;
      } else {                                // No possibility to set a tie
          cnt = 0;
          RecentTileSet = 0;  
        }
      }
    }
    cnt = 0;
    RecentTileSet = 0;   
  }
  return 0;
}