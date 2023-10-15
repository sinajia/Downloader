#pragma once
#pragma warning(disable: 4302)
#pragma warning(disable: 4244)

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <CommDlg.h>
#include <Shlwapi.h>
#include <malloc.h>
#include <memory>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
#include <shlobj.h>
#include <shellapi.h>
#include <atlconv.h>
#include <atlbase.h>
#include <atlapp.h>
#include <ExDispid.h>
#include <MsHtmdid.h>
#include <WinInet.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlwin.h>
#include <comdef.h>
#include <mshtmhst.h>
#include <shellapi.h>
#include <strsafe.h>
#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <new>

extern CAppModule _Module;
extern HINSTANCE _hInstance;

#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif

#define SIZEOF(A) (sizeof(A) / sizeof((A)[0]))

#ifndef MAX_URL_LEN
#define MAX_URL_LEN INTERNET_MAX_URL_LENGTH
#endif

#include "util/base.h"

#define MAX_WINDOW_WIDTH 800
#define MAX_WINDOW_HEIGHT 600
