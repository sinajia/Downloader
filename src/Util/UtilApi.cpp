#include "stdafx.h"
#include <Tlhelp32.h>
#include <shlobj.h>
#include <util/md5.h>
#include <fstream>
#include <mmsystem.h>
#include "util/base.h"
#include "util/util_tools.h"
#include "UtilApi.h"

#pragma comment(lib, "winmm.lib")

namespace Util {
namespace Tools {
int GetTextWidth(HWND hWnd, LPCTSTR lpszText, HFONT hFont) {
  CPaintDC dc(hWnd);
  HFONT hOldFont = dc.SelectFont(hFont);
  ::SetBkMode(dc.m_hDC, TRANSPARENT);
  CSize szText;
  ::GetTextExtentPoint32(dc.m_hDC, lpszText, lstrlen(lpszText), &szText);
  dc.SelectFont(hOldFont);

  return szText.cx;
}

int GetStringMd5(LPCTSTR lpSrc, TCHAR *lpDest, DWORD dwSize) {
  char *pBuf = new char[MAX_PATH];
  memset(pBuf, 0, MAX_PATH * sizeof(char));

  USES_CONVERSION;

  StringCchCopyA(pBuf, MAX_PATH, T2A(lpSrc));
  MD5 md5((BYTE *)pBuf, strlen(pBuf));
  string strMd5 = md5.toString();
  StringCchCopyA(pBuf, MAX_PATH, strMd5.c_str());
  StringCchCopy(lpDest, dwSize, A2T(pBuf));
  delete[] pBuf;
  return strMd5.size();
}

int GetStringMd5A(const char *lpSrc, char *lpDest, DWORD dwSize) {
  char *pBuf = new char[MAX_PATH];
  memset(pBuf, 0, MAX_PATH * sizeof(char));
  StringCchCopyA(pBuf, MAX_PATH, (lpSrc));
  MD5 md5((BYTE *)pBuf, strlen(pBuf));
  string strMd5 = md5.toString();
  StringCchCopyA(pBuf, MAX_PATH, strMd5.c_str());
  StringCchCopyA(lpDest, dwSize, (pBuf));
  delete[] pBuf;
  return strMd5.size();
}

BOOL IsWin7() {
  return Util::Base::GetAccurateOSVersion() >= WINDOWS_VISTA_NO_SP ? TRUE
    : FALSE;
}


} // namespace Tools

} // namespace Util
