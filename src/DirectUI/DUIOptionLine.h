#pragma once

#include "DUIElement.h"

class COptionLineCtl : public CBaseElementCtl {
public:
  COptionLineCtl(HWND father, BOOL trans = FALSE);
  COptionLineCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                 LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans = FALSE);
  ~COptionLineCtl(void);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);

  void SetTextFont(HFONT font);

protected:
  HFONT _textFont;
  COLORREF _textCol;
  COLORREF _lineCol;
};
