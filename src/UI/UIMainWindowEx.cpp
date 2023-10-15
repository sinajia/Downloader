#include "stdafx.h"
#include <mmsystem.h>
#include <regex>
#include <math.h>
#include "jsoncpp/json/json.h"
#include "util/util_tools.h"
#include "util/system.h"
#include "../base/Thread.h"
#include "../base/common_threads.h"
#include "../Global.h"
#include "../LogAssist/LogAssist.h"
#include "../Language/Language.h"
#include "../Util/UtilApi.h"
#include "UtilityWindow.h"
#include "UICannotDown.h"
#include "UIAgreement.h"
#include "UIMainWindowEx.h"

#define PELEMENT (pEle = _elementManager.Search(strID))

CUIMainWindowEx::CUIMainWindowEx(): 
  instep_(BEFOREDOWN),
  installtype_(DEFAULT),
  downloadway_(SINGLE),
  showstate_(),
  time_download_(0), 
  time_install_(0),
  b_user_quit_(false),
  window_showing_(true),
  is_pack_complete_(false),
  f_down_process_(0.0),
  urldownloader_(this, global.downloadurl_),
  filedownloader_(this, global.downloadurl_, global.m_packPath) {}

CUIMainWindowEx::~CUIMainWindowEx() {
}

LRESULT CUIMainWindowEx::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  if (wParam != SC_MAXIMIZE) {
    bHandled = FALSE;
  }
  if (wParam == SC_RESTORE) {
    RaiseInvalidate(TRUE);
  }
  if (SC_CLOSE == wParam) {
    if (instep_ < INSTALLING) {
      CUIUpDate obj;
      obj.CreateWnd(m_hWnd);
      if (obj.DoModal()) {
        DownLoadingAUserQuit();
        b_user_quit_ = true;
      } else {
        bHandled = TRUE;
        base::Threads::Get(Threads::UI)
            ->PostTask(Bind(this, &CUIMainWindowEx::DoInstall));
      }
    } else if (instep_ == INSTALLING) {
      bHandled = TRUE;
      ShakeWindow();
    } else {
      bHandled = TRUE;
      base::Threads::Get(Threads::UI)
          ->PostTask(Bind(this, &CUIMainWindowEx::OnInstallComplete));
    }
  }
  return 0;
}

LRESULT CUIMainWindowEx::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled) {
  if (bshaking_ || _bCUpdateWindowBaseEx_TransShow) {
    bHandled = TRUE;
    return S_OK;
  }
  RECT rect;
  GetWindowRect(&rect);
  int x = GET_X_LPARAM(lParam);
  int y = GET_Y_LPARAM(lParam);

  DUI_Point pt(x, y);
  ScreenToClient(&pt);

  if (_elementManager.MouseOnElement(pt))
    return HTCLIENT;

  UINT nFlags = 0;
  _elementManager.OnMouseMove(nFlags, pt);

  int nTop = 0;
  int nLeft = 0;
  int nRight = 0;
  int nBottom = 0;

  if (IsZoomed()) {
    if ((y > rect.top + nTop) && (y <= rect.top + _nTitleHeight))
      return HTCAPTION;
    return HTCLIENT;
  }

  if ((y > rect.top + nTop) && (y <= rect.top + _nTitleHeight))
    return HTCAPTION;

  if (_bSupportChangeSize) {
    if (x >= rect.left && x <= rect.left + nLeft) {
      if (y < rect.top + nTop)
        return HTTOPLEFT;
      else if (y > rect.bottom - nBottom)
        return HTBOTTOMLEFT;

      return HTLEFT;
    }

    if (x <= rect.right && x >= rect.right - nRight) {
      if (y < rect.top + nTop)
        return HTTOPRIGHT;
      else if (y > rect.bottom - nBottom)
        return HTBOTTOMRIGHT;

      return HTRIGHT;
    }

    if (y >= rect.top && y <= rect.top + nTop)
      return HTTOP;

    if ((y > rect.top + nTop) && (y <= rect.top + _nTitleHeight))
      return HTCAPTION;

    if (y <= rect.bottom && y >= rect.bottom - nBottom) {
      return HTBOTTOM;
    }
  }

  return HTCLIENT;
}

LRESULT CUIMainWindowEx::OnPathIllCharacter(UINT uMsg, WPARAM wParam,
  LPARAM lParam, BOOL& bHandled) {
  CBaseElementCtl* pEle = NULL;
  tstring strID;
  strID = _T("id_panel_userdef");
  pEle = _elementManager.Search(strID);
  if (pEle && pEle->GetVisible()) {
  }
  else
    return 0;
  strID = _T("id_edit_installpath");
  pEle = _elementManager.Search(strID);
  if (!pEle)
    return 0;
  std::wstring title = pEle->GetTitle();
  std::wregex p(
    L"^[a-zA-Z]:\\\\{1}(?:[^/:*?\"<>|])*[^./:*?\"<>|]{1}$|[a-zA-Z]:\\\\");
  bool ism = std::regex_match(title, p);
  BOOL tipshow = FALSE;
  if (ism) {
    if (!(-1 == title.find(L"\\\\") && -1 == title.find(L".\\") && -1 == title.find(L"\\."))) {
      tipshow = TRUE;
    }
  } else {
    tipshow = TRUE;
  }
  strID = _T("id_static_patherror");
  pEle = _elementManager.Search(strID);
  if (pEle)
    pEle->SetVisible(tipshow);
  strID = _T("id_btn_installnow");
  PELEMENT;
  if (pEle) {
    ((CButtonCtl*)pEle)->Enable(!tipshow);
  }
  return 0;
}

LRESULT CUIMainWindowEx::OnTaskBarCreated(UINT uMsg, WPARAM wParam,
  LPARAM lParam, BOOL& bHandled) {
  if (CGlobal::pTaskbar_)
    return S_OK;

  if (S_OK == ::CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&CGlobal::pTaskbar_))) {
    CGlobal::pTaskbar_->HrInit();
    CGlobal::pTaskbar_->SetProgressState(m_hWnd, TBPF_NORMAL);
  }
  return S_OK;
}

HWND CUIMainWindowEx::CreateWnd(HWND hParent, const WCHAR *wstr) {
  CTransWindowEx<CUIMainWindowEx>::DUICreate(hParent, wstr);
  TransBeginShow();
  TransShowWindow();
  return m_hWnd;
}

void CUIMainWindowEx::DestroyWnd() {
  if (::IsWindow(m_hWnd)) {
    DestroyWindow();
  }
}

void CUIMainWindowEx::RestoreWindow() {
  if (::IsIconic(m_hWnd))
    ::ShowWindow(m_hWnd, SW_RESTORE);
  RaiseInvalidate();
}

void CUIMainWindowEx::OnLoad() {
  CBaseElementCtl *pEle = NULL;
  tstring strID;
  strID = _T("id_close");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnClose));
  }
  strID = _T("id_install");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnInstall));
  }

  strID = _T("id_checkbox");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    ((CCheckBoxCtl *)pEle)->SetCheck(TRUE);
    pEle->RegisterLButtonDownEvent(BIND(*this, CUIMainWindowEx::OnCheck));
  }
  strID = _T("id_openurl");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnAgreement));
  }
  strID = _T("id_userinstall");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnSelected));
  }
  strID = _T("id_btn_min");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnMin));
  }
  strID = _T("id_panel_userdef");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  strID = _T("id_btn_backmain");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnBackMain));
  }
  strID = _T("id_btn_installdir");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OpenInstallDir));
  }
  strID = _T("id_checkbox_shortcut");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    ((CCheckBoxCtl *)pEle)->SetCheck(TRUE);
  }
  strID = _T("id_checkbox_easymenu");
  pEle = _elementManager.Search(strID);
  if (pEle)
    ((CCheckBoxCtl *)pEle)->SetCheck(TRUE);
  strID = _T("id_checkbox_autorun");
  pEle = _elementManager.Search(strID);
  if (pEle)
    ((CCheckBoxCtl *)pEle)->SetCheck(FALSE);
  strID = _T("id_edit_installpath");
  PELEMENT;
  if (pEle)
    pEle->SetTitle(global.m_strInstallPath);
  strID = _T("id_btn_installnow");
  pEle = _elementManager.Search(strID);
  PELEMENT;
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnInstallSelected));
  }
  strID = _T("id_panel_download");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  strID = _T("id_panel_complete");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  strID = _T("id_btn_usenow");
  PELEMENT;
  if (pEle) {
    pEle->RegisterLButtonUpEvent(BIND(*this, CUIMainWindowEx::OnUseNow));
  }
  strID = _T("id_btn_complete");
  PELEMENT;
  if (pEle) {
    pEle->RegisterLButtonUpEvent(
        BIND(*this, CUIMainWindowEx::OnInstallComplete));
  }
  strID = _T("id_static_tryshutdown");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
}

void CUIMainWindowEx::OnLoaded() {}

void CUIMainWindowEx::IsWindowShowed() {
  window_showing_ = false;
  if (CheckCanInstall()) {
    base::Threads::Get(base::Threads::FILE)
      ->PostTask(base::Bind(this, &CUIMainWindowEx::FileInit));
    base::MessageLoop::current()->PostDelayedTask(
      base::Bind(this, &CUIMainWindowEx::Rotational),
      base::TimeDelta::FromMilliseconds(1000));
  } else {
    CUIUpDate obj;
    obj.infostyle_ = CUIUpDate::CHECKSYSTEM;
    obj.CreateWnd(m_hWnd);
    obj.DoModal();
    PostMessage(WM_CLOSE, 0, 0);
  }
}

void CUIMainWindowEx::StartShake() {
  tstring strID;
  CBaseElementCtl* pEle = NULL;
  strID = _T("id_static_tryshutdown");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(TRUE);
  }
}

void CUIMainWindowEx::Shaked() {
  tstring strID;
  CBaseElementCtl* pEle = NULL;
  strID = _T("id_static_tryshutdown");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
}

void CUIMainWindowEx::DrawBorder(CDCHandle dc) {
  CTransWindowEx<CUIMainWindowEx>::DrawBorder(dc);
  if (showstate_.bPending_) {
    HBITMAP hBitMap = NULL;
    switch (showstate_.step_) {
    case ShowState::APP1: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP6"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap =
        CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, _T("APP1"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    case ShowState::APP2: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP1"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP2"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    case ShowState::APP3: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP2"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP3"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    case ShowState::APP4: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP3"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP4"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    case ShowState::APP5: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP4"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP5"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    case ShowState::APP6: {
      DUI_Rect rRect(6, 6, 486, 246);
      hBitMap =
        CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
          _T("APP5"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP6"), PNG);
      if (hBitMap) {
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(
          hDC, hBitMap, rRect, DUI_BT_ALPHA, showstate_.degree_);
      }
    } break;
    default:
      break;
    }
  }
  else {
    HBITMAP hBitMap = NULL;
    switch (showstate_.step_) {
    case ShowState::APP1: {
      hBitMap =
        CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, _T("APP1"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    case ShowState::APP2: {
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP2"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    case ShowState::APP3: {
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP3"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    case ShowState::APP4: {
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP4"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    case ShowState::APP5: {
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP5"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    case ShowState::APP6: {
      hBitMap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
        _T("APP6"), PNG);
      if (hBitMap) {
        DUI_Rect rRect(6, 6, 486, 246);
        HDC hDC = dc.m_hDC;
        CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
      }
    } break;
    default:
      break;
    }
  }

  HBITMAP hBitMap =
    CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, _T("APPLOGO"), PNG);
  if (hBitMap) {
    BITMAP bmpFrameData;
    GetObject(hBitMap, sizeof(bmpFrameData), &bmpFrameData);
    DUI_Rect rRect(20 - 4, 20 - 6, 20 - 4 + bmpFrameData.bmWidth,
      20 - 6 + bmpFrameData.bmHeight);
    HDC hDC = dc.m_hDC;
    CGDIResource::GetInstance().GDI_DrawImage(hDC, hBitMap, rRect);
  }
}

void CUIMainWindowEx::FetchURLComplete(std::string res) {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  if (b_user_quit_) {
    return;
  }
  do {
    instep_ = DOWNLOADING;

    std::wstring errorNotice =
      lan::AppLanguage::s_lanArr[lan::AppLanguage::lanIndex_]
      .pstrServiceError;

    std::string fileurl;

    do {
      std::map<std::string, std::string> map_token;
      bool bret = GetJsonTokenByDownLoadUrl(res, map_token);
      if (!bret) {
        CUIUpDate obj;
        obj.infostyle_ = CUIUpDate::NOTICE;
        obj.CreateWnd(m_hWnd);
        obj.SetCustomTitle(errorNotice);
        obj.DoModal();
        PostMessage(WM_CLOSE, 0, 0);
        return;
      }
      std::map<std::string, std::string>::iterator ite;

      ite = map_token.find(std::string("downloadUrl"));
      if (ite != map_token.end() && ite->second.length() &&
        (0 == ite->second.find("http"))) {
        fileurl = ite->second;
      } else {
        CUIUpDate obj;
        obj.infostyle_ = CUIUpDate::NOTICE;
        obj.CreateWnd(m_hWnd);
        obj.SetCustomTitle(errorNotice);
        obj.DoModal();
        PostMessage(WM_CLOSE, 0, 0);
        return;
      }

    } while (false);

    global.m_filedownurl = fileurl;

    break;
  } while (false);
}

void CUIMainWindowEx::FetchFileComplete(bool status, DownCompleteInfo& rest) {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  if (b_user_quit_) {
    return;
  }
  const net::URLFetcher::ErrorType reason = rest.reason_;
  if (!status) {
    if (reason == net::URLFetcher::DISK_NO_SPACE) {
      CUIUpDate obj;
      obj.infostyle_ = CUIUpDate::NOTICE;
      obj.CreateWnd(m_hWnd);
      obj.SetCustomTitle(std::wstring(L"Not enough disk space"));
      obj.DoModal();
      base::Threads::Get(Threads::FILE)
        ->PostTask(
          Bind(this, &CUIMainWindowEx::ClearDownloads, global.m_packPath));
      PostMessage(WM_CLOSE, 0, 0);
    }
    else {
      if (filedownloader_.retrycount_++ != static_cast<char>(-1)) {
        base::Threads::Get(Threads::FILE)
          ->PostTask(Bind(this, &CUIMainWindowEx::ClearDownloads,
            global.m_packPath));
        base::MessageLoop::current()->PostDelayedTask(
          Bind(this, &CUIMainWindowEx::RetryDownLoadAgain),
          base::TimeDelta::FromMilliseconds(1000));
      } else {
        CUIUpDate obj;
        obj.infostyle_ = CUIUpDate::NOTICE;
        obj.CreateWnd(m_hWnd);
        obj.SetCustomTitle(std::wstring(L"Unable connect to server"));
        obj.DoModal();
        base::Threads::Get(Threads::FILE)
          ->PostTask(Bind(this, &CUIMainWindowEx::ClearDownloads,
            global.m_packPath));
        PostMessage(WM_CLOSE, 0, 0);
      }
    }
    return;
  }

  time_download_ = timeGetTime() - time_download_;
  is_pack_complete_ = true;
  if (IsWindowEnabled()) {
    instep_ = INSTALLING;
    DoInstall();
  }
}

void CUIMainWindowEx::FetchFileProgress(double cur) {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  if (b_user_quit_) {
    return;
  }

  CBaseElementCtl* pEle = NULL;
  tstring strID;
  strID = _T("id_progress_download");
  PELEMENT;
  if (pEle) {
    ((CProgressCtl*)pEle)->SetPos(cur * 0.8);
  }
  DWORD now = static_cast<DWORD>(100 * cur * 0.8);
  SetProgressTaskBar(now, 100);
}

void CUIMainWindowEx::DeletePack(const std::wstring& str_path) {
  SystemCommon::FilePathHelper::DeepDeleteFile(str_path);
}

void CUIMainWindowEx::OnMin() {
  ::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CUIMainWindowEx::OnClose() {
  if (instep_ < INSTALLING) {
    CUIUpDate obj;
    obj.CreateWnd(m_hWnd);
    if (obj.DoModal()) {
      DownLoadingAUserQuit();
      b_user_quit_ = true;
      PostMessage(WM_CLOSE, 0, 0);
    }
    else {
      base::Threads::Get(Threads::UI)
        ->PostTask(Bind(this, &CUIMainWindowEx::DoInstall));
    }
  }
  else if (instep_ == INSTALLING) {

    ShakeWindow();
  }
  else {
    OnInstallComplete();
  }
}

void CUIMainWindowEx::OnInstall() {
  if (window_showing_)
    return;

  tstring strID = _T("id_panel_main");
  CBaseElementCtl *pEle = NULL;
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }

  installtype_ = DEFAULT;
  Ready2DownloadZip();
}

void CUIMainWindowEx::OnInstallSelected() {
  tstring strID = _T("id_edit_installpath");
  CBaseElementCtl *pEle = NULL;
  int deskcut = 1;
  int barcut = 1;
  int autorun = 1;
  pEle = _elementManager.Search(strID);
  if (pEle) {
    global.m_strInstallPath = pEle->GetTitle();
    if (global.m_strInstallPath[global.m_strInstallPath.length() - 1] == L'\\') {
      std::wstring strtemp = global.m_strInstallPath.c_str();
      strtemp[strtemp.length() - 1] = L'\0';
      global.m_strInstallPath = strtemp.c_str();
    }
  }
  strID = _T("id_checkbox_shortcut");
  PELEMENT;
  if (pEle) {
    deskcut = ((CCheckBoxCtl *)pEle)->GetCheck() ? 1 : 0;
  }
  strID = _T("id_checkbox_easymenu");
  PELEMENT;
  if (pEle) {
    barcut = ((CCheckBoxCtl *)pEle)->GetCheck() ? 1 : 0;
  }
  strID = _T("id_checkbox_autorun");
  PELEMENT;
  if (pEle) {
    autorun = ((CCheckBoxCtl *)pEle)->GetCheck() ? 1 : 0;
  }
  strID = _T("id_panel_userdef");
  PELEMENT;
  if (!pEle)
    return;
  pEle->SetVisible(FALSE);
  installtype_ = SELECT;
  Ready2DownloadZip();
}

void CUIMainWindowEx::OnAgreement() {
  CUIAgreement obj;
  obj.CreateWnd(m_hWnd);
  if (obj.DoModal()) {
  }
}

void CUIMainWindowEx::OnCheck() {
  if (window_showing_)
    return;

  bool bCheck = false;
  tstring strID = _T("id_checkbox");
  CBaseElementCtl* pEle = _elementManager.Search(strID);
  if (pEle) {
    bCheck = ((CCheckBoxCtl*)pEle)->GetCheck();
  }
  strID = _T("id_install");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    if (bCheck) {
      ((CButtonCtl*)pEle)->Enable(TRUE);
    }
    else {
      ((CButtonCtl*)pEle)->Enable(FALSE);
    }
  }
  strID = _T("id_userinstall");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    if (bCheck) {
      ((CButtonCtl*)pEle)->Enable(TRUE);
    }
    else {
      ((CButtonCtl*)pEle)->Enable(FALSE);
    }
  }
}

void CUIMainWindowEx::OpenInstallDir() {
  BROWSEINFO bi;
  bi.hwndOwner = this->GetHost();
  bi.pidlRoot = NULL;
  bi.pszDisplayName = NULL;
  bi.lpszTitle = TEXT("Please select a folder");
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
  bi.lpfn = NULL;
  bi.lParam = 0;
  bi.iImage = 0;
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
  if (pidl == NULL) {
    return;
  }
  WCHAR szPath[MAX_PATH + 1] = {0};
  if (!SHGetPathFromIDListW(pidl, szPath)) {
    return;
  }

  tstring strID;
  strID = _T("id_edit_installpath");
  CBaseElementCtl *pEle = _elementManager.Search(strID);
  if (pEle) {
    std::wstring strPath = szPath;
    static_cast<CLayeredEditCtl *>(pEle)->SetTitle(strPath);
  }
  strID = _T("id_static_patherror");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  strID = _T("id_btn_installnow");
  PELEMENT;
  if (pEle) {
    static_cast<CButtonCtl *>(pEle)->Enable(TRUE);
  }
}

void CUIMainWindowEx::OnSelected() {
  if (window_showing_)
    return;

  CBaseElementCtl *pEle = NULL;
  tstring strID;
  strID = _T("id_panel_main");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  ::SetWindowPos(m_hWnd, NULL, 0, 0, 492, 472, SWP_NOZORDER | SWP_NOMOVE);
  ChangeSize(492, 472);
  strID = _T("id_panel_userdef");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(TRUE);
    if (GetPathEditError())
      ;
    else {
      strID = _T("id_static_patherror");
      pEle = _elementManager.Search(strID);
      if (pEle)
        pEle->SetVisible(FALSE);
    }
  }
}

void CUIMainWindowEx::OnBackMain() {
  CBaseElementCtl *pEle = NULL;
  tstring strID;
  strID = _T("id_panel_userdef");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  ChangeSize(492, 402);
  strID = _T("id_panel_main");
  pEle = _elementManager.Search(strID);
  if (pEle) {
    pEle->SetVisible(TRUE);
  }
  ::SetWindowPos(m_hWnd, NULL, 0, 0, 492, 402, SWP_NOZORDER | SWP_NOMOVE);
}

void CUIMainWindowEx::OnUseNow() {
  b_user_quit_ = true;

  std::wstring app = global.m_strInstallPath + L"\\" + Util::Base::s2ws(global.appname_);

  STARTUPINFO si = { 0 };
  PROCESS_INFORMATION pi = { 0 };
  WCHAR szProcessCmd[MAX_PATH + 1 + MAX_URL_LEN + MAX_PATH] = { 0 };
  _snwprintf(szProcessCmd, MAX_PATH + MAX_URL_LEN + MAX_PATH, L"\"%s\"",
    app.c_str());
  do {
    std::wstring wstrtmp = szProcessCmd;
    std::string stemstr = Util::Base::ws2s(wstrtmp);

  } while (false);

  bool ProcessCreated = true;

  do {
    if (!SystemCommon::OS::CreateLowIntegrityProcess(szProcessCmd)) {
      ProcessCreated = false;
      break;
    }
  } while (false);
  if (false == ProcessCreated) {
    CreateProcessW(NULL, szProcessCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si,
      &pi);
  }
  PostMessage(WM_CLOSE, 0, 0);
}

void CUIMainWindowEx::OnInstallComplete() {
  b_user_quit_ = true;
  PostMessage(WM_CLOSE, 0, 0);
}

void CUIMainWindowEx::CreateShortCut(LPCTSTR szExePath,
  const ShortCutType Stype) {
  ::CoInitialize(NULL);

  IShellLink* psl;
  LPITEMIDLIST pidl;
  LPMALLOC pShellMalloc;
  std::wstring strDesktopPath;
  std::wstring strStartMenuPath;

  const int nFolder[2] = { CSIDL_DESKTOPDIRECTORY, CSIDL_STARTMENU };

  if (SUCCEEDED(SHGetMalloc(&pShellMalloc))) {
    TCHAR Path[MAX_PATH + 1];
    for (int i = 0; i < 2; ++i) {
      if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, nFolder[i], &pidl))) {
        if (SHGetPathFromIDList(pidl, Path)) {
          if (i == 0)
            strDesktopPath = Path;
          else
            strStartMenuPath = Path;
        }
        pShellMalloc->Free(pidl);
      }
    }
    pShellMalloc->Release();
  }

  std::wstring str;

  switch (Stype) {
  case DESK: {
    str = strDesktopPath;
  } break;
  case STARTMENU: {
    str = strStartMenuPath;
  } break;
  case TASKBAR: {
    std::wstring strTaskBarPath =
      SystemCommon::FilePathHelper::GetAssignPath(CSIDL_APPDATA, TRUE);
    strTaskBarPath +=
      L"\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\TaskBar";
    str = strTaskBarPath;
  } break;
  default: {
    return;
  } break;
  }
  str += _T("\\");
  str += Util::Base::s2ws(global.shortcutname_);
  str += _T(".lnk");

  do {
    HRESULT hr = CoCreateInstance(CLSID_ShellLink,
      NULL, CLSCTX_INPROC_SERVER,
      IID_IShellLink,
      (LPVOID*)&psl);
    if (SUCCEEDED(hr)) {
      IPersistFile* ppf;
      psl->SetPath(szExePath);
      psl->SetArguments(_T(""));
      psl->SetShowCmd(SW_SHOW);
      psl->SetIconLocation(szExePath, 0);
      if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf))) {
        if (SUCCEEDED(ppf->Save(str.c_str(), TRUE)))
          ;
        ppf->Release();
      }
      psl->Release();
    }
  } while (false);

  ::CoUninitialize();

}

void CUIMainWindowEx::SetProgressTaskBar(DWORD now, DWORD total) {
  if (CGlobal::pTaskbar_) {
    CGlobal::pTaskbar_->SetProgressValue(m_hWnd, now, total);
  }
}

void CUIMainWindowEx::Rotational() {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  do {
    if (showstate_.bPending_ == false) {
      showstate_.bPending_ = true;
      switch (showstate_.step_) {
      case ShowState::APP1: {
        showstate_.step_ = ShowState::APP2;
      } break;
      case ShowState::APP2: {
        showstate_.step_ = ShowState::APP3;
      } break;
      case ShowState::APP3: {
        showstate_.step_ = ShowState::APP4;
      } break;
      case ShowState::APP4: {
        showstate_.step_ = ShowState::APP5;
      } break;
      case ShowState::APP5: {
        showstate_.step_ = ShowState::APP6;
      } break;
      case ShowState::APP6: {
        showstate_.step_ = ShowState::APP1;
      } break;
      default:
        break;
      }
      showstate_.degree_ = ShowState::DETEN;
      break;
    }
    if (showstate_.degree_ == ShowState::DEALL) {
      showstate_.bPending_ = false;
      RaiseInvalidate(TRUE);
      base::MessageLoop::current()->PostDelayedTask(
        base::Bind(this, &CUIMainWindowEx::Rotational),
        base::TimeDelta::FromMilliseconds(4000));
      return;
    }
    switch (showstate_.degree_) {
    case ShowState::DETEN: {
      showstate_.degree_ = ShowState::DEFIF;
    } break;
    case ShowState::DEFIF: {
      showstate_.degree_ = ShowState::DESIX;
    } break;
    case ShowState::DESIX: {
      showstate_.degree_ = ShowState::DESEVN;
    } break;
    case ShowState::DESEVN: {
      showstate_.degree_ = ShowState::DEEIGHT;
    } break;
    case ShowState::DEEIGHT: {
      showstate_.degree_ = ShowState::DENIGHT;
    } break;
    case ShowState::DENIGHT: {
      showstate_.degree_ = ShowState::DEALL;
    } break;
    case ShowState::DEALL: {

    } break;
    default:
      break;
    }

  } while (false);
  RaiseInvalidate(TRUE);
  base::MessageLoop::current()->PostDelayedTask(
    base::Bind(this, &CUIMainWindowEx::Rotational),
    base::TimeDelta::FromMilliseconds(100));
}

void CUIMainWindowEx::Ready2DownloadZip() {
  CBaseElementCtl *pEle = NULL;
  tstring strID;
  ChangeSize(492, 402);
  ::SetWindowPos(m_hWnd, NULL, 0, 0, 492, 402, SWP_NOZORDER | SWP_NOMOVE);
  strID = _T("id_panel_download");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(TRUE);
  }
  if (global.m_filedownurl.empty()) {
    base::MessageLoop::current()->PostDelayedTask(
        base::Bind(this, &CUIMainWindowEx::CheckIFTureWillDownLoad),
        base::TimeDelta::FromMilliseconds(200));
  } else {
    CombineRealInstallPath();
    DownLoadingFileUrl();
  }
}

void CUIMainWindowEx::ClearDownloads(const std::wstring str_path) {
  CUIMainWindowEx::DeletePack(str_path);
}

void CUIMainWindowEx::FileInit() {
  CHECK(base::MessageLoop::current() == base::Threads::Get(base::Threads::FILE));
  global.m_packPath += L"\\AppInsPackFileDir\\";
  SystemCommon::FilePathHelper::ForceCreateDir(global.m_packPath.c_str());
  global.m_packPath += L"AppPackZip";
  global.m_packPath += Util::Base::longtocharw(time(nullptr));
  base::Threads::Get(Threads::UI)
    ->PostTask(base::Bind(this, &CUIMainWindowEx::QueryDownLoadUrl));
}

void CUIMainWindowEx::QueryDownLoadUrl() {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  global.downloadurl_ =
    global.downloadurl_ +
    std::string("?version=") +
    Util::Base::ws2s(
      SystemCommon::FileVersionHelper::GetFileVersion(_hInstance)) +
    std::string("&mac_address=") +
    global.m_macAddress +
    std::string("&platform=") +
    global.m_platform;

  urldownloader_.fetcher_->set_download_url(global.downloadurl_);
  urldownloader_.fetcher_->Start();
}

void CUIMainWindowEx::DoInstall() {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  if (is_pack_complete_ && IsWindowEnabled()) {
    instep_ = INSTALLING;

    CBaseElementCtl* pEle = NULL;
    tstring strID;
    strID = _T("id_static_downing");
    PELEMENT;
    if (pEle) {
      std::wstring strTitle =
        lan::AppLanguage::s_lanArr[lan::AppLanguage::lanIndex_]
        .pstrInstalling;
      pEle->SetTitle(strTitle);
      pEle->RaiseInvalidate(TRUE);
    }
    strID = _T("id_static_downing_size");
    PELEMENT;
    if (pEle) {

      pEle->SetVisible(FALSE);
      pEle->RaiseInvalidate(TRUE);
    }

    InstallIng();
  }
}

void CUIMainWindowEx::InstallIng() {
  if (base::MessageLoop::current() != base::Threads::Get(Threads::FILE)) {
    base::Threads::Get(Threads::FILE)
      ->PostTask(base::Bind(this, &CUIMainWindowEx::InstallIng));
    return;
  }
  do {
    bool bIs = false;
    std::wstring exename = Util::Base::s2ws(global.appname_);
    exename = SystemCommon::FilePathHelper::GetFileName(exename);
    DWORD dwPid = SystemCommon::OS::ProcesstoPid(exename.c_str());
    if (dwPid) {
      char szArgs[50] = { 0 };
      _snprintf_s(szArgs, 50, _TRUNCATE, "TASKKILL /F /PID %u", dwPid);
      system(szArgs);
      bIs = true;
    }
    if (bIs) {
      Sleep(100);
    }
  } while (false);
  time_install_ = timeGetTime();
  BOOL res = SystemCommon::FilePathHelper::UnzipFileToPath(global.m_packPath.c_str(),
    global.m_strInstallPath.c_str());
  base::Threads::Get(Threads::UI)
    ->PostTask(base::Bind(this, &CUIMainWindowEx::UnZipComplete, res));
}

void CUIMainWindowEx::CheckIFTureWillDownLoad() {
  if (b_user_quit_) {
    return;
  }
  if (global.m_filedownurl.empty()) {
    base::MessageLoop::current()->PostDelayedTask(
      base::Bind(this, &CUIMainWindowEx::CheckIFTureWillDownLoad),
      base::TimeDelta::FromMilliseconds(200));
  } else {
    CombineRealInstallPath();
    DownLoadingFileUrl();
  }
}

void CUIMainWindowEx::DownLoadingAUserQuit() {
  CHECK(base::Threads::Get(Threads::UI) == base::MessageLoop::current());
  urldownloader_.fetcher_->Stop();
  filedownloader_.fetcher_->Stop();
}

void CUIMainWindowEx::CombineRealInstallPath() {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  global.m_strInstallPath = global.m_strInstallPath + L"\\" + Util::Base::s2ws(CGlobal::appDirName_);
  base::Threads::Get(Threads::FILE)
      ->PostTask(base::Bind(this, &CUIMainWindowEx::ForceCreateInstallPath,
                            global.m_strInstallPath));
}

void CUIMainWindowEx::ForceCreateInstallPath(std::wstring path) {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::FILE));
  path += L"\\";
  SystemCommon::FilePathHelper::ForceCreateDir(path.c_str());
}

bool CUIMainWindowEx::CheckCanInstall() {
  const DWORD c_memsize = 200;
  const DWORD c_freeharddisk = 1000;
  const DWORD c_cpu_type = PROCESSOR_INTEL_486;
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  DWORD cpu_type = si.dwProcessorType;
  DWORD cpu_core_num = si.dwNumberOfProcessors;

  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  GlobalMemoryStatusEx(&statex);
  DWORD memsize = (DWORDLONG)statex.ullTotalPhys / (1000 * 1000);

  ULARGE_INTEGER uli_free_bytes_available;
  ULARGE_INTEGER uli_total_bytes;
  ULARGE_INTEGER uli_total_free_bytes;
  GetDiskFreeSpaceExW(global.m_packPath.c_str(), &uli_free_bytes_available,
    &uli_total_bytes, &uli_total_free_bytes);
  DWORD freedisksize = uli_total_free_bytes.QuadPart / (1024 * 1024);
  if (memsize >= c_memsize && cpu_type >= c_cpu_type &&
    freedisksize >= c_freeharddisk && cpu_core_num >= 2)
    return true;
  else
    return false;
}

void CUIMainWindowEx::RetryDownLoadAgain() {
  if (b_user_quit_) {
    return;
  }
  global.m_packPath = SystemCommon::FilePathHelper::GetPath(global.m_packPath);
  global.m_packPath += L"AppPackZip";
  global.m_packPath += Util::Base::longtocharw(time(NULL));
  filedownloader_.fetcher_ = net::URLFetcher::Create(
    net::URLFetcher::Params(global.m_filedownurl, global.m_packPath), &filedownloader_);
  filedownloader_.fetcher_->Start();
}

bool CUIMainWindowEx::GetPathEditError() {
  CBaseElementCtl* pEle = NULL;
  tstring strID;
  strID = _T("id_edit_installpath");
  pEle = _elementManager.Search(strID);
  if (!pEle)
    return false;
  std::wstring title = pEle->GetTitle();
  std::wregex p(
    L"^[a-zA-Z]:\\\\{1}(?:[^/:*?\"<>|])*[^./:*?\"<>|]{1}$|[a-zA-Z]:\\\\");
  bool ism = std::regex_match(title, p);
  bool tipshow = false;
  if (ism) {
    if (!(-1 == title.find(L"\\\\") && -1 == title.find(L".\\") && -1 == title.find(L"\\."))) {
      tipshow = true;
    }
  } else {
    tipshow = true;
  }
  return tipshow;
}

void CUIMainWindowEx::DownLoadingFileUrl() {
  if (UNKNOW == downloadway_) {
    base::MessageLoop::current()->PostDelayedTask(
      base::Bind(this, &CUIMainWindowEx::DownLoadingFileUrl),
      base::TimeDelta::FromMilliseconds(200));
    return;
  }
  time_download_ = timeGetTime();
  if (downloadway_ == SINGLE) {
    filedownloader_.fetcher_->set_download_url(global.m_filedownurl);
    filedownloader_.fetcher_->set_file_loadpath(global.m_packPath);
    filedownloader_.fetcher_->Start();
  }
}

void CUIMainWindowEx::UnZipComplete(BOOL res) {
  CHECK(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  time_install_ = timeGetTime() - time_install_;
  if (FALSE == res) {
    base::MessageLoop::current()->PostQuitTask(
      base::Bind(CUIMainWindowEx::DeletePack, global.m_packPath));
    CUIUpDate obj;
    obj.infostyle_ = CUIUpDate::NOTICE;
    obj.CreateWnd(m_hWnd);
    obj.SetCustomTitle(std::wstring(L"File is occupied"));
    obj.DoModal();
    PostMessage(WM_CLOSE, 0, 0);
    return;
  }
  CBaseElementCtl* pEle = NULL;
  tstring strID;
  strID = _T("id_progress_download");
  PELEMENT;
  if (pEle) {
    ((CProgressCtl*)pEle)->SetPos((0.95));
    SetProgressTaskBar(95, 100);
  }
  strID = _T("id_panel_download");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  strID = _T("id_panel_complete");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(TRUE);
  }
  if (CGlobal::pTaskbar_) {
    CGlobal::pTaskbar_->SetProgressState(m_hWnd, TBPF_NOPROGRESS);
  }
  strID = _T("id_static_tryshutdown");
  PELEMENT;
  if (pEle) {
    pEle->SetVisible(FALSE);
  }
  std::wstring app_path = global.m_strInstallPath + L"\\" + Util::Base::s2ws(global.appname_);
  strID = _T("id_checkbox_shortcut");
  PELEMENT;
  if (pEle) {
    if (((CCheckBoxCtl*)pEle)->GetCheck()) {
      CreateShortCut(app_path.c_str(), DESK);
    }
  }
  strID = _T("id_checkbox_autorun");
  PELEMENT;
  if (pEle) {
    if (((CCheckBoxCtl*)pEle)->GetCheck()) {
      const std::wstring appName = Util::Base::s2ws(global.appDirName_);
      std::wstring regKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
      if (SystemCommon::OS::IsWow64()) {
        SystemCommon::OS::SetRegValue(HKEY_CURRENT_USER, regKey.c_str(),
          appName.c_str(), app_path);
      } else {
        SystemCommon::OS::SetRegValue(HKEY_CURRENT_USER, regKey.c_str(),
          appName.c_str(), app_path);
      }
    }
  }
  if (Util::Base::GetAccurateOSVersion() >= WINDOWS_7) {
    strID = _T("id_checkbox_easymenu");
    PELEMENT;
    if (pEle) {
      if (((CCheckBoxCtl*)pEle)->GetCheck()) {
        CreateShortCut(app_path.c_str(), TASKBAR);
      }
    }
  }
  //start memu
  CreateShortCut(app_path.c_str(), STARTMENU);
  instep_ = INSTALLED;

  if (CGlobal::openurl_.length() > 0) {
    ShellExecuteA(NULL, "open", CGlobal::openurl_.c_str(), NULL, NULL, SW_SHOWNORMAL);
  }

}

bool CUIMainWindowEx::GetJsonTokenByDownLoadUrl(
    const std::string &res, std::map<std::string, std::string> &map_token) {

  if (res.length() == 0 || res[0] == 0)
    return false;

  Json::Features features = Json::Features::strictMode();
  Json::Reader reader(features);
  Json::Value root;
  bool parsingSuccessful = false;

  parsingSuccessful = reader.parse(res, root, true);

  if (parsingSuccessful && root.isObject()) {
    Json::Value::Members keys = root.getMemberNames();
    Json::Value::Members::iterator it = keys.begin();
    std::string strItem;
    for (; it != keys.end(); ++it) {
      Json::Value valDefault("");
      std::string &strKey = *it;
      Json::Value item = root.get(strKey, valDefault);

      strItem = "downloadUrl";

      if (strKey == strItem) {
        if (item.isString()) {
          map_token.insert(make_pair(strItem, item.asString()));
        }
        continue;
      }
    }
    return true;
  }
  return false;
}
