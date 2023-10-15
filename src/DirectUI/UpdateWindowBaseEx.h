#pragma once

#include <atlapp.h>
#include <atlwinx.h>
#include "DUIDef.h"

constexpr int TIMER_ID1 = 0x1110;
constexpr int TIMER_ID2 = 0x1111;
constexpr int COUNT_END_CONST = -3;

template <typename T>
class __declspec(novtable) CUpdateWindowBaseEx : public CWindowImpl<T> {
public:
  CUpdateWindowBaseEx();
  virtual ~CUpdateWindowBaseEx();

public:
  BEGIN_MSG_MAP_EX(CUpdateWindowBaseEx<T>)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_TIMER, CUpdateWindowBaseEx_OnTimer)
  MESSAGE_HANDLER(MSG_UPDATE_WINDOW_INVALIDATE, UpdateTranslate)
  MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()

  LRESULT CUpdateWindowBaseEx_OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL &bHandled);
  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT UpdateTranslate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                          BOOL &bHandled);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

protected:
  typedef BOOL(WINAPI *UPDATELAYERWINDOWPTR)(HWND, HDC, POINT *, SIZE *, HDC,
                                             POINT *, COLORREF, BLENDFUNCTION *,
                                             DWORD);
  UPDATELAYERWINDOWPTR UpdateLayeredWindow;

  BLENDFUNCTION m_Blend;
  HINSTANCE m_hFuncInst;

public:
  BOOL UpdateTransDisplay(int Transparent);

protected:
  BOOL UpdateTransDisplay2(int Transparent);
  BOOL UpdateTransDisplay3(int Transparent);

  virtual void DrawBg(HDC hDC) = 0;

  virtual void OnDraw(HDC hDC) = 0;

  virtual void IsWindowShowed() {}

  bool IsBits32();

public:
  void ClearOBJ();
  CRect _CUpdateWindowBaseEx_WndRect_con;
  CRect _CUpdateWindowBaseEx_WndRect_var;
  BOOL _bCUpdateWindowBaseEx_TransShow;
  void TransShowWindow(int SW = SW_SHOWNORMAL);
  int _CUpdateWindowBaseEx_Count;
  void TransBeginShow();
  void StretchRect(const RECT &i_rc, float percent, RECT &o_rc);
  void AnimateRect(const RECT &i_rc, int x, int y, RECT &o_rc);
  float _CUpdateWindowBaseEx_percent;

  void StretchShow(int, int);

  void StretchShowSim(int, int);
  HDC _hdcTemp;
  HDC _hdcMemory;
  HBITMAP _hBitMap;
  HGDIOBJ _oldOBJ;
};

template <class T>
CUpdateWindowBaseEx<T>::CUpdateWindowBaseEx()
    : _CUpdateWindowBaseEx_WndRect_con(0, 0, 0, 0) {
  m_hFuncInst = NULL;
  _bCUpdateWindowBaseEx_TransShow = FALSE;
  _CUpdateWindowBaseEx_Count = 10;
  _CUpdateWindowBaseEx_percent = 0.0;
  _hdcTemp = NULL;
  _hdcMemory = NULL;
  _hBitMap = NULL;
  _oldOBJ = NULL;
}
template <class T> CUpdateWindowBaseEx<T>::~CUpdateWindowBaseEx() {
  if (m_hFuncInst)
    FreeLibrary(m_hFuncInst);
}

template <class T>
LRESULT CUpdateWindowBaseEx<T>::OnCreate(UINT uMsg, WPARAM wParam,
                                         LPARAM lParam, BOOL &bHandled) {
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

template <class T>
LRESULT CUpdateWindowBaseEx<T>::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL &bHandled) {
  CPaintDC dc(m_hWnd);
  DrawBg(dc);
  OnDraw(dc);
  return 0;
}

template <class T>
LRESULT CUpdateWindowBaseEx<T>::UpdateTranslate(UINT uMsg, WPARAM wParam,
                                                LPARAM lParam, BOOL &bHandled) {
  UpdateTransDisplay(lParam);
  return 0;
}

template <class T>
BOOL CUpdateWindowBaseEx<T>::UpdateTransDisplay(int Transparent) {

  if (!IsBits32()) {
    Invalidate(TRUE);
    return FALSE;
  }
  if (_bCUpdateWindowBaseEx_TransShow)
    return FALSE;

  CRect rcWindow;
  GetWindowRect(&rcWindow);
  POINT ptWinPos = {rcWindow.left, rcWindow.top};

  HDC hdcTemp = GetDC();
  HDC m_hdcMemory = CreateCompatibleDC(hdcTemp);
  HBITMAP hBitMap =
      CreateCompatibleBitmap(hdcTemp, rcWindow.Width(), rcWindow.Height());
  HGDIOBJ m_oldbj = SelectObject(m_hdcMemory, hBitMap);
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
  SelectObject(m_hdcMemory, m_oldbj);
  DeleteObject(hBitMap);
  DeleteDC(m_hdcMemory);

  return bRet;
}

template <class T> bool CUpdateWindowBaseEx<T>::IsBits32() {
  DEVMODE lpDevMode;
  BOOL Result = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode);
  if (Result) {
    if (lpDevMode.dmBitsPerPel == 32)
      return true;
  }
  return false;
}

template <class T>
void CUpdateWindowBaseEx<T>::StretchShow(int a, int b) {
  AnimateRect(_CUpdateWindowBaseEx_WndRect_con, -100 + a, -100 + a,
              _CUpdateWindowBaseEx_WndRect_var);
  SetWindowPos(0, &_CUpdateWindowBaseEx_WndRect_var, SWP_NOZORDER);
  UpdateTransDisplay2(b);
}

template <class T>
void CUpdateWindowBaseEx<T>::StretchShowSim(int a, int b) {
  AnimateRect(_CUpdateWindowBaseEx_WndRect_con, -100 + a, -100 + a,
              _CUpdateWindowBaseEx_WndRect_var);

  UpdateTransDisplay3(b);
}

template <class T>
LRESULT CUpdateWindowBaseEx<T>::CUpdateWindowBaseEx_OnTimer(UINT uMsg,
                                                            WPARAM wParam,
                                                            LPARAM lParam,
                                                            BOOL &bHandled) {
  if (TIMER_ID1 == wParam) {
    --_CUpdateWindowBaseEx_Count;
    if (_CUpdateWindowBaseEx_Count < COUNT_END_CONST) {
      KillTimer(TIMER_ID1);
      _bCUpdateWindowBaseEx_TransShow = FALSE;
      return 0;
    }
    switch (_CUpdateWindowBaseEx_Count) {
    case 9: {
      StretchShowSim(6, 40);
    } break;
    case 8: {
      StretchShowSim(23, 45);
    } break;
    case 7: {
      StretchShowSim(51, 55);
    } break;
    case 6: {
      StretchShowSim(90, 60);
    } break;
    case 5: {
      KillTimer(TIMER_ID1);
      StretchShowSim(140, 65);
      SetTimer(TIMER_ID2, 30, NULL);
    } break;
    default: {
      KillTimer(TIMER_ID1);
      SetTimer(TIMER_ID2, 30, NULL);
    }
    }
    return 0;
  }
  if (wParam == TIMER_ID2) {
    --_CUpdateWindowBaseEx_Count;
    if (_bCUpdateWindowBaseEx_TransShow == FALSE) {
      KillTimer(TIMER_ID2);
      UpdateTransDisplay(100);
      ClearOBJ();
      IsWindowShowed();
      return 0;
    }
    if (_CUpdateWindowBaseEx_Count < COUNT_END_CONST) {
      _bCUpdateWindowBaseEx_TransShow = FALSE;
      return 0;
    }
    switch (_CUpdateWindowBaseEx_Count) {
    case 4: {
      StretchShow(126, 70);
    } break;
    case 3: {
      StretchShow(114, 75);
    } break;
    case 2: {
      StretchShow(106, 80);
    } break;
    case 1: {
      StretchShow(102, 90);
    } break;
    case 0: {
      SetWindowPos(0, &_CUpdateWindowBaseEx_WndRect_con, SWP_NOZORDER);
      UpdateTransDisplay(100);
      _bCUpdateWindowBaseEx_TransShow = FALSE;
    } break;
    default: {
      SetWindowPos(0, &_CUpdateWindowBaseEx_WndRect_con, SWP_NOZORDER);
      UpdateTransDisplay(100);
      _bCUpdateWindowBaseEx_TransShow = FALSE;
    }
    }
    return 0;
  }
  bHandled = FALSE;
  return 0;
}

template <typename T> void CUpdateWindowBaseEx<T>::TransBeginShow() {
  if (!IsBits32()) {
    _bCUpdateWindowBaseEx_TransShow = FALSE;
    return;
  }
  _bCUpdateWindowBaseEx_TransShow = TRUE;
}

template <class T>
void CUpdateWindowBaseEx<T>::StretchRect(/*_In_*/ const RECT &i_rc,
                                         float percent, /*_Out_*/ RECT &o_rc) {
  int x;
  int y;
  x = (i_rc.right - i_rc.left) * percent;
  y = (i_rc.bottom - i_rc.top) * percent;
  o_rc.left = i_rc.left + ((i_rc.right - i_rc.left) - x) / 2;
  o_rc.right = (o_rc.left + x);
  o_rc.top = i_rc.top + ((i_rc.bottom - i_rc.top) - y) / 2;
  o_rc.bottom = (o_rc.top + y);
}

template <class T>
BOOL CUpdateWindowBaseEx<T>::UpdateTransDisplay2(int Transparent) {
  CRect rcWindow;
  rcWindow = _CUpdateWindowBaseEx_WndRect_var;
  POINT ptWinPos = {rcWindow.left, rcWindow.top};
  if (Transparent < 0 || Transparent > 100)
    Transparent = 100;

  m_Blend.SourceConstantAlpha = int(Transparent * 2.55);
  HDC hdcScreen = ::GetDC(m_hWnd);
  SIZE sizeWindow = {rcWindow.Width(), rcWindow.Height()};
  HDC yyhdcTemp = GetDC();
  HDC m_yyhdcMemory = CreateCompatibleDC(yyhdcTemp);
  HBITMAP yyhBitMap = CreateCompatibleBitmap(
      yyhdcTemp, _CUpdateWindowBaseEx_WndRect_var.Width(),
      _CUpdateWindowBaseEx_WndRect_var.Height());
  HGDIOBJ m_oldbj2 = SelectObject(m_yyhdcMemory, yyhBitMap);
  ::StretchBlt(m_yyhdcMemory, 0, 0, sizeWindow.cx, sizeWindow.cy, _hdcMemory, 0,
               0, _CUpdateWindowBaseEx_WndRect_con.Width(),
               _CUpdateWindowBaseEx_WndRect_con.Height(), SRCCOPY);
  POINT ptSrc = {0, 0};
  DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
  if ((dwExStyle & 0x80000) != 0x80000)
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
  BOOL bRet = FALSE;

  bRet = UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow,
                             m_yyhdcMemory, &ptSrc, 0, &m_Blend, 2);
  ::ReleaseDC(m_hWnd, hdcScreen);
  ::ReleaseDC(m_hWnd, yyhdcTemp);
  SelectObject(m_yyhdcMemory, m_oldbj2);
  DeleteDC(m_yyhdcMemory);
  DeleteObject(yyhBitMap);
  return bRet;
}

template <class T>
BOOL CUpdateWindowBaseEx<T>::UpdateTransDisplay3(int Transparent) {
  CRect rcWindow;
  rcWindow = _CUpdateWindowBaseEx_WndRect_con;
  POINT ptWinPos = {rcWindow.left - 20, rcWindow.top - 20};
  if (Transparent < 0 || Transparent > 100)
    Transparent = 100;

  m_Blend.SourceConstantAlpha = int(Transparent * 2.55);
  HDC hdcScreen = ::GetDC(m_hWnd);
  SIZE sizeWindow = {rcWindow.Width() + 40, rcWindow.Height() + 40};
  HDC yyhdcTemp = GetDC();
  HDC m_yyhdcMemory = CreateCompatibleDC(yyhdcTemp);
  HBITMAP yyhBitMap = CreateCompatibleBitmap(
      yyhdcTemp, _CUpdateWindowBaseEx_WndRect_con.Width() + 40,
      _CUpdateWindowBaseEx_WndRect_con.Height() + 40);
  HGDIOBJ m_oldbj2 = SelectObject(m_yyhdcMemory, yyhBitMap);
  int delta_x = _CUpdateWindowBaseEx_WndRect_con.Width() + 40 -
                _CUpdateWindowBaseEx_WndRect_var.Width();
  delta_x = delta_x / 2;
  int delta_y = _CUpdateWindowBaseEx_WndRect_con.Height() + 40 -
                _CUpdateWindowBaseEx_WndRect_var.Height();
  delta_y = delta_y / 2;
  ::StretchBlt(m_yyhdcMemory, delta_x, delta_y,
               _CUpdateWindowBaseEx_WndRect_var.Width(),
               _CUpdateWindowBaseEx_WndRect_var.Height(), _hdcMemory, 0, 0,
               _CUpdateWindowBaseEx_WndRect_con.Width(),
               _CUpdateWindowBaseEx_WndRect_con.Height(), SRCCOPY);
  POINT ptSrc = {0, 0};
  DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
  if ((dwExStyle & 0x80000) != 0x80000)
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);
  BOOL bRet = FALSE;

  bRet = UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow,
                             m_yyhdcMemory, &ptSrc, 0, &m_Blend, 2);

  ::ReleaseDC(m_hWnd, hdcScreen);
  ::ReleaseDC(m_hWnd, yyhdcTemp);
  SelectObject(m_yyhdcMemory, m_oldbj2);
  DeleteDC(m_yyhdcMemory);
  DeleteObject(yyhBitMap);
  return bRet;
}

template <class T> void CUpdateWindowBaseEx<T>::ClearOBJ() {
  if (_hdcTemp && _hdcMemory) {
    ::ReleaseDC(m_hWnd, _hdcTemp);
    SelectObject(_hdcMemory, _oldOBJ);
    DeleteDC(_hdcMemory);
    DeleteObject(_hBitMap);
    _hdcTemp = NULL;
    _hdcMemory = NULL;
    _hBitMap = NULL;
    _oldOBJ = NULL;
  }
}

template <class T> void CUpdateWindowBaseEx<T>::TransShowWindow(int SW) {
  if (_bCUpdateWindowBaseEx_TransShow) {
    ATLASSERT(_CUpdateWindowBaseEx_Count == 10);
    int cx, cy;
    cx = _CUpdateWindowBaseEx_WndRect_con.Width();
    cy = _CUpdateWindowBaseEx_WndRect_con.Height();
    if (cx >= 150 && cy >= 110) {

      MSG msg;
      while (::PeekMessage(&msg, m_hWnd, MSG_UPDATE_WINDOW_INVALIDATE,
                           MSG_UPDATE_WINDOW_INVALIDATE, PM_REMOVE))
        ;
      ((T *)this)->ChangeSize(cx, cy);
      if (!_hdcTemp) {
        _hdcTemp = GetDC();
      }
      if (!_hdcMemory) {
        _hdcMemory = CreateCompatibleDC(_hdcTemp);
      }
      if (!_hBitMap) {
        _hBitMap = CreateCompatibleBitmap(
            _hdcTemp, _CUpdateWindowBaseEx_WndRect_con.Width(),
            _CUpdateWindowBaseEx_WndRect_con.Height());
      }
      _oldOBJ = SelectObject(_hdcMemory, _hBitMap);
      DrawBg(_hdcMemory);
      OnDraw(_hdcMemory);

      AnimateRect(_CUpdateWindowBaseEx_WndRect_con, 40, 40,
                  _CUpdateWindowBaseEx_WndRect_var);
      SetWindowPos(0, &_CUpdateWindowBaseEx_WndRect_var, SWP_NOZORDER);

      AnimateRect(_CUpdateWindowBaseEx_WndRect_con, -100, -100,
                  _CUpdateWindowBaseEx_WndRect_var);
      UpdateTransDisplay3(35);
      ShowWindow(SW_SHOWNORMAL);
      _CUpdateWindowBaseEx_Count = 10;
      SetTimer(TIMER_ID1, 30, NULL);
    } else {
      _bCUpdateWindowBaseEx_TransShow = FALSE;
      ShowWindow(SW);
    }
  } else {
    ShowWindow(SW);
  }
}

template <class T>
void CUpdateWindowBaseEx<T>::AnimateRect(/*_In_*/ const RECT &i_rc, int x,
                                         int y, /*_Out_*/ RECT &o_rc) {
  y = x * (i_rc.bottom - i_rc.top) / (i_rc.bottom - i_rc.top);
  x = -x;
  o_rc.left = i_rc.left + x / 2;
  o_rc.right = (i_rc.right - x / 2);
  y = -y;
  o_rc.top = i_rc.top + y / 2;
  o_rc.bottom = (i_rc.bottom - y / 2);
}
