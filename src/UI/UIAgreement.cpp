#include "stdafx.h"
#include "util/util_tools.h"
#include "../base/Thread.h"
#include "../base/WeakPtr.h"
#include "../base/WrapperObj.h"
#include "../base/common_threads.h"
#include "../base/lazy_instance.h"
#include "../Global.h"
#include "UIAgreement.h"

static constexpr int BEGINSCROLL = 0x101;
static constexpr int SCROLLING = 0x102;
static constexpr int HEIGHT = 300;
static constexpr int ENHEIGHT = 400;

CUIContents::CUIContents() : _parent(NULL), _scrollDelta(0) {}

CUIContents::~CUIContents() { ATLTRACE("delete CUIContents\n"); }

void CUIContents::OnWheel(WPARAM wParam) {
  const int zDelta = (int)(short)HIWORD(wParam);
  const int height = global.m_zh ? HEIGHT : ENHEIGHT;
  if (zDelta < 0) {
    _scrollDelta += 20;
    if (_scrollDelta > height) {
      _scrollDelta = height;
    }
  }
  if (zDelta > 0) {
    _scrollDelta -= 20;
    if (_scrollDelta < 0) {
      _scrollDelta = 0;
    }
  }
  tstring strID = _T("id_static_tip2");
  CBaseElementCtl *pEle = _elementManager.Search(strID);
  if (pEle) {
    static_cast<CStaticCtl *>(pEle)->_scrollDelta = _scrollDelta;
    RaiseInvalidate(TRUE);
  }
}

LRESULT CUIContents::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled) {
  const int height = global.m_zh ? HEIGHT : ENHEIGHT;
  if (wParam == BEGINSCROLL) {
    KillTimer(BEGINSCROLL);
    SetTimer(SCROLLING, 100, NULL);
  }
  if (wParam == SCROLLING) {
    _scrollDelta += 4;
    if (_scrollDelta > height) {
      KillTimer(SCROLLING);
    } else {
      tstring strID = _T("id_static_tip2");
      CBaseElementCtl *pEle = _elementManager.Search(strID);
      if (pEle) {
        static_cast<CStaticCtl *>(pEle)->_scrollDelta = _scrollDelta;
        RaiseInvalidate(TRUE);
      }
    }
  }
  return S_OK;
}

LRESULT CUIContents::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  if (_parent && ::IsWindow(_parent)) {
    ::PostMessage(_parent, WM_CLOSE, 0, 0);
  }
  return S_OK;
}

void CUIContents::CreateWnd(HWND hWnd) {
  _parent = hWnd;
  std::wstring strXMLSource;
  switch (global.lanId_) {
  case 0x804: {
    strXMLSource = L"UICONTENT";
  } break;
  default:
    strXMLSource = L"ENGUICONTENT";
  }

  CTransWindow::DUICreate(hWnd, strXMLSource.c_str());
  ::ShowWindow(m_hWnd, SW_SHOW);
}

void CUIContents::OnFinalMessage(HWND) {
  _elementManager.Dispose();
  delete this;
}

void CUIContents::OnLoad() {
  RECT rc;
  ::GetWindowRect(_parent, &rc);
  rc.left += 20;
  rc.top += 47;

  SetWindowPos(0, &rc, SWP_NOZORDER | SWP_NOSIZE);
}

void CUIContents::OnLoaded() { SetTimer(BEGINSCROLL, 600, NULL); }

CUIAgreement::CUIAgreement() : _obj(NULL) {}

CUIAgreement::~CUIAgreement() {}

void CUIAgreement::CreateWnd(HWND hpWnd) {
  std::wstring strXMLSource;
  switch (global.lanId_) {
  case 0x804: {
    strXMLSource = L"UIAGREEMENT";
  } break;
  default:
    strXMLSource = L"ENGUIAGREEMENT";
  }

  CTransWindow::DUICreate(hpWnd, strXMLSource.c_str());
}

void CUIAgreement::DestroyWnd() {
  _elementManager.Dispose();
  DestroyWindow();
}

LRESULT CUIAgreement::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  CTransWindow::OnLButtonDown(uMsg, wParam, lParam, bHandled);
  PostMessage(WM_CLOSE, 0, 0);
  return S_OK;
}

void CUIAgreement::OnLoad() {
  _obj = new CUIContents();
  _obj->CreateWnd(m_hWnd);
}

void CUIAgreement::OnLoaded() { SetFocus(); }

void CUIAgreement::OnExit() {
  _elementManager.Dispose();
  DestroyWnd();

  (static_cast<base::MessagePumpForUI *>(
       base::MessageLoop::LayerLoop::Back()->pump_win()))
      ->Quit();
}

void CUIAgreement::OnFinalMessage(HWND) { m_hWnd = NULL; }

void CUIAgreement::DrawBorder(CDCHandle dc) {
  DUI_Rect wndRect;
  GetClientRect(&wndRect);

  HDC hDC = dc.m_hDC;

  BITMAP bmpFrameData;
  GetObject(_bmpFrame, sizeof(bmpFrameData), &bmpFrameData);

  if (_bmpFrame) {
    CGDIResource::GetInstance().GDI_DrawImageStretch(
        hDC, _bmpFrame, wndRect,
        DUI_Rect(0, 0, bmpFrameData.bmWidth, bmpFrameData.bmHeight), 210,
        _rcFrameCorner);
  }
}

LRESULT CUIAgreement::OnWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) {
  if (_obj && ::IsWindow(_obj->m_hWnd)) {
    _obj->OnWheel(wParam);
  }
  return S_OK;
}
