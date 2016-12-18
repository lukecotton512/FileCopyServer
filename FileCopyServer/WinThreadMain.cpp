// WinThreadMain.cpp
// Luke Cotton
// Defines our main functions for each thread for the Windows version.

// Our includes.
#include "WinThreadMain.h"
#include "threadmain.hpp"

// Our server thread main function.
DWORD WINAPI serverWinMain(LPVOID arg) {
	// Get our argument and typecast it to an ordinary void.
	void *ptr = (void *)arg;
	// Call server main with our argument.
	serverMain(ptr);
	// Return.
	return 0;
}

// Our connection main function.
DWORD WINAPI connWinMain(LPVOID arg) {
	// Get our argument and typecast it to an ordinary void.
	void *ptr = (void *)arg;
	// Call connMain with our argument.
	connMain(ptr);
	// Return.
	return 0;
}