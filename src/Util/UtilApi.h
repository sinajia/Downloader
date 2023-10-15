#pragma once

namespace Util {
namespace Tools {
int GetTextWidth(HWND hWnd, LPCTSTR lpszText, HFONT hFont);

int GetStringMd5(LPCTSTR lpSrc, TCHAR *lpDest, DWORD dwSize);

int GetStringMd5A(const char *lpSrc, char *lpDest, DWORD dwSize);

BOOL IsWin7();

} // namespace Tools

} // namespace Util
