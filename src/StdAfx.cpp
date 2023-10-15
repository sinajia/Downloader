#include "StdAfx.h"

#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "strsafe.lib")

void TrackMouseLeave(HWND hWnd) {
  TRACKMOUSEEVENT tme;
  memset(&tme, 0, sizeof(tme));
  tme.cbSize = sizeof(tme);
  tme.dwFlags = TME_LEAVE;
  tme.hwndTrack = hWnd;
  tme.dwHoverTime = 0;
  TrackMouseEvent(&tme);
}
