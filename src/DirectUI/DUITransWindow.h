#pragma once

#include "DUIButton.h"
#include "DUICheckBox.h"
#include "DUIElement.h"
#include "DUIElementsMgr.h"
#include "DUIHyperLink.h"
#include "DUILayeredEdit.h"
#include "DUIPanel.h"
#include "DUIProgress.h"
#include "DUIStatic.h"
#include "DUITab.h"
#include "DUITabList.h"
#include "UpdateWindowBase.h"

#define CHAIN_ALL_MSG(func)                                                    \
  {                                                                            \
    if (func(uMsg, wParam, lParam, bHandled))                                  \
      return TRUE;                                                             \
  }

class __declspec(novtable) CTransWindow : public CMessageFilter,
                                          public CBaseElementCtl,
                                          public CUpdateWindowBase {
public:
  CTransWindow();
  virtual ~CTransWindow();

  DECLARE_WND_CLASS(NULL)

  BEGIN_MSG_MAP(CTransWindow)
  CHAIN_MSG_MAP(CUpdateWindowBase)
  CHAIN_ALL_MSG(OnChainMsg)

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

  MESSAGE_HANDLER(WM_MOVE, OnMove)
  MESSAGE_HANDLER(WM_CLOSE, OnClose)
  MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
  MESSAGE_HANDLER(WM_GETICON, OnGetIcon)

  MESSAGE_HANDLER(WM_MSG_TIPS, OnTips)
  MESSAGE_HANDLER(WM_MSG_CURSOR, OnSetCursor)
  MESSAGE_HANDLER(WM_MOUSEHWHEEL, OnMouseWheel)
  MESSAGE_HANDLER(WM_ASYNC_CALL_LUP, OnAsynCall_LUp)
  MESSAGE_HANDLER(WM_ASYNC_CALL_RUP, OnAsynCall_RUp)
  MESSAGE_HANDLER(WM_MSG_ACTIVE, OnElementActive)

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

  virtual LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled);
  virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled);
  virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                           BOOL &bHandled);
  virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
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
  virtual LRESULT OnElementActive(UINT nMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL &bHandled);
  virtual LRESULT OnChainMsg(UINT nMsg, WPARAM wParam, LPARAM lParam,
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

  BOOL _bOK;

protected:
  virtual void OnDynamicCreate();

  virtual void SetSkinedWindowRgn(BOOL bZoomed);
  virtual void CreateElement(XmlElementPtr element);
  virtual void SetWndStyle();

  BOOL _bIsWin7;

  BOOL _bActived;

  int _nTimerID;

  CAnyElementsMgr _elementManager;

  CToolTipCtrl *m_ptips;

  BOOL _bSupportDblTitle;

  BOOL _bSupportChangeSize;

  int _nMinWidth;
  int _nMinHeight;
  int _nTitleHeight;
  BOOL _bIsCenter;
  BOOL _bRoundCorner;

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
};

inline LRESULT CTransWindow::OnAsynCall_LUp(UINT nMsg, WPARAM wParam,
                                            LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_LeftUp();

  return S_OK;
}

inline LRESULT CTransWindow::OnAsynCall_RUp(UINT nMsg, WPARAM wParam,
                                            LPARAM lParam, BOOL &bHandled) {
  CBaseElementCtl *pOwner = (CBaseElementCtl *)wParam;
  pOwner->OnCall_RightUp();

  return S_OK;
}
