#include "StdAfx.h"
#include "UpdateWindowBase.h"

CUpdateWindowBase::CUpdateWindowBase() { m_hFuncInst = NULL; }

CUpdateWindowBase::~CUpdateWindowBase() {
  if (m_hFuncInst)
    FreeLibrary(m_hFuncInst);
}

LRESULT CUpdateWindowBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  m_Blend.BlendOp = 0;
  m_Blend.BlendFlags = 0;
  m_Blend.AlphaFormat = 1;
  m_Blend.SourceConstantAlpha = 255;

  if (IsBits32()) {
    m_hFuncInst = LoadLibrary(_T("User32.DLL"));
    BOOL bRet = FALSE;
    if (m_hFuncInst)
      UpdateLayeredWindow = (UPDATELAYERWINDOWPTR)GetProcAddress(
          m_hFuncInst, "UpdateLayeredWindow");
    else
      assert(0);

    PostMessage(MSG_UPDATE_WINDOW_INVALIDATE, 0, 100);
  }

  bHandled = FALSE;

  return 0;
}

LRESULT CUpdateWindowBase::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  CPaintDC dc(m_hWnd);
  DrawBg(dc);
  OnDraw(dc);
  return 0;
}

LRESULT CUpdateWindowBase::UpdateTranslate(UINT uMsg, WPARAM wParam,
                                           LPARAM lParam, BOOL &bHandled) {
  UpdateTransDisplay(lParam);
  return 0;
}

BOOL CUpdateWindowBase::UpdateTransDisplay(int Transparent) {

  if (!IsBits32()) {
    Invalidate(TRUE);
    return FALSE;
  }

  CRect rcWindow;
  GetWindowRect(rcWindow);
  POINT ptWinPos = {rcWindow.left, rcWindow.top};

  HDC hdcTemp = GetDC();
  HDC m_hdcMemory = CreateCompatibleDC(hdcTemp);
  HBITMAP hBitMap =
      CreateCompatibleBitmap(hdcTemp, rcWindow.Width(), rcWindow.Height());
  SelectObject(m_hdcMemory, hBitMap);
  if (Transparent < 0 || Transparent > 100)
    Transparent = 100;

  m_Blend.SourceConstantAlpha = int(Transparent * 2.55);
  HDC hdcScreen = ::GetDC(m_hWnd);

  DrawBg(m_hdcMemory);
  OnDraw(m_hdcMemory);

  SIZE sizeWindow = {rcWindow.Width(), rcWindow.Height()};

  POINT ptSrc = {0, 0};
  DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
  if ((dwExStyle & 0x80000) != 0x80000)
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);

  BOOL bRet = FALSE;
  bRet = UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow,
                             m_hdcMemory, &ptSrc, 0, &m_Blend, 2);

  ::ReleaseDC(m_hWnd, hdcScreen);
  hdcScreen = NULL;
  ::ReleaseDC(m_hWnd, hdcTemp);
  hdcTemp = NULL;
  DeleteObject(hBitMap);
  DeleteDC(m_hdcMemory);
  m_hdcMemory = NULL;

  return bRet;
}

bool CUpdateWindowBase::IsBits32() {
  DEVMODE lpDevMode;
  BOOL Result = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode);
  if (Result) {
    if (lpDevMode.dmBitsPerPel == 32)
      return true;
  }

  return false;
}
