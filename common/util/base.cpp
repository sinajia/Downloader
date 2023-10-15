#include "stdafx.h"
#include <io.h>
#include "Base.h"

namespace Util {
namespace Base {
int GetAccurateOSVersion() {
  static int lCurrentVersion = -1;
  if (-1 ==  lCurrentVersion) {
    lCurrentVersion = WINDOWS_OLDEST;
    OSVERSIONINFOW os = {sizeof(OSVERSIONINFOW)};
    if (::GetVersionExW(&os) && os.dwPlatformId == VER_PLATFORM_WIN32_NT) {
      if (os.dwMajorVersion == 5) {
        if (os.dwMinorVersion == 0)
          lCurrentVersion = WINDOWS_2000;
        else if (os.dwMinorVersion == 1) {
          if (_wcsnicmp(L"Service Pack 1", os.szCSDVersion, 127) == 0)
            lCurrentVersion = WINDOWS_XP_SP_1;
          else if (_wcsnicmp(L"Service Pack 2", os.szCSDVersion, 127) == 0)
            lCurrentVersion = WINDOWS_XP_SP_2;
          else if (_wcsnicmp(L"Service Pack 3", os.szCSDVersion, 127) == 0)
            lCurrentVersion = WINDOWS_XP_SP_3;
          else
            lCurrentVersion = WINDOWS_XP_NO_SP;
        } else if (os.dwMinorVersion == 2)
          lCurrentVersion = WINDOWS_2003;
      } else if (os.dwMajorVersion == 6) {
        if (os.dwMinorVersion == 0) {
          if (_wcsnicmp(L"Service Pack 1", os.szCSDVersion, 127) == 0)
            lCurrentVersion = WINDOWS_VISTA_SP_1;
          else if (_wcsnicmp(L"Service Pack 2", os.szCSDVersion, 127) == 0)
            lCurrentVersion = WINDOWS_VISTA_SP_2;
          else
            lCurrentVersion = WINDOWS_VISTA_NO_SP;
        } else if (os.dwMinorVersion == 1)
          lCurrentVersion = WINDOWS_7;
        else if (os.dwMinorVersion == 2)
          lCurrentVersion = WINDOWS_8;
        else if (os.dwMinorVersion == 3)
          lCurrentVersion = WINDOWS_8_SP1;
      } else if (os.dwMajorVersion == 10) {
        lCurrentVersion = WINDOWS_11;
      } else {
        lCurrentVersion = WINDOWS_LATEST;
      }
    }
  }
  return lCurrentVersion;
}

std::string GetOSVersion() {
  long version = GetAccurateOSVersion();
  std::string version_str = "unknown";
  switch (version) {
  case WINDOWS_2000: {
    version_str = "Windows2000";
  } break;
  case WINDOWS_XP_NO_SP:
  case WINDOWS_XP_SP_1:
  case WINDOWS_XP_SP_2:
  case WINDOWS_XP_SP_3: {
    version_str = "WindowsXP";
  } break;
  case WINDOWS_2003: {
    version_str = "WindowsServer2003";
  } break;
  case WINDOWS_VISTA_NO_SP:
  case WINDOWS_VISTA_SP_1: {
    version_str = "WindowsVista";
  } break;
  case WINDOWS_7: {
    version_str = "Windows7";
  } break;
  case WINDOWS_8: {
    version_str = "Windows8";
  } break;
  case WINDOWS_8_SP1: {
    version_str = "Windows8.1";
  } break;
  case WINDOWS_11: {
    version_str = "Windows11";
  } break;
  default:
    break;
  }
  return version_str;
}

std::string longtochar(long l) {
  char s[50] = {0};
  _snprintf_s(s, 50, _TRUNCATE, "%ld", l);
  std::string s_s = s;
  return s_s;
}

std::wstring longtocharw(long l) {
  WCHAR s[50] = {0};
  _snwprintf_s(s, 50, _TRUNCATE, L"%ld", l);
  std::wstring s_s = s;
  return s_s;
}

std::wstring UTF8DecodeW(LPCSTR lpszUtf8Src) {
  std::wstring strUnicode(L"");
  if (!lpszUtf8Src)
    return strUnicode;

  int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8Src,
    strlen(lpszUtf8Src), NULL, 0);

  wchar_t* wszString = new wchar_t[wcsLen + 1];
  if (wszString) {
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8Src, strlen(lpszUtf8Src),
      wszString, wcsLen);
    wszString[wcsLen] = L'\0';
    strUnicode = wszString;
    delete[] wszString;
  }
  return strUnicode;
}

std::string UTF8DecodeA(LPCSTR lpszUtf8Src) {
  std::string strAnsi("");
  if (!lpszUtf8Src)
    return strAnsi;

  int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8Src,
    strlen(lpszUtf8Src), NULL, 0);

  wchar_t* wszString = new wchar_t[wcsLen + 1];
  if (wszString) {
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8Src, strlen(lpszUtf8Src),
      wszString, wcsLen);
    wszString[wcsLen] = L'\0';

    int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString,
      wcslen(wszString), NULL, 0, NULL, NULL);
    char* szAnsi = new char[ansiLen + 1];
    if (szAnsi) {
      ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi,
        ansiLen, NULL, NULL);
      szAnsi[ansiLen] = '\0';
      strAnsi = szAnsi;
      delete[] szAnsi;
    }
    delete[] wszString;
  }

  return strAnsi;
}

std::string WChar2Ansi(LPCWSTR pwszSrc) {
  int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
  if (nLen <= 0)
    return std::string("");
  char* pszDst = new char[nLen];
  if (NULL == pszDst)
    return std::string("");
  WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
  pszDst[nLen - 1] = 0;
  std::string strTemp(pszDst);
  delete[] pszDst;
  return strTemp;
}

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen) {
  int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
  if (nSize <= 0)
    return std::wstring(L"");
  WCHAR* pwszDst = new WCHAR[nSize + 1];
  if (NULL == pwszDst)
    return std::wstring(L"");
  MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
  pwszDst[nSize] = 0;
  if (pwszDst[0] == 0xFEFF)
    for (int i = 0; i < nSize; i++)
      pwszDst[i] = pwszDst[i + 1];
  std::wstring wcharString(pwszDst);
  delete pwszDst;
  return wcharString;
}

std::string ws2s(std::wstring& inputws) { return WChar2Ansi(inputws.c_str()); }

std::wstring s2ws(const std::string& s) {
  return Ansi2WChar(s.c_str(), s.size());
}

std::string UnicodeToUTF8(LPCWSTR lpszUnicode) {
  std::string utf8("");
  if (!lpszUnicode)
    return utf8;

  int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, lpszUnicode,
                                    wcslen(lpszUnicode), NULL, 0, NULL, NULL);

  char *szU8 = new char[u8Len + 1];
  if (szU8) {
    ::WideCharToMultiByte(CP_UTF8, NULL, lpszUnicode, wcslen(lpszUnicode), szU8,
                          u8Len, NULL, NULL);
    szU8[u8Len] = '\0';
    utf8 = szU8;
    delete[] szU8;
  }
  return utf8;
}

std::wstring UTF8ToUnicode(LPCSTR lpszUtf8) {
  std::wstring strUnicode(L"");
  if (!lpszUtf8)
    return strUnicode;

  int wcsLen =
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8, strlen(lpszUtf8), NULL, 0);

  wchar_t *wszString = new wchar_t[wcsLen + 1];
  if (wszString) {
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszUtf8, strlen(lpszUtf8), wszString,
      wcsLen);
    wszString[wcsLen] = L'\0';
    strUnicode = wszString;
    delete[] wszString;
  }
  return strUnicode;
}

std::wstring dwtos(const DWORD dw) {
  WCHAR ws[50] = { 0 };
  _sntprintf_s(ws, 50, _TRUNCATE, _T("%u"), dw);
  return std::wstring(ws);
}

std::string dwtoss(const DWORD dw) {
  char ss[50] = { 0 };
  _snprintf(ss, 50, "%u", dw);
  return std::string(ss);
}

DWORD stodw(std::string& strNum) {
  DWORD dw = strtoul(strNum.c_str(), NULL, 10);
  return dw;
}

std::string f2ss(const double f) {
  char ss[50] = { 0 };
  _snprintf(ss, 50, "%f", f);
  return std::string(ss);
}

std::wstring f2ws(const double f) {
  WCHAR ws[50] = { 0 };
  _sntprintf_s(ws, 50, _TRUNCATE, _T("%f"), f);
  return std::wstring(ws);
}



} // namespace Base

} // namespace Util
