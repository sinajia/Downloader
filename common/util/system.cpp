#include "StdAfx.h"
#include <io.h>
#include <Tlhelp32.h>
#include <Nb30.h>
#include <winioctl.h>
#include "xzip/XUnzip.h"
#include "system.h"

#pragma comment(lib, "Version")
#pragma comment(lib, "Wininet")
#pragma comment(lib, "Netapi32.lib")

namespace SystemCommon {
namespace FilePathHelper {
BOOL ForceCreateDir(LPCTSTR pszFullPathFileName) {
  if (!pszFullPathFileName || !pszFullPathFileName[0])
    return FALSE;

  if (PathIsRoot(pszFullPathFileName))
    return TRUE;

  TCHAR Dir[MAX_PATH + 1] = {};
  int nNeed;
  LPCTSTR p, pLast;
  BOOL Result;

  Result = FALSE;
  pLast = pszFullPathFileName;
  if (_tcslen(pLast) > _MAX_PATH) {
    return FALSE;
  }
  while (NULL != *pLast) {
    p = _tcsstr(pLast, _T("\\"));
    if (NULL == p) {
      p = _tcsstr(pLast, _T("/"));
      if (NULL == p) {
        return Result;
      }
    }
    nNeed = p - pszFullPathFileName;
    if (nNeed > 0) {
      memset(Dir, 0, sizeof(Dir));
      _tcsncpy_s(Dir, pszFullPathFileName, p - pszFullPathFileName);
      Result = CreateMyDir(Dir);
    }
    p++;
    pLast = p;
  }
  return Result;
}

BOOL CreateDir(LPCTSTR lpszPath) {
  tstring strFile = lpszPath;
  tstring strPath = GetPath(strFile);
  strFile = strPath.substr(0, strPath.size() - 1);
  while (!PathIsDirectory(strFile.c_str())) {
    if (PathFileExists(strFile.c_str())) {
      SetFileAttributes(strFile.c_str(), FILE_ATTRIBUTE_NORMAL);
      DeleteFile(strFile.c_str());
    }

    TCHAR wszPath[1024] = {};
    _tcsncpy_s(wszPath, strFile.c_str(), _TRUNCATE);
    if ((_T('\\') == strFile[strFile.size() - 1]) ||
        (_T('/') == strFile[strFile.size() - 1])) {
      PathRemoveFileSpec(wszPath);
    }
    PathRemoveFileSpec(wszPath);

    strFile = wszPath;
  }

  return ForceCreateDir(strPath.c_str());
}

BOOL CreateMyDir(LPCTSTR pszDir) {
  assert(pszDir);

  if (!pszDir || !pszDir[0])
    return FALSE;

  BOOL bRet;
  bRet = ::CreateDirectory(pszDir, NULL);
  if (FALSE == bRet) {
    if (ERROR_ALREADY_EXISTS == GetLastError())
      return TRUE;
  }
  return bRet;
}

BOOL GetAppPath(HINSTANCE hInst, LPTSTR lpPath, DWORD dMaxLen, LPCTSTR pAddon) {
  if (!lpPath || dMaxLen <= 0)
    return FALSE;

  TCHAR FileName[MAX_PATH] = {};
  TCHAR szFullPath[MAX_PATH] = {};
  int nStrLen, i;

  if (!GetModuleFileName(hInst, szFullPath, MAX_PATH))
    return FALSE;

  GetLongPathName(szFullPath, FileName, MAX_PATH);

  nStrLen = (int)_tcslen(FileName);

  for (i = nStrLen - 1; i > 0; i--) {
    if (FileName[i] == _T('\\') && i >= 2) {
      FileName[i + 1] = _T('\0');
      break;
    }
  }

  if (NULL == pAddon)
    _sntprintf_s(lpPath, dMaxLen, _TRUNCATE, _T("%s"), FileName);
  else
    _sntprintf_s(lpPath, dMaxLen, _TRUNCATE, _T("%s%s"), FileName, pAddon);
  lpPath[dMaxLen - 1] = _T('\0');
  return TRUE;
}

tstring GetAssignPath(DWORD assing, BOOL forecCreate) {
  TCHAR szAppPath[MAX_PATH + 1] = {0};
  SHGetSpecialFolderPath(NULL, szAppPath, assing, forecCreate);
  return szAppPath;
}

tstring GetFileName(tstring pathname) {
  int i;
  for (i = pathname.length() - 1; i >= 0; --i) {
    if (pathname[i] == _T('\\') || pathname[i] == _T('/'))
      break;
  }
  return pathname.substr(i + 1);
}

tstring GetPath(tstring pathname) {
  int i;
  for (i = pathname.length() - 1; i >= 0; --i) {
    if (pathname[i] == _T('\\') || pathname[i] == _T('/'))
      break;
  }
  return pathname.substr(0, i + 1);
}

BOOL DeepDeleteFile(const tstring &strFile) {
  int nR = _taccess(strFile.c_str(), 0);

  if (nR != 0 || PathIsDirectory(strFile.c_str())) {
    return TRUE;
  }

  if (!PathFileExists(strFile.c_str()))
    return TRUE;

  SetFileAttributes(strFile.c_str(), FILE_ATTRIBUTE_NORMAL);

  BOOL bRet = ::DeleteFile(strFile.c_str());
  if (!bRet) {
    tstring strDstTmp = strFile + _T(".tmp");
    MoveFileEx(strFile.c_str(), strDstTmp.c_str(), MOVEFILE_REPLACE_EXISTING);
    if (!DeleteFile(strDstTmp.c_str())) {
      MoveFileEx(strDstTmp.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    }
  }
  return bRet;
}

BOOL DeleteAllFiles(tstring strPath) {
  if (strPath.empty()) {
    return TRUE;
  }
  DeleteFile(strPath.c_str());

  SHFILEOPSTRUCT fo;
  tstring strFrom = strPath;

  if (strPath[strPath.length() - 1] != _T('\\')) {
    strFrom = strFrom + _T("\\");
  }
  strFrom = strFrom + _T("*.*") + _T('\0');
  memset(&fo, 0, sizeof(fo));

  fo.hwnd = NULL;
  fo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
  fo.wFunc = FO_DELETE;
  fo.pFrom = strFrom.c_str();

  SHFileOperation(&fo);
  RemoveDirectory(strPath.c_str());

  return TRUE;
}

BOOL GetBackslashDir(tstring &strPath) {

  TCHAR szPath[MAX_PATH] = {};
  int len = strPath.length();
  _tcsncpy_s(szPath, strPath.c_str(), _TRUNCATE);

  if (len >= 1 &&
      (_T('\\') != szPath[len - 1] || (_T('/') != szPath[len - 1]))) {
    PathAddBackslash(szPath);
  }
  strPath = szPath;
  return TRUE;
}

BOOL CopyFolder(const tstring &strSrc, const tstring &strDst, BOOL bForce) {
  tstring tmpFrom(strSrc);
  tstring tmpTo(strDst);
  BOOL bRet = FALSE;

  if (!PathFileExists(tmpTo.c_str())) {
    ForceCreateDir(tmpTo.c_str());
  }
  if (PathFileExists(tmpFrom.c_str())) {
    WIN32_FIND_DATA FindFileData;
    tstring tmpFind(tmpFrom);
    tmpFind += _T("\\*.*");

    HANDLE hFind = ::FindFirstFile(tmpFind.c_str(), &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)
      return bRet;

    while (TRUE) {
      tstring tmpFile(tmpTo);
      tmpFile += _T("\\");
      tmpFile += FindFileData.cFileName;

      if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (FindFileData.cFileName[0] != '.') {
          if (!PathFileExists(tmpFile.c_str())) {
            GetBackslashDir(tmpFile);
            ForceCreateDir(tmpFile.c_str());
          }
          bRet = CopyFolder(tmpFrom + _T("\\") + FindFileData.cFileName,
                            tmpFile, bForce);
        }
      } else {
        if (bForce) {
          bRet = DeepCopyFile(
              (tmpFrom + _T("\\") + FindFileData.cFileName).c_str(),
              tmpFile.c_str());
        } else if (!PathFileExists(tmpFile.c_str())) {
          bRet =
              ::CopyFile((tmpFrom + _T("\\") + FindFileData.cFileName).c_str(),
                         tmpFile.c_str(), TRUE);
        }
      }
      if (!FindNextFile(hFind, &FindFileData))
        break;
    }
    FindClose(hFind);
  }
  return bRet;
}

BOOL DeepCopyFile(const tstring &strSrc, const tstring &strDst) {
  BOOL bRet = FALSE;
  if (PathFileExists(strSrc.c_str())) {
    if (!StringHelper::CompareNoCase(strSrc, strDst)) {
      if (PathFileExists(strDst.c_str())) {
        SetFileAttributes(strDst.c_str(), FILE_ATTRIBUTE_NORMAL);
      } else {
        tstring strPath = GetPath(strDst);
        ForceCreateDir(strPath.c_str());
      }

      if (!DeleteFile(strDst.c_str())) {
        tstring strDstTmp = strDst + _T(".tmp");
        MoveFileEx(strDst.c_str(), strDstTmp.c_str(),
                   MOVEFILE_REPLACE_EXISTING);
        if (!DeleteFile(strDstTmp.c_str())) {
          MoveFileEx(strDstTmp.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
        }
      }
      ::CopyFile(strSrc.c_str(), strDst.c_str(), FALSE);
      bRet = TRUE;
    }
  }
  return bRet;
}

BOOL UnzipFileToPath(LPCTSTR lpszFile, LPCTSTR lpszDir) {
  BOOL res = FALSE;
  TCHAR szPath[MAX_PATH] = { 0 };
  GetCurrentDirectory(MAX_PATH, szPath);
  if (!SetCurrentDirectory(lpszDir))
    return res;
  HZIP hz = OpenZip((LPVOID)lpszFile, 0, ZIP_FILENAME);

  if (hz) {
    ZIPENTRYW ze = { 0 };
    if (ZR_OK == GetZipItem(hz, -1, &ze)) {
      res = TRUE;
      int nCount = ze.index;
      for (int i = 0; i < nCount; i++) {
        GetZipItem(hz, i, &ze);
        SetFileAttributes(ze.name, FILE_ATTRIBUTE_ARCHIVE);
        if (UnzipItem(hz, i, ze.name, 0, ZIP_FILENAME) != ZR_OK) {
          res = FALSE;
          break;
        }
        SetFileAttributes(ze.name, FILE_ATTRIBUTE_NORMAL);
      }
    }
    CloseZip(hz);
  }
  SetCurrentDirectory(szPath);
  return res;
}

DWORD GetFileSize(const tstring& path) {
  HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile && hFile != INVALID_HANDLE_VALUE) {
    DWORD dwFileSize = ::GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    return dwFileSize;
  }
  return INVALID_FILE_SIZE;
}

} // namespace FilePathHelper

namespace FileVersionHelper {
BOOL GetFileVersion(LPCTSTR strFile, LPTSTR pszVersion, int nVersionLen) {
  if (!strFile || !strFile[0] || !pszVersion || nVersionLen <= 0)
    return FALSE;

  TCHAR szVersionBuffer[8192] = {};
  DWORD dwVerSize;
  DWORD dwHandle;

  dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
  if (dwVerSize == 0 || dwVerSize > (sizeof(szVersionBuffer) - 1))
    return FALSE;

  if (GetFileVersionInfo(strFile, 0, dwVerSize, szVersionBuffer)) {
    VS_FIXEDFILEINFO *pInfo;
    unsigned int nInfoLen;

    if (VerQueryValue(szVersionBuffer, _T("\\"), (void **)&pInfo, &nInfoLen)) {
      _sntprintf_s(
          pszVersion, nVersionLen, _TRUNCATE, _T("%d.%d.%d.%d"),
          HIWORD(pInfo->dwFileVersionMS), LOWORD(pInfo->dwFileVersionMS),
          HIWORD(pInfo->dwFileVersionLS), LOWORD(pInfo->dwFileVersionLS));
      pszVersion[nVersionLen - 1] = _T('\0');
      return TRUE;
    }
  }

  return FALSE;
}

tstring GetFileVersion(LPCTSTR strFile) {
  if (!strFile || !strFile[0])
    return _T("");

  TCHAR szVersionBuffer[8192] = {};
  if (GetFileVersion(strFile, szVersionBuffer, SIZEOF(szVersionBuffer))) {
    return szVersionBuffer;
  }
  return _T("");
}

tstring GetFileVersion(HMODULE hModule) {
  if (!hModule)
    return _T("");

  TCHAR szFilePath[MAX_PATH] = {};
  GetModuleFileName(hModule, szFilePath, MAX_PATH);
  return GetFileVersion(szFilePath);
}

} // namespace FileVersionHelper

namespace NetHelper {
BOOL GetDomain(LPCTSTR url, LPTSTR buf, int cchBuf) {
  if (!url || !url[0] || !buf || cchBuf <= 0)
    return FALSE;

  URL_COMPONENTS uc = {};
  uc.dwStructSize = sizeof(uc);
  uc.lpszHostName = buf;
  uc.dwHostNameLength = cchBuf;

  memset(buf, 0, cchBuf * sizeof(TCHAR));
  __try {
    if (!InternetCrackUrl(url, lstrlen(url), (DWORD)ICU_DECODE, &uc)) {
      return FALSE;
    } else {
      LPTSTR lpstr = StrStr(buf, _T("\\"));
      if (lpstr)
        *lpstr = 0;
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }

  if (_tcslen(buf) == 0)
    return FALSE;
  return TRUE;
}

} // namespace NetHelper

namespace StringHelper {
tstring Trim(tstring &s) {
  if (s.empty()) {
    return s;
  }

  s.erase(0, s.find_first_not_of(_T(" ")));
  s.erase(s.find_last_not_of(_T(" ")) + 1);
  return s;
}

tstring ReplaceAll(tstring &str, const tstring &old_value,
                    const tstring &new_value) {
  for (tstring::size_type pos(0); pos != tstring::npos;
       pos += new_value.length()) {
    if ((pos = str.find(old_value, pos)) != tstring::npos)
      str.replace(pos, old_value.length(), new_value);
    else
      break;
  }
  return str;
}

tstring MakeLower(tstring &s) {
  if (s.empty()) {
    return s;
  }

  TCHAR *pBuf = new TCHAR[s.length() + 1];
  _tcsncpy_s(pBuf, s.length() + 1, s.c_str(), _TRUNCATE);
#if _MSC_VER < 1500
  _tcslwr(pBuf);
#else
  _tcslwr_s(pBuf, s.length() + 1);
#endif
  s = pBuf;
  delete[] pBuf;

  return s;
}

BOOL CompareNoCase(tstring sA, tstring sB) {
  MakeLower(sA);
  MakeLower(sB);
  if (0 == sA.compare(sB)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int FindNoCase(tstring sA, tstring sB) {
  MakeLower(sA);
  MakeLower(sB);
  return sA.find(sB);
}

std::string TString2String(tstring str) {
  std::string result;
#ifdef _UNICODE
  result = Wstring2String(str.c_str());
#else
  result = str;
#endif
  return result;
}

std::string Wstring2String(const wchar_t *pwc) {
  if (!pwc || !pwc[0])
    return "";

  std::string result;

  int nLen =
      WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pwc, -1, NULL, 0, NULL, NULL);
  if (nLen <= 0) {
    return std::string("");
  }
  char *presult = new char[nLen];
  if (NULL == presult) {
    return std::string("");
  }
  WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pwc, -1, presult, nLen, NULL, NULL);
  presult[nLen - 1] = 0;
  result = presult;
  delete[] presult;

  return result;
}

std::wstring String2WString(const char *pc) {
  if (!pc || !pc[0])
    return L"";

  int nLen = strlen(pc);
  int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pc, nLen, 0, 0);
  if (nSize <= 0) {
    return std::wstring(L"");
  }
  WCHAR *pDst = new WCHAR[nSize + 1];
  if (NULL == pDst) {
    return NULL;
  }
  MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pc, nLen, pDst, nSize);
  pDst[nSize] = 0;
  if (0xFEFF == pDst[0]) {
    for (int i = 0; i < nSize; i++) {
      pDst[i] = pDst[i + 1];
    }
  }
  std::wstring wcstr(pDst);
  delete[] pDst;
  return wcstr;
}

tstring String2TString(const char *pc) {
  tstring result;
#ifdef _UNICODE
  result = String2WString(pc);
#else
  result = pc;
#endif
  return result;
}
} // namespace StringHelper

namespace OS {
  using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);

  typedef struct {
    TCHAR szOperation[MAX_PATH];
    TCHAR szFilePath[MAX_PATH + 1];
    TCHAR szParam[MAX_PATH];
    TCHAR szDirectory[MAX_PATH + 1];
    int nShowCmd;
  } SHELLEXECUTE_PARAM;

BOOL AysnShellExecute(LPCTSTR lpszOperation, LPCTSTR lpszFilePath,
                      LPCTSTR lpszParam, LPCTSTR lpszDirectory, int nShowCmd) {
  SHELLEXECUTE_PARAM *pParam = new SHELLEXECUTE_PARAM;
  memset(pParam, 0, sizeof(SHELLEXECUTE_PARAM));
  if (lpszOperation)
    _sntprintf_s(pParam->szOperation, _countof(pParam->szOperation),
                 lpszOperation);
  if (lpszFilePath)
    _sntprintf_s(pParam->szFilePath, _countof(pParam->szFilePath),
                 lpszFilePath);
  if (lpszParam)
    _sntprintf_s(pParam->szParam, _countof(pParam->szParam), lpszParam);
  if (lpszDirectory)
    _sntprintf_s(pParam->szDirectory, _countof(pParam->szDirectory),
                 lpszDirectory);
  pParam->nShowCmd = nShowCmd;

  HANDLE hThread =
      CreateThread(NULL, 0, ShellExecuteProc, (LPVOID)pParam, 0, NULL);
  CloseHandle(hThread);
  return TRUE;

}

DWORD WINAPI ShellExecuteProc(LPVOID lpParam) {
  if (lpParam) {
    SHELLEXECUTE_PARAM *pParam = (SHELLEXECUTE_PARAM *)lpParam;
    ShellExecute(NULL, pParam->szOperation, pParam->szFilePath, pParam->szParam,
                 pParam->szDirectory, pParam->nShowCmd);
    delete pParam;
  }
  return 0;
}

void SafeTerminateThread(HANDLE hThread, DWORD dwExitCode /* = -1*/) {
  ::SuspendThread(hThread);
  ::TerminateThread(hThread, dwExitCode);
  ::CloseHandle(hThread);
}

DWORD ProcesstoPid(const TCHAR *pid) {
  DWORD dwpid = 0;
  int nProcessCount = 0;
  HANDLE hProcessSnap = NULL;
  TCHAR buffer[MAX_PATH + 1];
  PROCESSENTRY32 pe32 = {0};
  int i;
  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hProcessSnap == (HANDLE)-1) {
    return dwpid;
  }
  pe32.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hProcessSnap, &pe32)) {
    do {
      lstrcpy(buffer, pe32.szExeFile);
      for (i = lstrlen(buffer); i > 0; i--)
        if (buffer[i] == _T('\\'))
          break;
      if (!lstrcmpi(pid, &buffer[i])) {
        nProcessCount++;
        dwpid = pe32.th32ProcessID;
        break;
      }
    } while (Process32Next(hProcessSnap, &pe32));
  } else {
    return dwpid;
  }
  CloseHandle(hProcessSnap);
  return dwpid;
}

BOOL IsWow64() {
  BOOL bIsWow64 = FALSE;

  LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
      GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

  if (NULL != fnIsWow64Process) {
    if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
    }
  }
  return bIsWow64;
}

BOOL SetRegValue(HKEY hKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, tstring& strVal) {
  HKEY hResKey;
  LONG lRet;
  DWORD dwType = 0;
  lRet =
    RegCreateKeyEx(hKey, lpszKeyName, 0, REG_NONE, REG_OPTION_NON_VOLATILE,
      KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hResKey, &dwType);
  if (lRet != ERROR_SUCCESS)
    return FALSE;

  lRet = RegSetValueExW(hResKey, lpszValueName, NULL, REG_SZ,
    (LPBYTE)strVal.c_str(), (strVal.length() + 1) * 2);
  if (lRet != ERROR_SUCCESS)
    return FALSE;

  RegCloseKey(hResKey);

  return TRUE;
}

BOOL CreateLowIntegrityProcess(LPCTSTR Param) {
  DWORD dwError = ERROR_SUCCESS;
  HANDLE hToken = NULL;
  HANDLE hNewToken = NULL;
  SID_IDENTIFIER_AUTHORITY MLAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;
  PSID pIntegritySid = NULL;
  TOKEN_MANDATORY_LABEL tml = { 0 };
  STARTUPINFO si = { sizeof(si) };
  PROCESS_INFORMATION pi = { 0 };
  if (!OpenProcessToken(GetCurrentProcess(),
    TOKEN_DUPLICATE | TOKEN_QUERY | TOKEN_ADJUST_DEFAULT |
    TOKEN_ASSIGN_PRIMARY,
    &hToken)) {
    dwError = GetLastError();
    goto Cleanup;
  }
  if (!DuplicateTokenEx(hToken, 0, NULL, SecurityImpersonation, TokenPrimary,
    &hNewToken)) {
    dwError = GetLastError();
    goto Cleanup;
  }
  if (!AllocateAndInitializeSid(&MLAuthority, 1, SECURITY_MANDATORY_MEDIUM_RID,
    0, 0, 0, 0, 0, 0, 0, &pIntegritySid)) {
    dwError = GetLastError();
    goto Cleanup;
  }
  tml.Label.Attributes = SE_GROUP_INTEGRITY;
  tml.Label.Sid = pIntegritySid;
  if (!SetTokenInformation(hNewToken, TokenIntegrityLevel, &tml,
    (sizeof(tml) + GetLengthSid(pIntegritySid)))) {
    dwError = GetLastError();
    goto Cleanup;
  }
  if (!CreateProcessAsUser(hNewToken, NULL, (LPWSTR)Param, NULL, NULL,

    FALSE, 0, NULL, NULL, &si, &pi)) {
    dwError = GetLastError();
    goto Cleanup;
  }

Cleanup:

  if (hToken) {
    CloseHandle(hToken);
    hToken = NULL;
  }
  if (hNewToken) {
    CloseHandle(hNewToken);
    hNewToken = NULL;
  }
  if (pIntegritySid) {
    FreeSid(pIntegritySid);
    pIntegritySid = NULL;
  }
  if (pi.hProcess) {
    CloseHandle(pi.hProcess);
    pi.hProcess = NULL;
  }
  if (pi.hThread) {
    CloseHandle(pi.hThread);
    pi.hThread = NULL;
  }
  if (ERROR_SUCCESS != dwError) {
    SetLastError(dwError);
    return FALSE;
  }
  else {
    return TRUE;
  }
}

} // namespace OS

namespace Window {
HWND FindWindowByProcess(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                         DWORD nProcessID) {
  HWND hWnd = NULL;
  if (nProcessID == 0)
    nProcessID = GetCurrentProcessId();
  DWORD dwWindowProcessID = 0;
  do {
    hWnd = FindWindowEx(NULL, hWnd, lpszClassName, lpszWindowName);
    if (hWnd != NULL) {
      GetWindowThreadProcessId(hWnd, &dwWindowProcessID);
      if (dwWindowProcessID == nProcessID)
        break;
    }
  } while (hWnd != NULL);

  return hWnd;
}

void StretchForegroundWindow(HWND hWnd) {
  HWND hForWnd = ::GetForegroundWindow();
  if (hWnd == hForWnd)
    return;

  DWORD dwProcessId;
  DWORD dwThread1 = GetWindowThreadProcessId(hWnd, &dwProcessId);
  DWORD dwThread2 = GetWindowThreadProcessId(hForWnd, &dwProcessId);
  if (dwThread1 != dwThread2) {
    AttachThreadInput(dwThread1, dwThread2, TRUE);
    SetForegroundWindow(hWnd);
    AttachThreadInput(dwThread1, dwThread2, FALSE);
  }
  else {
    SetForegroundWindow(hWnd);
  }
}

} // namespace Window

namespace Device {
  typedef struct _ASTAT_ {
    ADAPTER_STATUS adapt;
    NAME_BUFFER NameBuff[30];
  } ASTAT, * PASTAT;

ASTAT Adapter;
static char g_strTempBuf[1024];

const char* GetTheFirstMac() {
  NCB ncb;
  UCHAR uRetCode;
  LANA_ENUM lana_enum_buf;

  memset(g_strTempBuf, 0, sizeof(g_strTempBuf));

  memset(&ncb, 0, sizeof(ncb));
  ncb.ncb_command = NCBENUM;

  ncb.ncb_buffer = (unsigned char*)&lana_enum_buf;
  ncb.ncb_length = sizeof(lana_enum_buf);

  uRetCode = Netbios(&ncb);

  if (uRetCode != 0) {
    return g_strTempBuf;
  }

  int nIndex = 0;

  {
    int lana_num = lana_enum_buf.lana[nIndex];

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = lana_num;

    uRetCode = Netbios(&ncb);

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_num;

    strcpy((char*)ncb.ncb_callname, "*               ");
    ncb.ncb_buffer = (unsigned char*)&Adapter;

    ncb.ncb_length = sizeof(Adapter);

    uRetCode = Netbios(&ncb);

    if (uRetCode == 0) {
      char szTemp[32] = { 0 };
      _snprintf(
        szTemp, 32, "%02X%02X%02X%02X%02X%02X",
        Adapter.adapt.adapter_address[0], Adapter.adapt.adapter_address[1],
        Adapter.adapt.adapter_address[2], Adapter.adapt.adapter_address[3],
        Adapter.adapt.adapter_address[4], Adapter.adapt.adapter_address[5]);

      strcat(g_strTempBuf, szTemp);
    }
  }

  return g_strTempBuf;
}

} // namespace Device


} // namespace SystemCommon
