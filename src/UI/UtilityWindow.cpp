#include "StdAfx.h"
#include "UtilityWindow.h"

CUtilityWindow::CUtilityWindow() {}

CUtilityWindow::~CUtilityWindow() {}

CUtilityWindow& CUtilityWindow::GetInstance() {
  static CUtilityWindow* pThis = new CUtilityWindow();
  return *pThis;
}

BOOL CUtilityWindow::Initialize() {
  CGDIResource::GetInstance().Initialize(_T(""));

  RECT rcPos = {0, 0, 1, 1};

  HWND hwnd = NULL;

  hwnd = Create(HWND_MESSAGE, rcPos, CGlobal::szGlobalString_, WS_POPUP);

  if (!hwnd)
    return FALSE;

  SetWindowLongPtr(GWLP_USERDATA, CGlobal::UitlWndPrivacyCode_);
  return TRUE;
}

void CUtilityWindow::UnInitialize() {
  DestroyWindow();

  pUIMainWindow_ = NULL;

  CGDIResource::GetInstance().Dispose();

  CGDIResource::GetInstance().UnInitialize();
}

LRESULT CUtilityWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/,
                                 LPARAM /*lParam*/, BOOL & /*bHandled*/) {

  global.hUitlWnd_ = m_hWnd;
  pUIMainWindow_ = new CUIMainWindowEx();

  if (NULL == pUIMainWindow_.get()) {
    return -1;
  }
  std::wstring strXMLSource;
  switch (global.lanId_) {
  case 0x804: {
    strXMLSource = L"UIMAINWINDOW";
  } break;
  default:
    strXMLSource = L"ENGUIMAINWINDOW";
  }

  if (!pUIMainWindow_->CreateWnd(NULL, strXMLSource.c_str())) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return -1;
  }
  return 0;
}

LRESULT CUtilityWindow::OnDelGdi(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  HWND hwnd = (HWND)lParam;
  if (::IsWindow(hwnd)) {
    ::DestroyWindow(hwnd);
  }
  return 0;
}

LRESULT CUtilityWindow::OnRestore(UINT /*uMsg*/, WPARAM /*wParam*/,
                                  LPARAM /*lParam*/, BOOL & /*bHandled*/) {
  pUIMainWindow_->RestoreWindow();
  return 0;
}
