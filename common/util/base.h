#pragma once

constexpr int WINDOWS_OLDEST = -1000;
constexpr int WINDOWS_2000 = 1;
constexpr int WINDOWS_XP_NO_SP = 2;
constexpr int WINDOWS_XP_SP_1 = 3;
constexpr int WINDOWS_XP_SP_2 = 4;
constexpr int WINDOWS_XP_SP_3 = 5;
constexpr int WINDOWS_2003 = 6;
constexpr int WINDOWS_VISTA_NO_SP = 7;
constexpr int WINDOWS_VISTA_SP_1 = 8;
constexpr int WINDOWS_VISTA_SP_2 = 8;
constexpr int WINDOWS_7 = 9;
constexpr int WINDOWS_8 = 10;
constexpr int WINDOWS_8_SP1 = 11;
constexpr int WINDOWS_11 = 12;
constexpr int WINDOWS_LATEST = 1000;

namespace Util {
namespace Base {
int GetAccurateOSVersion();

std::string GetOSVersion();

std::string longtochar(long l);

std::wstring longtocharw(long l);

std::wstring UTF8DecodeW(LPCSTR lpszUtf8Src);

std::string UTF8DecodeA(LPCSTR lpszUtf8Src);

std::string WChar2Ansi(LPCWSTR pwszSrc);

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);

std::string ws2s(std::wstring& inputws);

std::wstring s2ws(const std::string& s);

std::string UnicodeToUTF8(LPCWSTR lpszUnicodeSrc);

std::wstring UTF8ToUnicode(LPCSTR lpszUtf8Src);

std::wstring dwtos(const DWORD dw);

std::string dwtoss(const DWORD dw);

DWORD stodw(std::string& strNum);

std::string f2ss(const double f);

std::wstring f2ws(const double f);

} // namespace Base

} // namespace Util
