#pragma once

#include "util/system.h"
#include "DUIButton.h"
#include "DUICheckBox.h"
#include "DUIElement.h"
#include "DUIElementsMgr.h"
#include "DUIHyperLink.h"
#include "DUILayeredEdit.h"
#include "DUIOptionLine.h"
#include "DUIPanel.h"
#include "DUIProgress.h"
#include "DUIStatic.h"
#include "DUITab.h"
#include "DUITabList.h"
#include "UpdateWindowBaseEx.h"

enum enumShakeWindow { SHAKEWINDOW_ERROR = 0 };

#define SHAKEWINDOW 0x10

template <class T>
class __declspec(novtable) CTransWindowEx : public CMessageFilter,
                                            public CBaseElementCtl,
                                            public CUpdateWindowBaseEx<T> {
public:
  CTransWindowEx();
  virtual ~CTransWindowEx();
  BEGIN_MSG_MAP(CTransWindowEx)
  CHAIN_MSG_MAP(CUpdateWindowBaseEx<T>)
  MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
  MESSAGE_HANDLER(WM_SIZE, OnSize)
  MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
  MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
  MESSAGE_HANDLER(WM_SYSKEYDOWN, OnSysKeyDown)
  MESSAGE_HANDLER(WM_SYSKEYUP, OnSysKeyUp)
  MESSAGE_HANDLER(WM_NCLBUTTONDBLCLK, OnNcLButtonDblClk)
  MESSAGE_HANDLER(WM_NCMOUSEMOVE, OnNcMouseMove)
  MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
  MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
  MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
  MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUP)
  MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
  MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
  MESSAGE_HANDLER(WM_COMMAND, OnCommand)
  MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
  MESSAGE_HANDLER(WM_MOVE, OnMove)
  MESSAGE_HANDLER(WM_CLOSE, OnClose)
  MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
  MESSAGE_HANDLER(WM_GETICON, OnGetIcon)
  MESSAGE_HANDLER(WM_TIMER, OnTimer)
  MESSAGE_HANDLER(WM_MSG_TIPS, OnTips)
  MESSAGE_HANDLER(WM_MSG_CURSOR, OnSetCursor)
  MESSAGE_HANDLER(WM_MOUSEHWHEEL, OnMouseWheel)
  MESSAGE_HANDLER(WM_ASYNC_CALL_LUP, OnAsynCall_LUp)
  MESSAGE_HANDLER(WM_ASYNC_CALL_RUP, OnAsynCall_RUp)
  MESSAGE_HANDLER(WM_ASYNC_CALL_ENTERKEY, OnAsynCall_EnterKey)
  MESSAGE_HANDLER(WM_ASYNC_CALL_TABKEY, OnAsynCall_TabKey)
  MESSAGE_HANDLER(WM_ASYNC_CALL_LDOWN, OnAsynCall_LDown)
  MESSAGE_HANDLER(WM_MSG_ACTIVE, OnElementActive)
  MESSAGE_HANDLER(WM_PATH_ILLCHARACTER, OnPathIllCharacter)
  END_MSG_MAP()

  BOOL DUICreate(HWND hParentWnd, LPCTSTR lpszXmlWidow);
  BOOL DUICreateW(HWND hParentWnd, WCHAR *lpszXml);
  BOOL DUICreate(HWND hParentWnd, DWORD dwExStyle = 0);
  BOOL DoModal();
  virtual void DrawBg(HDC hDC);
  virtual void OnDraw(HDC hDC);

  virtual void DrawFrame(CDCHandle dc);

  virtual void DrawBorder(CDCHandle dc);

  virtual void DrawTitle(CDCHandle dc);

  virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                           BOOL &bHandled);
  virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                         BOOL &bHandled);
  virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled);
  virtual LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                          BOOL &bHandled);
  virtual LRESULT OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled);
  virtual LRESULT OnSysKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled);
  virtual LRESULT OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled);
  virtual LRESULT OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled);
  virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled);
  virtual LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled);
  virtual LRESULT OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled);
  virtual LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled);
  virtual LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled);
  virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                          BOOL &bHandled);
  virtual LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                         BOOL &bHandled);
  virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled);
  virtual LRESULT OnGetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled);
  virtual LRESULT OnSetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled);
  virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                          BOOL &bHandled);
  virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnTips(UINT uMsg, WPARAM wParam, LPARAM lParam,
                         BOOL &bHandled);
  virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled);
  virtual LRESULT OnAsynCall_LUp(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL &bHandled);
  virtual LRESULT OnAsynCall_RUp(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL &bHandled);
  virtual LRESULT OnAsynCall_EnterKey(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled);
  virtual LRESULT OnAsynCall_TabKey(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled);
  virtual LRESULT OnAsynCall_LDown(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled);
  virtual LRESULT OnElementActive(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled);
  virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled);
  virtual LRESULT OnPathIllCharacter(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled);
  virtual BOOL PreTranslateMessage(MSG *pMsg);

  virtual void SetWndPos(DUI_Point &pt, DUI_Size &sz);

  virtual void OnLoad();
  virtual void OnLoaded();

  virtual void OnExit();
  virtual void OnLoadIcon();

  virtual DWORD OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD);
  virtual DWORD OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD);
  virtual DWORD OnSubItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD);

  void ChangeSize(int w, int h);

protected:
  virtual void OnDynamicCreate();

  virtual void SetSkinedWindowRgn(BOOL bZoomed);
  virtual void CreateElement(XmlElementPtr element);
  virtual void SetWndStyle();

  BOOL _bIsWin7;

  BOOL _bActived;

  int _nTimerID;

  CAnyElementsMgr _elementManager;

  CToolTipCtrl m_tips;

  BOOL _bSupportDblTitle;

  BOOL _bSupportChangeSize;

  int _nMinWidth;
  int _nMinHeight;
  int _nTitleHeight;
  BOOL _bIsCenter;

  tstring _strXmlWindow;

  tstring _strIconFile;

  HBITMAP _bmpSingleBack;

  HBITMAP _bmpArrow;

  HBITMAP _bmpBackgroundTL;
  HBITMAP _bmpBackgroundT;
  HBITMAP _bmpBackgroundTR;
  HBITMAP _bmpBackgroundL;
  HBITMAP _bmpBackgroundC;
  HBITMAP _bmpBackgroundR;
  HBITMAP _bmpBackgroundBL;
  HBITMAP _bmpBackgroundB;
  HBITMAP _bmpBackgroundBR;

  HBITMAP _bmpFrame;
  DUI_Rect _rcFrameCorner;
  HBITMAP _bmpFrameBack;
  DUI_Rect _rcFrameBackCorner;
  DUI_Rect _rcFrameBackPos;

  HBITMAP _bmpResize;

  HBITMAP _bmpTitle;

  HBITMAP _bmpLogo;

  XmlElementPtr m_pXmlWindow;

  BOOL _bOK;

  bool bshaking_;

protected:
  virtual void StartShake() {}
  virtual void Shaked() {}

public:
  void ShakeWindow(int reason = 0);
  RECT m_WindowRect;
  int m_count;
  int m_Lrud;
  void LmoveRect(/*_In_*/ const RECT &i_rc, long delta, /*_Out_*/ RECT &o_rc) {
    o_rc = i_rc;
    o_rc.left -= delta;
    o_rc.right -= delta;
  }
  void UmoveRect(/*_In_*/ const RECT &i_rc, long delta, /*_Out_*/ RECT &o_rc) {
    o_rc = i_rc;
    o_rc.top -= delta;
    o_rc.bottom -= delta;
  }
  void RmoveRect(/*_In_*/ const RECT &i_rc, long delta, /*_Out_*/ RECT &o_rc) {
    o_rc = i_rc;
    o_rc.left += delta;
    o_rc.right += delta;
  }
  void DmoveRect(/*_In_*/ const RECT &i_rc, long delta, /*_Out_*/ RECT &o_rc) {
    o_rc = i_rc;
    o_rc.top += delta;
    o_rc.bottom += delta;
  }
};

template <class T>
LRESULT CTransWindowEx<T>::OnAsynCall_LUp(UINT nMsg, WPARAM wParam,
                                          LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_LeftUp();
  return S_OK;
}

template <typename T>
LRESULT CTransWindowEx<T>::OnAsynCall_LDown(UINT nMsg, WPARAM wParam,
                                            LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_LeftDown();
  return S_OK;
}

template <class T>
LRESULT CTransWindowEx<T>::OnAsynCall_RUp(UINT nMsg, WPARAM wParam,
                                          LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_RightUp();

  return S_OK;
}

template <class T>
LRESULT CTransWindowEx<T>::OnAsynCall_EnterKey(UINT nMsg, WPARAM wParam,
                                               LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_EnterKey();
  return S_OK;
}

template <class T>
LRESULT CTransWindowEx<T>::OnAsynCall_TabKey(UINT nMsg, WPARAM wParam,
                                             LPARAM lParam, BOOL &bHandled) {
  return S_OK;
}

template <class T> CTransWindowEx<T>::CTransWindowEx() : bshaking_(false) {
  _bIsWin7 = FALSE;
  _bActived = FALSE;
  _nTimerID = 0;
  _transparent = TRUE;
  _nMinWidth = 307;
  _nMinHeight = 451;
  _nTitleHeight = 20;
  _bmpArrow = NULL;
  _bmpFrame = NULL;
  _bmpFrameBack = NULL;
  _bmpResize = NULL;
  _bmpTitle = NULL;
  _bmpLogo = NULL;
  _bIsCenter = FALSE;
  _bSupportChangeSize = FALSE;
  _bSupportDblTitle = FALSE;
  _bOK = FALSE;
  m_count = 0;
  m_Lrud = 0;
}

template <class T> CTransWindowEx<T>::~CTransWindowEx() { Detach(); }

template <class T>
BOOL CTransWindowEx<T>::DUICreateW(HWND hParentWnd, WCHAR *lpszXml) {

  CXmlHelper helper;
  helper.LoadStringW(lpszXml);

  m_pXmlWindow = helper.GetRoot();
  if (m_pXmlWindow == NULL) {
    return FALSE;
  }
  if (helper.GetElementName(m_pXmlWindow).compare(_T("window"))) {
    return FALSE;
  }

  CreateElement(m_pXmlWindow);

  SetWndPos(_location, _size);

  SetWndStyle();

  DUI_Rect rcWnd(_location, _size);
  _CUpdateWindowBaseEx_WndRect_con = rcWnd;

  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | DS_MODALFRAME;
  _style |= DS_MODALFRAME;
  HWND hWnd = Create(hParentWnd, rcWnd, _title.c_str(), _style, _ex_style);

  if (hWnd == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  ::SetForegroundWindow(m_hWnd);

  OnLoaded();
  return TRUE;
}

template <class T>
BOOL CTransWindowEx<T>::DUICreate(HWND hParentWnd, LPCTSTR lpszXmlWidow) {
  _strXmlWindow = lpszXmlWidow;

  CXmlHelper helper;
  helper.loadResFile(_hInstance, _strXmlWindow.c_str(), _T("xml"));

  m_pXmlWindow = helper.GetRoot();
  if (m_pXmlWindow == NULL) {
    return FALSE;
  }
  if (helper.GetElementName(m_pXmlWindow).compare(_T("window"))) {
    return FALSE;
  }

  CreateElement(m_pXmlWindow);

  SetWndPos(_location, _size);

  SetWndStyle();

  DUI_Rect rcWnd(_location, _size);
  _CUpdateWindowBaseEx_WndRect_con = rcWnd;

  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | DS_MODALFRAME;
  _style |= DS_MODALFRAME;
  HWND hWnd = Create(hParentWnd, rcWnd, _title.c_str(), _style, _ex_style);

  if (hWnd == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  ::SetForegroundWindow(m_hWnd);

  OnLoaded();

  return TRUE;
}

template <class T>
BOOL CTransWindowEx<T>::DUICreate(HWND hParentWnd, DWORD dwExStyle) {
  SetWndPos(_location, _size);

  DUI_Rect rcWnd(_location, _size);
  _CUpdateWindowBaseEx_WndRect_con = rcWnd;
  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | DS_MODALFRAME;

  if (Create(hParentWnd, rcWnd, _T(""), dwStyle, dwExStyle) == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  if (dwExStyle & WS_EX_TOOLWINDOW) {
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
  } else {
    ::SetForegroundWindow(m_hWnd);
  }

  return TRUE;
}

template <class T>
void CTransWindowEx<T>::SetWndPos(DUI_Point &pt, DUI_Size &sz) {}

template <class T> BOOL CTransWindowEx<T>::DoModal() {
  // todo
  return _bOK;
}

template <class T>
LRESULT CTransWindowEx<T>::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  _host = m_hWnd;

  if (_bIsCenter) {
    CenterWindow();
    GetWindowRect(&_CUpdateWindowBaseEx_WndRect_con);
  }

  _elementManager.InitOwner(m_hWnd);

  OnDynamicCreate();

  m_tips.Create(m_hWnd);
  m_tips.Activate(FALSE);
  m_tips.AddTool(m_hWnd);
  m_tips.SetMaxTipWidth(260);

  OnLoad();

  OnLoadIcon();

  return 0;
}

template <class T> void CTransWindowEx<T>::OnLoadIcon() {
  static HICON hIcon = (HICON)::LoadImage(
      _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
      ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON),
      LR_DEFAULTCOLOR);
  if (hIcon) {
    SetIcon(hIcon, TRUE);
    SetIcon(hIcon, FALSE);
  }
}

template <class T> void CTransWindowEx<T>::OnDynamicCreate() {
  CXmlHelper helper;
  std::vector<XmlElementPtr> pAllElement = helper.GetChild(m_pXmlWindow);
  std::vector<XmlElementPtr>::iterator ite = pAllElement.begin();
  tstring strEleName;
  CBaseElementCtl *pElement = NULL;
  for (ite; ite != pAllElement.end(); ite++) {

    CXmlHelper helper;
    strEleName = helper.GetElementName(*ite);

    if (!strEleName.compare(_T("button"))) {
      pElement = new CButtonCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }
    if (!strEleName.compare(_T("tab"))) {
      pElement = new CTabCtl(m_hWnd);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("static"))) {
      pElement = new CStaticCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("link"))) {
      pElement = new CHyperLinkCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("layedit"))) {
      pElement = new CLayeredEditCtl(m_hWnd);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("panel"))) {
      pElement = new CPanelCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }
    if (!strEleName.compare(_T("progress"))) {
      pElement = new CProgressCtl(m_hWnd);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
    }

    if (!strEleName.compare(_T("checkbox"))) {
      pElement = new CCheckBoxCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("tablist"))) {
      pElement = new CTabListCtl(_host, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }
    if (!strEleName.compare(_T("optionline"))) {
      pElement = new COptionLineCtl(_host, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
    }
  }
}

template <class T>
void CTransWindowEx<T>::CreateElement(XmlElementPtr element) {
  GetElementStyle(element);

  _nMinWidth = GetIntegerAttribute(element, "minWidth");
  _nMinHeight = GetIntegerAttribute(element, "minHeight");
  _nTitleHeight = GetIntegerAttribute(element, "titleHeight");
  _strIconFile = GetStringAttribute(element, "icon");
  _bmpFrame = GetBitmapAttribute(element, "frame");
  _rcFrameCorner = GetRectAttribute(element, "frameCorner");
  _bmpFrameBack = GetBitmapAttribute(element, "frameBack");
  _rcFrameBackCorner = GetRectAttribute(element, "frameBackCorner");
  _rcFrameBackPos = GetRectAttribute(element, "frameBackPos");
  _bIsCenter = GetIntegerAttribute(element, "center") == 1 ? TRUE : FALSE;

  GetIntegerAttribute(element, "topmost") ? (_ex_style |= WS_EX_TOPMOST)
                                          : _ex_style;

  _bmpResize = GetBitmapAttribute(element, "resize");
  _bmpTitle = GetBitmapAttribute(element, "title");
  _bmpLogo = GetBitmapAttribute(element, "logo");
}

template <class T> void CTransWindowEx<T>::DrawBg(HDC hDC) {
  DrawBorder(hDC);
  DrawTitle(hDC);
}

template <class T> void CTransWindowEx<T>::OnDraw(HDC hDC) {
  _elementManager.OnPaint(hDC);
}

template <class T> void CTransWindowEx<T>::DrawFrame(CDCHandle dc) {
  RECT rcClient;
  GetClientRect(&rcClient);

  HDC hDC = dc.m_hDC;

  CGDIResource::GetInstance().GDI_DrawRectangle(
      hDC, &rcClient, RGB(110, 150, 177), RGB(182, 234, 253));
}

template <class T> void CTransWindowEx<T>::DrawBorder(CDCHandle dc) {
  if (!_bmpFrame)
    return;

  DUI_Rect wndRect(0, 0, 0, 0);

  if (_bCUpdateWindowBaseEx_TransShow) {
    HDC hDC = dc.m_hDC;

    BITMAP bmpFrameData;
    GetObject(_bmpFrame, sizeof(bmpFrameData), &bmpFrameData);

    wndRect.right = _CUpdateWindowBaseEx_WndRect_con.Width();
    wndRect.bottom = _CUpdateWindowBaseEx_WndRect_con.Height();

    CGDIResource::GetInstance().GDI_DrawImageStretch(
        hDC, _bmpFrame, wndRect,
        DUI_Rect(0, 0, bmpFrameData.bmWidth, bmpFrameData.bmHeight), 255,
        _rcFrameCorner);
  } else {
    GetClientRect(&wndRect);

    HDC hDC = dc.m_hDC;

    BITMAP bmpFrameData;
    GetObject(_bmpFrame, sizeof(bmpFrameData), &bmpFrameData);

    CGDIResource::GetInstance().GDI_DrawImageStretch(
        hDC, _bmpFrame, wndRect,
        DUI_Rect(0, 0, bmpFrameData.bmWidth, bmpFrameData.bmHeight), 255,
        _rcFrameCorner);
  }
}

template <class T> void CTransWindowEx<T>::DrawTitle(CDCHandle dc) {
  BITMAP bmpLogoInfor;
  if (_bmpLogo) {
    GetObject(_bmpLogo, sizeof(bmpLogoInfor), &bmpLogoInfor);

    int nLeft = 12;
    int nTop = 10;
    if (nTop < 0)
      nTop = 0;
    CGDIResource::GetInstance().GDI_DrawImage(
        dc.m_hDC, _bmpLogo, nLeft, nTop, bmpLogoInfor.bmWidth,
        bmpLogoInfor.bmHeight, DUI_BT_ALPHA);
  }

  if (_title.size() > 0) {
    RECT rcClient = {0, 0, 0, 0};
    if (_bCUpdateWindowBaseEx_TransShow) {
      rcClient.right = _CUpdateWindowBaseEx_WndRect_con.Width();
      rcClient.bottom = _CUpdateWindowBaseEx_WndRect_con.Height();
    } else
      GetClientRect(&rcClient);

    RECT rcText;
    rcText.left = _bmpLogo != NULL ? 8 + bmpLogoInfor.bmWidth + 8 : 14;
    rcText.right = rcClient.right - 1;
    rcText.top = 9;
    rcText.bottom = _nTitleHeight > 0 ? _nTitleHeight : rcText.top + 20;
    if (1) {
      CGDIResource::GetInstance().GDI_DrawText_Trans(
          dc.m_hDC, _title.c_str(), &rcText, RGB(255, 255, 255));
    }
  }
}

template <class T>
LRESULT CTransWindowEx<T>::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  if (bshaking_) {
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

  int nTop = 4;
  int nLeft = 4;
  int nRight = 4;
  int nBottom = 4;

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

template <class T>
LRESULT CTransWindowEx<T>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  if (_bCUpdateWindowBaseEx_TransShow)
    return 0;

  int cx = GET_X_LPARAM(lParam);
  int cy = GET_Y_LPARAM(lParam);

  _elementManager.OnSizeChanged(_size, cx, cy);
  _size.cx = cx;
  _size.cy = cy;
  return 0;
}

template <class T> void CTransWindowEx<T>::ChangeSize(int a, int b) {
  _size.cx = a;
  _size.cy = b;
  _elementManager.OnSizeChanged(_size, a, b);
}

template <class T>
LRESULT CTransWindowEx<T>::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) {
  _elementManager.OnKeyDown((DWORD)wParam);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  _elementManager.OnKeyUp((DWORD)wParam);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL &bHandled) {

  _elementManager.OnKeyDown((DWORD)wParam);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnSysKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled) {
  _elementManager.OnKeyUp((DWORD)wParam);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam,
                                           LPARAM lParam, BOOL &bHandled) {
  MINMAXINFO *lpMMI = (MINMAXINFO *)lParam;
  lpMMI->ptMinTrackSize.x = _nMinWidth;
  lpMMI->ptMinTrackSize.y = _nMinHeight;

  RECT rt;
  SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rt, 0);

  lpMMI->ptMaxPosition.x = rt.left;
  lpMMI->ptMaxPosition.y = rt.top;

  lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x = rt.right - rt.left;
  lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y = rt.bottom - rt.top;

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnNcLButtonDblClk(UINT uMsg, WPARAM wParam,
                                             LPARAM lParam, BOOL &bHandled) {
  if (_bSupportDblTitle) {
    if (IsZoomed())
      ::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    else
      ::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  }

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnNcMouseMove(UINT uMsg, WPARAM wParam,
                                         LPARAM lParam, BOOL &bHandled) {
  if (IsZoomed() && !_bIsWin7) {
    ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);
  } else {
    ModifyStyle(WS_SYSMENU, 0, 0);
  }
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnGetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) {
  ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);
  bHandled = FALSE;
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnSetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) {
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnLButtonDown(UINT uMsg, WPARAM wParam,
                                         LPARAM lParam, BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseDown(uMsg, pt);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseUp(uMsg, pt);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnRButtonDown(UINT uMsg, WPARAM wParam,
                                         LPARAM lParam, BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseRUp(uMsg, pt);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  MSG msg;
  msg.hwnd = m_hWnd;
  msg.message = WM_MOUSEMOVE;
  msg.wParam = 0;
  msg.lParam = GetMessagePos();
  if (IsWindow())
    m_tips.RelayEvent(&msg);

  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseMove(uMsg, pt);

  TrackMouseLeave(m_hWnd);

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL &bHandled) {
  bHandled = TRUE;
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseMove(uMsg, pt);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) {
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled) {
  if (!IsWindow()) {
    return 0;
  }
  UINT nState = (UINT)wParam;
  if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE) {
    _bActived = TRUE;
  } else {
    _bActived = FALSE;
  }

  RECT rcClient;
  GetClientRect(&rcClient);
  rcClient.bottom = rcClient.top + _nTitleHeight;
  InvalidateRect(&rcClient);

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  this->OnExit();
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) {
  if (::IsWindow((m_tips.m_hWnd))) {
    m_tips.DestroyWindow();
  }
  m_tips.m_hWnd = NULL;
  KillTimer(SHAKEWINDOW);
  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnPathIllCharacter(UINT uMsg, WPARAM wParam,
                                              LPARAM lParam, BOOL &bHandled) {
  return 0;
}

template <class T> void CTransWindowEx<T>::OnExit() {
  _elementManager.Dispose();
}

template <class T>
LRESULT CTransWindowEx<T>::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  _elementManager.OnMove();
  return 0;
}

template <class T> void CTransWindowEx<T>::SetSkinedWindowRgn(BOOL bZoomed) {
  if (bZoomed) {
    ::SetWindowRgn(m_hWnd, NULL, TRUE);
  } else {
    RECT rect;
    GetWindowRect(&rect);
    OffsetRect(&rect, -rect.left, -rect.top);
    const int nRgnCornerSize = 5;
    HRGN hRgn1 =
        CreateRoundRectRgn(rect.left, rect.top, rect.right + 1, rect.bottom,
                           nRgnCornerSize, nRgnCornerSize);
    HRGN hRgn2 = CreateRectRgn(rect.left, rect.top + nRgnCornerSize, rect.right,
                               rect.bottom);
    HRGN hRgn = CreateRectRgn(rect.left, rect.top + nRgnCornerSize, rect.right,
                              rect.bottom);

    CombineRgn(hRgn, hRgn1, hRgn2, RGN_OR);
    DeleteObject(hRgn1);
    DeleteObject(hRgn2);

    ::SetWindowRgn(m_hWnd, hRgn, TRUE);
  }
}

template <class T> BOOL CTransWindowEx<T>::PreTranslateMessage(MSG *pMsg) {
  if (::IsWindow(m_tips.m_hWnd)) {
    m_tips.RelayEvent(pMsg);
  }

  return FALSE;
}

template <class T>
LRESULT CTransWindowEx<T>::OnTips(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  BOOL bTips = (BOOL)wParam;
  if (bTips) {
    LPCTSTR lpszText = (LPCTSTR)lParam;

    m_tips.UpdateTipText(lpszText, m_hWnd);
    m_tips.Activate(TRUE);
  } else {
    m_tips.Activate(FALSE);
  }

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnElementActive(UINT nMsg, WPARAM wParam,
                                           LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  _elementManager.SetActive(pOwner);

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL &bHandled) {
  HCURSOR hCur = (HCURSOR)wParam;
  if (hCur == LoadCursor(NULL, IDC_HAND)) {
    ::SetClassLong(m_hWnd, GCL_HCURSOR,
                   (long)LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
  } else {
    ::SetClassLong(m_hWnd, GCL_HCURSOR,
                   (long)LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
  }

  return 0;
}

template <class T>
LRESULT CTransWindowEx<T>::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL &bHandled) {
  UINT fwKeys = LOWORD(wParam); /* key flags */
  short zDelta = (short)HIWORD(wParam);
  /* wheel rotation */
  int xPos = (short)LOWORD(lParam);
  /* horizontal position of pointer */
  int yPos = (short)HIWORD(lParam);
  /* vertical position of pointer */
  CPoint pt(xPos, yPos);
  ScreenToClient(&pt);
  _elementManager.OnMouseWheel(fwKeys, zDelta, pt);
  return 0;
}

template <class T> void CTransWindowEx<T>::OnLoad() {}

template <class T> void CTransWindowEx<T>::OnLoaded() {}

template <class T>
DWORD CTransWindowEx<T>::OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_NOTIFYITEMDRAW;
}

template <class T>
DWORD CTransWindowEx<T>::OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_DODEFAULT;
}

template <class T>
DWORD CTransWindowEx<T>::OnSubItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_DODEFAULT;
}

template <class T> void CTransWindowEx<T>::ShakeWindow(int reason) {
  SystemCommon::Window::StretchForegroundWindow(m_hWnd);
  ::SetFocus(m_hWnd);
  if (reason == SHAKEWINDOW_ERROR) {
    StartShake();
    bshaking_ = true;
    GetWindowRect(&m_WindowRect);
    KillTimer(SHAKEWINDOW);
    m_count = 11;
    m_Lrud = 0;
    SetTimer(SHAKEWINDOW, 45, NULL);
    return;
  }
}

template <class T>
LRESULT CTransWindowEx<T>::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  switch (wParam) {
  case ACTIVE_TIMER: {
    _elementManager.DrawCursor();
  } break;
  case SHAKEWINDOW: {
    if (m_count <= 0) {
      Shaked();
      bshaking_ = false;
      KillTimer(SHAKEWINDOW);
      m_count = 0;
      SetWindowPos(0, &m_WindowRect, SWP_NOZORDER | SWP_NOSIZE);
    } else {
      m_count--;
      RECT rc;
      int delta = 2;
      if (m_Lrud == 0) {
        m_Lrud++;
        LmoveRect(m_WindowRect, delta, rc);
        SetWindowPos(0, &rc, SWP_NOZORDER | SWP_NOSIZE);
      } else if (m_Lrud == 1) {
        m_Lrud++;
        UmoveRect(m_WindowRect, delta, rc);
        SetWindowPos(0, &rc, SWP_NOZORDER | SWP_NOSIZE);
      } else if (m_Lrud == 2) {
        m_Lrud++;
        RmoveRect(m_WindowRect, delta, rc);
        SetWindowPos(0, &rc, SWP_NOZORDER | SWP_NOSIZE);
      } else if (m_Lrud == 3) {
        m_Lrud = 0;
        DmoveRect(m_WindowRect, delta, rc);
        SetWindowPos(0, &rc, SWP_NOZORDER | SWP_NOSIZE);
      }
    }
  } break;
  }
  return 0;
}

template <class T> void CTransWindowEx<T>::SetWndStyle() {}
