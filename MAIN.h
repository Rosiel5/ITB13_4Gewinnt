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
#include <windowsx.h>
#include <process.h>
#include <stdio.h>

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
#define FIELD_X         7
#define FIELD_Y         6


#define BORDER_BOARD    10              //Size of border to window frame
#define BORDER_TOP      100             //Space for status messages
#define BORDER_TILE     3               //Border around the tiles

/*********************************************************************
*
*       Defines, fix
*
**********************************************************************
*/
#define ID_NEWGAME_PvsP           9001
#define ID_NEWGAME_PvsAI          9002
#define ID_NEWGAME_PvP_NET        9003
#define ID_NEWGAME_PvP_NET_Server 9004
#define ID_NEWGAME_PvP_NET_Client 9005

#define NET_MODE_SERVER           1
#define NET_MODE_CLIENT           2
#define NET_UDP_PORT              49149
#define NET_TCP_PORT              49150
#define NET_DISCOVER_TIMEOUT_S    5
#define NET_END_FLAG_VAL          (1u << 3)

/*********************************************************************
*
*       Global data
*
**********************************************************************
*/

EXTERN HWND hwnd;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

EXTERN int _redraw;                     // 0: Nothing have to be done, 1: We have to repaint the window
EXTERN int _close;                      // 
EXTERN int _Started;                    // Game has allready started? 1: Yes, 0: No
EXTERN int _Field[FIELD_Y][FIELD_X];    // [y][x] coordinate of the field
EXTERN int _CurrentPlayer;              // 1: Player 1; 2: Player 2; This option is undependet form _GameModus (Player 2 can be a human or AI)
EXTERN int _RoundCount;                 // Number of rounds we have played. Max value of round is 42 since we have 7x7 fields.
EXTERN int _GameModus;                  // Specify wheater we play vs a computer or a player. 1: Player vs Player; 2: Player vs AI
EXTERN int _GUIStarted;  
EXTERN char _acMessage[256];            // Status message
typedef struct {
    int width;
    int height;
} BOARD_SIZE;

EXTERN int _TileSize;
EXTERN BOARD_SIZE _Board;

EXTERN BOARD_SIZE _Window;
EXTERN char NET_ServerWaiting;
EXTERN char NET_TurnComplete;
EXTERN char NET_EndWinFlag;

/*********************************************************************
*
*       Player functions
*
**********************************************************************
*/
int  GetPlayer(void);                   //Returns player 1 or 2
void SwitchPlayer(void);                //Changes player

/*********************************************************************
*
*       Game logic
*
**********************************************************************
*/
int  GetField(int PosX);                //Returns true if correct field
bool CheckEnd(void);	                  //Returns true if field is filled up
bool CheckWin(int X, int Y);	          //Returns true if current player has won
void Start(void);
int  IncreaseRoundCntCheckEnd(int FieldX, int FieldY);
/*********************************************************************
*
*       GUI
*
**********************************************************************
*/
int  SetTile(int FieldX);	              // Draws the animation of the falling tile
void DisplayWin(void);			            // Sets the win message and ends game
void DisplayEnd(void);                  // Sets the end message and ends the game
void SetMessage(const char* acIn);      // Set a custom message
void ClearMessage(void);                // Clear message to automatically show status message
void DrawBoard(void);			              // Draws the game board


/*********************************************************************
*
*       AI
*
**********************************************************************
*/
int CheckNumInARow(int NumToBeChecked, int *pX, int *pY, int PlayerToBeChecked);
void CalculateNextMove(unsigned int );   // Calculates the next move
int Check2Tiels3Spaces(int *pX, int PlayerToBeChecked);

/*********************************************************************
*
*       NET
*
**********************************************************************
*/
VOID ThreadNET(PVOID pvoid);

/*********************************************************************
*
*       Common
*
**********************************************************************
*/
int DisplayErrorMessageBox(char Type, TCHAR * acStr);
#endif
/*************************** End of file ****************************/