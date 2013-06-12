/********************************************************************
*   File: MAIN.h
* 
* Purpose: Provides function prototypes for global functions
*
*/
#ifndef MAIN_H
#define MAIN_H

#ifdef MAIN
  #define EXTERN 
#else
  #define EXTERN extern
#endif

#include <windows.h>

/********************************************************************
*   Static variables
*
*/

EXTERN HWND hwnd;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

EXTERN int _redraw;
EXTERN int _close;
EXTERN int _Started;
EXTERN int _Field[6][7];
EXTERN int _CurrentPlayer;  // 1, 2
EXTERN int _RoundCount;     // <= 42;

typedef struct {
    int width;
    int height;
} BOARD_SIZE;

EXTERN int _TileSize;
EXTERN BOARD_SIZE _Board;

EXTERN BOARD_SIZE _Window;

#define FIELD_X         6
#define FIELD_Y         7


#define BORDER_BOARD    10  //Size of border to window frame
#define BORDER_TILE     3   //Border around the tiles

/********************************************************************
*    Player functions
*
*/
int  GetPlayer(void);         //Returns player 1 or 2
void SwitchPlayer(void);      //Changes player

/********************************************************************
*    Game logic
*
*/
int  GetField(int PosX);  //Returns true if correct field
bool CheckEnd(void);	    //Returns true if field is filled up
bool CheckWin(int X, int Y);	    //Returns true if current player has won
void Start(void);

/********************************************************************
*    GUI
*
*/
void CalculateField(void);      // Calculates width, height and tilesize
void SetWindowSize(void);       // Sets windows size on resize to match gamefield ratio
int  SetTile(int FieldX);	      // Draws the animation of the falling tile
void DisplayWin(void);			    // Drwas the win
void DisplayEnd(void);
void DrawBoard(void);			      // Draws the game board

#endif
/******* EOF *******************************************************/