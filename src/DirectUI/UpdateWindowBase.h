#pragma once

#include <atlapp.h>
#include <atlwinx.h>
#include "DUIDef.h"

class __declspec(novtable) CUpdateWindowBase
    : public CWindowImpl<CUpdateWindowBase> {
public:
  CUpdateWindowBase();
  ~CUpdateWindowBase();

public:
  BEGIN_MSG_MAP_EX(CUpdateWindowBase)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(MSG_UPDATE_WINDOW_INVALIDATE, UpdateTranslate)
  MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()

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

protected:
  BOOL UpdateTransDisplay(int Transparent);

  virtual void DrawBg(HDC hDC) = 0;

  virtual void OnDraw(HDC hDC) = 0;

  bool IsBits32();
};
