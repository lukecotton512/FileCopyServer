// WinThreadMain.h
// Luke Cotton
// Our thread main functions for Windows.

// Preprocessor guard.
#ifndef __WINTHREADMAIN_HPP__
#define __WINTHREADMAIN_HPP__
// Include Windows.h.
#include <Windows.h>
// Server main.
DWORD WINAPI serverWinMain(LPVOID arg);
// Connection Main.
DWORD WINAPI connWinMain(LPVOID arg);
#endif
