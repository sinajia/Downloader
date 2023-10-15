#include "StdAfx.h"
#include "util/util_tools.h"
#include "../base/Thread.h"
#include "../base/common_threads.h"
#include "DUITransWindow.h"

using namespace std;

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 600;

CTransWindow::CTransWindow() : _bRoundCorner(0) {
  _bIsWin7 = FALSE;
  _bActived = FALSE;
  _nTimerID = 0;
  _transparent = TRUE;
  _nMinWidth = 307;
  _nMinHeight = 451;
  _nTitleHeight = 20;
  _bmpFrame = NULL;
  _bmpFrameBack = NULL;
  _bmpResize = NULL;
  _bmpTitle = NULL;
  _bmpLogo = NULL;
  _bIsCenter = FALSE;
  m_ptips = NULL;
  _bSupportChangeSize = FALSE;
  _bSupportDblTitle = FALSE;
  _bOK = FALSE;
}

CTransWindow::~CTransWindow() { Detach(); }

BOOL CTransWindow::DUICreateW(HWND hParentWnd, WCHAR *lpszXml) {
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

  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

  HWND hWnd = Create(hParentWnd, rcWnd, _title.c_str(), _style, _ex_style);

  if (hWnd == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  ::SetForegroundWindow(m_hWnd);

  OnLoaded();
  return TRUE;
}

BOOL CTransWindow::DUICreate(HWND hParentWnd, LPCTSTR lpszXmlWidow) {
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

  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

  HWND hWnd = Create(hParentWnd, rcWnd, _title.c_str(), _style, _ex_style);

  if (hWnd == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  ::SetForegroundWindow(m_hWnd);

  OnLoaded();

  return TRUE;
}

BOOL CTransWindow::DUICreate(HWND hParentWnd, DWORD dwExStyle) {
  SetWndPos(_location, _size);

  DUI_Rect rcWnd(_location, _size);

  DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

  if (Create(hParentWnd, rcWnd, _T(""), dwStyle, dwExStyle) == NULL) {
    ATLTRACE(_T("Main window creation failed!\n"));
    return FALSE;
  }

  ::SetForegroundWindow(m_hWnd);

  return TRUE;
}

void CTransWindow::SetWndPos(DUI_Point &pt, DUI_Size &sz) {}

BOOL CTransWindow::DoModal() {
  HWND hWndParent = ::GetWindow(m_hWnd, GW_OWNER);
  ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
  ::EnableWindow(hWndParent, FALSE);

  std::auto_ptr<base::MessageLoop::LayerLoop> message_loop;
  message_loop.reset(
      new base::MessageLoop::LayerLoop(base::MessageLoop::TYPE_UI));
  CHECK(base::Threads::Get(Threads::UI) == base::MessageLoop::current());
  (static_cast<base::MessagePumpForUI *>(message_loop->pump_win()))
      ->upPumpLayer(static_cast<base::MessagePumpForUI *>(
          base::Threads::Get(Threads::UI)->pump_win()));

  base::MessageLoop::LayerLoop::PushBackLoop(message_loop.get());

  message_loop->Run();

  base::MessageLoop::LayerLoop::PopBackLoop();

  ::EnableWindow(hWndParent, TRUE);
  ::SetFocus(hWndParent);

  return _bOK;
}

LRESULT CTransWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled) {
  _host = m_hWnd;

  if (_bIsCenter)
    CenterWindow();

  _elementManager.InitOwner(m_hWnd);

  OnDynamicCreate();

  if (!m_ptips)
    m_ptips = new CToolTipCtrl;

  m_ptips->Create(m_hWnd);
  m_ptips->Activate(FALSE);
  m_ptips->AddTool(m_hWnd);
  m_ptips->SetMaxTipWidth(260);

  OnLoad();

  SetWindowText(_title.c_str());

  OnLoadIcon();

  return 0;
}

LRESULT CTransWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  if (m_ptips) {
    if (::IsWindow((m_ptips->m_hWnd))) {
      m_ptips->DestroyWindow();
    }
    delete m_ptips;
    m_ptips = NULL;
  }
  return 0;
}

void CTransWindow::OnLoadIcon() {
  static HICON hIcon = (HICON)::LoadImage(
      _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON,
      ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON),
      LR_DEFAULTCOLOR);
  if (hIcon) {
    SetIcon(hIcon, TRUE);
    SetIcon(hIcon, FALSE);
  }
}

void CTransWindow::OnDynamicCreate() {
  CXmlHelper helper;
  vector<XmlElementPtr> pAllElement = helper.GetChild(m_pXmlWindow);
  vector<XmlElementPtr>::iterator ite = pAllElement.begin();
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

    if (!strEleName.compare(_T("static"))) {
      pElement = new CStaticCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("tablist"))) {
      pElement = new CTabListCtl(_host, _transparent);
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
      continue;
    }

    if (!strEleName.compare(_T("checkbox"))) {
      pElement = new CCheckBoxCtl(m_hWnd, TRUE);
      pElement->CreateElement(*ite);
      _elementManager.AddElement(pElement);
      continue;
    }
  }
}

void CTransWindow::SetWndStyle() {}

void CTransWindow::CreateElement(XmlElementPtr element) {
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

  _bmpResize = GetBitmapAttribute(element, "resize");
  _bmpTitle = GetBitmapAttribute(element, "title");
  _bmpLogo = GetBitmapAttribute(element, "logo");
  _bRoundCorner = GetIntegerAttribute(element, "roundcorner");
}

HRESULT CTransWindow::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  bHandled = TRUE;
  return 0;
}

void CTransWindow::DrawBg(HDC hDC) {
  DrawBorder(hDC);
  DrawTitle(hDC);
}

void CTransWindow::OnDraw(HDC hDC) { _elementManager.OnPaint(hDC); }

void CTransWindow::DrawFrame(CDCHandle dc) {
  RECT rcClient;
  GetClientRect(&rcClient);

  HDC hDC = dc.m_hDC;

  CGDIResource::GetInstance().GDI_DrawRectangle(
      hDC, &rcClient, RGB(110, 150, 177), RGB(182, 234, 253));
}

void CTransWindow::DrawBorder(CDCHandle dc) {
  DUI_Rect wndRect;
  GetClientRect(&wndRect);

  HDC hDC = dc.m_hDC;

  BITMAP bmpFrameData;
  GetObject(_bmpFrame, sizeof(bmpFrameData), &bmpFrameData);

  if (_bmpFrame) {
    CGDIResource::GetInstance().GDI_DrawImageStretch(
        hDC, _bmpFrame, wndRect,
        DUI_Rect(0, 0, bmpFrameData.bmWidth, bmpFrameData.bmHeight), 255,
        _rcFrameCorner);
  }

  if (_bmpFrameBack) {
    BITMAP bmpBackData;
    GetObject(_bmpFrameBack, sizeof(bmpBackData), &bmpBackData);
    CGDIResource::GetInstance().GDI_DrawImageStretch(
        hDC, _bmpFrameBack, _rcFrameBackPos,
        DUI_Rect(0, 0, bmpBackData.bmWidth, bmpBackData.bmHeight), 255,
        _rcFrameBackCorner);
  }
}

void CTransWindow::DrawTitle(CDCHandle dc) {
  BITMAP bmpLogoInfor;
  if (_bmpLogo) {
    GetObject(_bmpLogo, sizeof(bmpLogoInfor), &bmpLogoInfor);

    int nLeft = 10;
    int nTop = 10;

    CGDIResource::GetInstance().GDI_DrawImage(
        dc.m_hDC, _bmpLogo, nLeft, nTop, bmpLogoInfor.bmWidth,
        bmpLogoInfor.bmHeight, DUI_BT_ALPHA);
  }

  if (_title.size() > 0) {
    RECT rcClient;
    GetClientRect(&rcClient);

    RECT rcText;
    rcText.left = _bmpLogo != NULL ? 10 + bmpLogoInfor.bmWidth + 7 : 18;
    rcText.right = rcClient.right - 1;
    rcText.top = 9;
    rcText.bottom = _nTitleHeight > 0 ? _nTitleHeight : rcText.top + 20;

    CGDIResource::GetInstance().GDI_DrawText_Trans(
        dc.m_hDC, _title.c_str(), &rcText,
        CGDIResource::GetInstance().GDI_GetNormalFont(), RGB(255, 255, 255));
  }
}

LRESULT CTransWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
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

LRESULT CTransWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled) {
  int cx = GET_X_LPARAM(lParam);
  int cy = GET_Y_LPARAM(lParam);

  _elementManager.OnSizeChanged(_size, cx, cy);
  _size.cx = cx;
  _size.cy = cy;

  SetSkinedWindowRgn(_bRoundCorner);

  return 0;
}

LRESULT CTransWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  _elementManager.OnKeyDown((DWORD)wParam);

  return 0;
}

LRESULT CTransWindow::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) {
  _elementManager.OnKeyUp((DWORD)wParam);

  return 0;
}

LRESULT CTransWindow::OnSysKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {

  _elementManager.OnKeyDown((DWORD)wParam);
  return 0;
}

LRESULT CTransWindow::OnSysKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL &bHandled) {
  _elementManager.OnKeyUp((DWORD)wParam);
  return 0;
}

LRESULT CTransWindow::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled) {
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

LRESULT CTransWindow::OnNcLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL &bHandled) {
  if (_bSupportDblTitle) {
    if (IsZoomed())
      ::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    else
      ::PostMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  }

  return 0;
}

LRESULT CTransWindow::OnNcMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  if (IsZoomed() && !_bIsWin7) {
    ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);
  } else {
    ModifyStyle(WS_SYSMENU, 0, 0);
  }

  return 0;
}

LRESULT CTransWindow::OnGetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  ModifyStyle(0, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);
  bHandled = FALSE;
  return 0;
}

LRESULT CTransWindow::OnSetIcon(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  return 0;
}

LRESULT CTransWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseDown(uMsg, pt);
  return 0;
}
LRESULT CTransWindow::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseUp(uMsg, pt);
  return 0;
}

LRESULT CTransWindow::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  return 0;
}

LRESULT CTransWindow::OnRButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseRUp(uMsg, pt);
  return 0;
}

LRESULT CTransWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  MSG msg;
  msg.hwnd = m_hWnd;
  msg.message = WM_MOUSEMOVE;
  msg.wParam = 0;
  msg.lParam = GetMessagePos();
  if (m_ptips)
    m_ptips->RelayEvent(&msg);

  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseMove(uMsg, pt);

  TrackMouseLeave(m_hWnd);

  return 0;
}

LRESULT CTransWindow::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL &bHandled) {
  DUI_Point pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  _elementManager.OnMouseMove(uMsg, pt);
  return 0;
}

LRESULT CTransWindow::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  return 0;
}

LRESULT CTransWindow::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam,
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

LRESULT CTransWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) {
  ShowWindow(SW_HIDE);

  OnExit();

  return 0;
}

LRESULT CTransWindow::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled) {
  _elementManager.OnMove();
  return 0;
}

LRESULT CTransWindow::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) {
  if (wParam == ACTIVE_TIMER) {
    _elementManager.DrawCursor();
  }

  return 0;
}

void CTransWindow::SetSkinedWindowRgn(BOOL bRound) {
  if (!bRound) {
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

BOOL CTransWindow::PreTranslateMessage(MSG *pMsg) {
  if (m_ptips && ::IsWindow(m_ptips->m_hWnd)) {
    m_ptips->RelayEvent(pMsg);
  }
  return FALSE;
}

LRESULT CTransWindow::OnTips(UINT uMsg, WPARAM wParam, LPARAM lParam,
                             BOOL &bHandled) {
  if (!m_ptips)
    return 0;
  BOOL bTips = (BOOL)wParam;
  if (bTips) {
    LPCTSTR lpszText = (LPCTSTR)lParam;
    m_ptips->UpdateTipText(lpszText, m_hWnd);
    m_ptips->Activate(TRUE);
  } else {
    m_ptips->Activate(FALSE);
  }

  return 0;
}

LRESULT CTransWindow::OnElementActive(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  _elementManager.SetActive(pOwner);

  return 0;
}

LRESULT CTransWindow::OnChainMsg(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL &bHandled) {
  _elementManager.OnMsg(nMsg, wParam, lParam, bHandled);
  return 0;
}

LRESULT CTransWindow::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled) {
  HCURSOR hCur = (HCURSOR)wParam;
  if (hCur == LoadCursor(NULL, IDC_HAND)) {
    ::SetClassLong(m_hWnd, GCL_HCURSOR,
                   (long)LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
  } else if (hCur == LoadCursor(NULL, IDC_IBEAM)) {
    ::SetClassLong(m_hWnd, GCL_HCURSOR,
                   (long)LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
  } else {
    ::SetClassLong(m_hWnd, GCL_HCURSOR,
                   (long)LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
  }

  return 0;
}

LRESULT CTransWindow::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam,
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

void CTransWindow::OnLoad() {}

void CTransWindow::OnLoaded() {}

void CTransWindow::OnExit() { _elementManager.Dispose(); }

DWORD CTransWindow::OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_NOTIFYITEMDRAW;
}

DWORD CTransWindow::OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_DODEFAULT;
}

DWORD CTransWindow::OnSubItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD) {
  return CDRF_DODEFAULT;
}
