#include "stdafx.h"
#include "util/base.h"
#include "util/util_tools.h"
#include "util/system.h"
#include "Global.h"
#include "Language/Language.h"
#include "LogAssist/LogAssist.h"
#include "UI/UtilityWindow.h"

HINSTANCE _hInstance = nullptr;
CAppModule _Module;

UINT WM_TASKBARBUTTONCREATED = 0x1000;

static HRESULT Initialize();
static HRESULT UnInitialize();

base::LazyInstance<base::Thread> io_thread_ LAZY_INSTANCE_INITIALIZER;
base::LazyInstance<base::Thread> file_thread_ LAZY_INSTANCE_INITIALIZER;

void InstallCommonThread() {
  if (!file_thread_.Get().IsRunning()) {
    file_thread_.Get().set_thread_name("file_thread");
    file_thread_.Get().StartWithOptions(base::Thread::Options());
    base::Threads::Set(base::Threads::FILE, file_thread_.Get().message_loop());
  }

  if (!io_thread_.Get().IsRunning()) {
    io_thread_.Get().set_thread_name("io_thread");
    io_thread_.Get().StartWithOptions(
        base::Thread::Options(base::MessageLoop::TYPE_IO));
    base::Threads::Set(base::Threads::IO, io_thread_.Get().message_loop());
  }
}

void InitGlobalPath() {
  if (Util::Base::GetAccurateOSVersion() <= WINDOWS_2003)
    global.m_strInstallPath = L"C:\\Program Files";
  else {
    global.m_strInstallPath =
        SystemCommon::FilePathHelper::GetAssignPath(CSIDL_APPDATA, FALSE);
  }

  global.m_packPath =
      SystemCommon::FilePathHelper::GetAssignPath(CSIDL_LOCAL_APPDATA, TRUE);
}

void UnstallCommonThread() {
  io_thread_.Get().Stop();
  file_thread_.Get().Stop();
  applog::log_entity_.Get().Thread().Stop();
}

int Run(int nCmdShow = SW_SHOWDEFAULT) {
  InstallCommonThread();

  InitGlobalPath();

  std::auto_ptr<base::MessageLoop> message_loop;
  message_loop.reset(new base::MessageLoop(base::MessageLoop::TYPE_UI));
  message_loop->set_thread_name("ui_thread");
  base::Threads::Set(Threads::UI, message_loop.get());

  INIT_LOG(DebugSet, _T("LOGINIT\\"))
  PRT_LOG3(DebugSet, ("sys lang: %d", global.lanId_));

  do {
    TCHAR szPath[MAX_PATH + 1] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    TCHAR szFileVersion[200] = {0};
    SystemCommon::FileVersionHelper::GetFileVersion(szPath, szFileVersion, 200);
    std::string verNum = Util::Base::WChar2Ansi(szFileVersion);
    PRT_LOG3(DebugSet, ("app started %s", verNum.c_str()))
  } while (false);

  if (!CUtilityWindow::GetInstance().Initialize()) {
    goto RETURN;
  }
  message_loop->Run();

  CUtilityWindow::GetInstance().UnInitialize();

RETURN:
  UnstallCommonThread();
  return TRUE;
}

void CheckIsRunning() {
  HWND hWnd = ::FindWindow(CGlobal::szUtilityClassName_, CGlobal::szGlobalString_);
  if (hWnd && ::IsWindow(hWnd) &&
      GetWindowLongPtr(hWnd, GWLP_USERDATA) == CGlobal::UitlWndPrivacyCode_) {

    ::PostMessage(hWnd, WM_IS_EXIST, 0, 0);
    ExitProcess(0);
  }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow) {
  CheckIsRunning();

  base::AtExitManager atmgr;

  _hInstance = hInstance;

  WM_TASKBARBUTTONCREATED = ::RegisterWindowMessage(L"TaskbarButtonCreated");

  HRESULT hRes = S_OK;

  BOOL bSuccess = AtlInitCommonControls(ICC_BAR_CLASSES);
  ::DefWindowProc(NULL, 0, 0, 0L);

  int nRet = -1;

  if (S_OK == Initialize()) {
    hRes = _Module.Init(NULL, hInstance);

    ATLASSERT(SUCCEEDED(hRes));

    nRet = Run(nCmdShow);

    _Module.Term();
  }

  UnInitialize();

  return nRet;
}

HRESULT Initialize() {
  LPCTSTR lpCommandLine = GetCommandLine();
  int nArgs = 0;
  LPTSTR *lpArgList = CommandLineToArgvW(lpCommandLine, &nArgs);
  LPCTSTR lpParaVal = nullptr;
  TCHAR szUrl[MAX_URL_LEN + 1] = {0};

  lan::AppLanguage::SysLanguage();

  return S_OK;
}

HRESULT UnInitialize() { return S_OK; }
