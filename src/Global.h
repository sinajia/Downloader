#pragma once

#include "util/util_tools.h"

class CGlobal {
public:
  // configuration value
  static std::string downloadurl_;
  static std::string appDirName_;
  static std::string appname_;
  static std::string shortcutname_;
  static std::string openurl_;

  CGlobal();
  virtual ~CGlobal();

  HWND hUitlWnd_ = NULL;
  LANGID lanId_;

  bool m_zh;
  std::string m_filedownurl;
  std::string m_downloadfileLen;
  std::string m_macAddress;
  std::string m_platform;
  std::wstring m_packPath;
  std::wstring m_strInstallPath;

  static ITaskbarList4* pTaskbar_;

  constexpr static TCHAR* szUtilityClassName_ = _T("AppInstallerWindow");
  constexpr static TCHAR* szGlobalString_ = _T("65E6B66785CEC64B329");
  constexpr static LONG UitlWndPrivacyCode_ = 0x746A04DE;

};

extern CGlobal global;
