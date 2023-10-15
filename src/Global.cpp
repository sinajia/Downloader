#include "stdafx.h"
#include "util/system.h"
#include "Global.h"


/// <configuration>
/// Set the following string and then compile

// Query zip download link
std::string CGlobal::downloadurl_ = "http://127.0.0.1:5001/download-url";
// directory name
std::string CGlobal::appDirName_ = "MyeXeAppDir";
// Exe file name such as eXeScope.exe or a relative path such as bin\\eXeScope.exe
std::string CGlobal::appname_ = "eXeScope.exe";
// shortcut file name
std::string CGlobal::shortcutname_ = "eXeScope";
// Link to open after installation
std::string CGlobal::openurl_ = "https://github.com/sinajia/Downloader";

/// </configuration>


ITaskbarList4* CGlobal::pTaskbar_ = nullptr;
TCHAR* CGlobal::szGlobalString_ = _T("65E6B66785CEC64B329");
LONG CGlobal::UitlWndPrivacyCode_ = 0x746A04DE;

CGlobal::CGlobal(): hUitlWnd_(NULL), lanId_(0), m_zh(false) {
  m_macAddress = SystemCommon::Device::GetTheFirstMac();
  m_platform = Util::Base::GetOSVersion();
}
CGlobal::~CGlobal() {}

CGlobal global;
