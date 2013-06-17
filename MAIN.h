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
#include <stdio.h>

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define FIELD_X         7
#define FIELD_Y         6


#define BORDER_BOARD    10  //Size of border to window frame
#define BORDER_TILE     3   //Border around the tiles

#define AI 1

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/

EXTERN HWND hwnd;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

EXTERN int _redraw;
EXTERN int _close;
EXTERN int _Started;
EXTERN int _Field[6][7];    // [y][x]
EXTERN int _CurrentPlayer;  // 1, 2
EXTERN int _RoundCount;     // <= 42;

typedef struct {
    int width;
    int height;
} BOARD_SIZE;

EXTERN int _TileSize;
EXTERN BOARD_SIZE _Board;

EXTERN BOARD_SIZE _Window;

/*********************************************************************
*
*       Player functions
*
**********************************************************************
*/
int  GetPlayer(void);         //Returns player 1 or 2
void SwitchPlayer(void);      //Changes player

/*********************************************************************
*
*       Game logic
*
**********************************************************************
*/
int  GetField(int PosX);  //Returns true if correct field
bool CheckEnd(void);	    //Returns true if field is filled up
bool CheckWin(int X, int Y);	    //Returns true if current player has won
void Start(void);

/*********************************************************************
*
*       GUI
*
**********************************************************************
*/
void CalculateField(void);      // Calculates width, height and tilesize
void SetWindowSize(void);       // Sets windows size on resize to match gamefield ratio
int  SetTile(int FieldX);	      // Draws the animation of the falling tile
void DisplayWin(void);			    // Drwas the win
void DisplayEnd(void);
void DrawBoard(void);			      // Draws the game board
int  IncreaseRoundCntCheckEnd(int FieldX, int FieldY);

/*********************************************************************
*
*       AI
*
**********************************************************************
*/
void CalculateNextMove(unsigned int );   // Calculates the next move


#endif
/*************************** End of file ****************************/