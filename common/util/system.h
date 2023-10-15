#pragma once

#ifndef tstring
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif
#endif

namespace SystemCommon {
namespace FilePathHelper {
BOOL ForceCreateDir(LPCTSTR pszFullPathFileName);

BOOL CreateDir(LPCTSTR lpszPath);

BOOL CreateMyDir(LPCTSTR pszDir);

BOOL GetAppPath(HINSTANCE hInst, LPTSTR lpPath, DWORD dMaxLen, LPCTSTR pAddon);

tstring GetAssignPath(DWORD assing, BOOL forceCreate);

tstring GetFileName(tstring pathname);

tstring GetPath(tstring pathname);

BOOL DeepDeleteFile(const tstring &strFile);

BOOL DeleteAllFiles(tstring strPath);

BOOL GetBackslashDir(tstring &strPath);

BOOL CopyFolder(const tstring &strSrc, const tstring &strDst,
                BOOL bForce = FALSE);

BOOL DeepCopyFile(const tstring &strSrc, const tstring &strDst);

BOOL UnzipFileToPath(LPCTSTR lpszFile, LPCTSTR lpszDir);

DWORD GetFileSize(const tstring& path);

} // namespace FilePathHelper


namespace FileVersionHelper {
BOOL GetFileVersion(LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen);

tstring GetFileVersion(LPCTSTR strFile);

tstring GetFileVersion(HMODULE hModule);

} // namespace FileVersionHelper


namespace NetHelper {
BOOL GetDomain(LPCTSTR url, LPTSTR buf, int cbBuf);
} // namespace NetHelper


namespace StringHelper {
tstring Trim(tstring &s);
tstring ReplaceAll(tstring &str, const tstring &old_value,
                    const tstring &new_value);

tstring MakeLower(tstring &s);

BOOL CompareNoCase(tstring sA, tstring sB);
int FindNoCase(tstring sA, tstring sB);

std::string TString2String(tstring str);
std::string Wstring2String(const wchar_t *pwc);

std::wstring String2WString(const char *pc);
tstring String2TString(const char *pc);

} // namespace StringHelper


namespace OS {
BOOL AysnShellExecute(LPCTSTR lpszOperation, LPCTSTR lpszFile,
                      LPCTSTR lpszParam = NULL, LPCTSTR lpszDirectory = NULL,
                      int nShowCmd = SW_SHOW);
DWORD WINAPI ShellExecuteProc(LPVOID lpParam);
void SafeTerminateThread(HANDLE hThread, DWORD dwExitCode = -1);

DWORD ProcesstoPid(const TCHAR *pid);

BOOL IsWow64();

BOOL SetRegValue(HKEY hKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName,
  tstring& strVal);

BOOL CreateLowIntegrityProcess(LPCTSTR);

} // namespace OS


namespace Window {
HWND FindWindowByProcess(LPCTSTR lpszClassName, LPCTSTR lpszWindowName = NULL,
                         DWORD nProcessID = 0 /*Current Process*/);
void StretchForegroundWindow(HWND);

} // namespace Window


namespace Device {
  const char* GetTheFirstMac();

} // namespace Device

}; // namespace SystemCommon
