/********************************************************************
*   File: Common.cpp
* 
* Purpose: Includes functions used by different modules.
*
*/

#include "MAIN.h"

int DisplayErrorMessageBox(char Type, TCHAR * acStr) {
    int MB_ID = MessageBox(
        NULL,
        (LPCWSTR)acStr,
        (LPCWSTR)L"Error",
        Type
    );

    switch (MB_ID) {
      case IDCANCEL:
        break;
      case IDRETRY:
        break;
      default:
        break;
    }

    return MB_ID;
}

/*************************** End of file ****************************/
