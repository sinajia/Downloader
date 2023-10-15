#pragma once

#include "DUIElement.h"

class CStaticCtl : public CBaseElementCtl {
public:
  CStaticCtl(HWND father, BOOL trans = FALSE);
  CStaticCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
             LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans = FALSE);
  ~CStaticCtl(void);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void OnMouseDown(UINT nFlags, CPoint point);
  void OnMouseUp(UINT nFlags, CPoint point);
  void OnCall_LeftUp();

  void SetTextFont(HFONT font);
  void SetTextColor(COLORREF colText);
  void SetImage(LPCTSTR lpszImageRes);
  void SetIcon(LPCTSTR lpszIconRes);
  void SetTextFormat(UINT nTextFormat);

protected:
  HBITMAP _bmpBackground;
  HBITMAP _bmpIcon;
  HFONT _textFont;
  COLORREF _textCol;
  BOOL _down;
  UINT _textFormat;
  BOOL _viewport;

public:
  int _scrollDelta;
};
