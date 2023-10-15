#include "stdafx.h"
#include "util/util_tools.h"
#include "../Global.h"
#include "../base/Thread.h"
#include "../base/WeakPtr.h"
#include "../base/WrapperObj.h"
#include "../base/common_threads.h"
#include "../base/lazy_instance.h"
#include "UICannotDown.h"

CUIUpDate::CUIUpDate() : infostyle_(QUIT) {}

CUIUpDate::~CUIUpDate() {}

void CUIUpDate::CreateWnd(HWND hpWnd) {
  std::wstring strXMLSource;
  switch (global.lanId_) {
  case 0x804: {
    strXMLSource = L"UICANNOTDOWN";
  } break;
  default:
    strXMLSource = L"ENGUICANNOTDOWN";
  }
  CTransWindow::DUICreate(hpWnd, strXMLSource.c_str());
}

void CUIUpDate::DestroyWnd() {
  _elementManager.Dispose();
  DestroyWindow();
}

void CUIUpDate::OnLoad() {
  tstring strID = _T("id_btn_close");
  CBaseElementCtl *pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIUpDate::OnCancal));
  }
  strID = _T("id_btn_ok");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIUpDate::OnYes));
  }
  strID = _T("id_btn_cancal");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIUpDate::OnCancal));
  }
  strID = _T("id_btn_quit");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIUpDate::OnYes));
  }
  if (infostyle_ == QUIT) {
    strID = _T("id_btn_quit");
    pEle = _elementManager.Search(strID);
    if (pEle) {
      pEle->SetVisible(FALSE);
    }
    strID = _T("id_static_tip2");
    pEle = _elementManager.Search(strID);
    if (pEle) {
      pEle->SetVisible(FALSE);
    }
  } else if (infostyle_ == CHECKSYSTEM) {
    strID = _T("id_btn_ok");
    pEle = _elementManager.Search(strID);
    if (pEle) {
      pEle->SetVisible(FALSE);
    }
    strID = _T("id_btn_cancal");
    pEle = _elementManager.Search(strID);
    if (pEle) {
      pEle->SetVisible(FALSE);
    }
    strID = _T("id_static_tip1");
    pEle = _elementManager.Search(strID);
    if (pEle)
      pEle->SetVisible(FALSE);
    strID = _T("id_btn_close");
    pEle = _elementManager.Search(strID);
    if (pEle)
      pEle->SetVisible(FALSE);
  }
}

void CUIUpDate::OnLoaded() {}

void CUIUpDate::OnExit() {
  _elementManager.Dispose();
  DestroyWnd();

  (static_cast<base::MessagePumpForUI *>(
       base::MessageLoop::LayerLoop::Back()->pump_win()))
      ->Quit();
}

void CUIUpDate::OnFinalMessage(HWND) { m_hWnd = NULL; }

void CUIUpDate::OnYes() {
  _bOK = TRUE;
  PostMessage(WM_CLOSE, 0, 0);
}

void CUIUpDate::OnCancal() {
  _bOK = FALSE;
  PostMessage(WM_CLOSE, 0, 0);
}

void CUIUpDate::DrawBorder(CDCHandle dc) {
  CTransWindow::DrawBorder(dc);
  {
    HBITMAP hBitMap = NULL;
    hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
                                                        _T("APPLOGO"), PNG);
    if (hBitMap) {
      BITMAP bmpFrameData;
      GetObject(hBitMap, sizeof(bmpFrameData), &bmpFrameData);
      DUI_Rect rRect(22, 20, 22 + bmpFrameData.bmWidth,
                     20 + bmpFrameData.bmHeight);
      HDC hDC = dc.m_hDC;
      CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
    }
  }
}

void CUIUpDate::SetCustomTitle(std::wstring &title) {
  tstring strID = _T("id_static_tip2");
  CBaseElementCtl *pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetTitle(title);
  }
}
