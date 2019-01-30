#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 500;

#ifdef _DEBUG

void RedirectIOToConsole()
{
  int hConHandle;
  long lStdHandle;
  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  FILE *fp;
  // allocate a console for this app
  AllocConsole();
  // set the screen buffer to be big enough to let us scroll text
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

  //coninfo.dwSize.Y = MAX_CONSOLE_LINES;
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

  //// redirect unbuffered STDOUT to the console
  //lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
  //hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  //fp = _fdopen(hConHandle, "w");
  //*stdout = *fp;
  //setvbuf(stdout, NULL, _IONBF, 0);

  //// redirect unbuffered STDIN to the console
  //lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
  //hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  //fp = _fdopen(hConHandle, "r");
  //*stdin = *fp;
  //setvbuf(stdin, NULL, _IONBF, 0);

  //// redirect unbuffered STDERR to the console
  //lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
  //hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  //fp = _fdopen(hConHandle, "w");
  //*stderr = *fp;
  //setvbuf(stderr, NULL, _IONBF, 0);

  //// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
  //// point to console as well

  //ios::sync_with_stdio(true);

  // Get STDOUT handle
  HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
  FILE *COutputHandle = _fdopen(SystemOutput, "w");

  // Get STDERR handle
  HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
  int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
  FILE *CErrorHandle = _fdopen(SystemError, "w");

  // Get STDIN handle
  HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
  int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
  FILE *CInputHandle = _fdopen(SystemInput, "r");

  //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
  ios::sync_with_stdio(true);

  // Redirect the CRT standard input, output, and error handles to the console
  freopen_s(&CInputHandle, "CONIN$", "r", stdin);
  freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
  freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

  std::wcout.clear();
  std::cout.clear();
  std::wcerr.clear();
  std::cerr.clear();
  std::wcin.clear();
  std::cin.clear();

}

#endif

//End of File