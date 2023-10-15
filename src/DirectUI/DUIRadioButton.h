#pragma once

#include "DUIButton.h"

class CRadioButtonCtl : public CButtonCtl {
public:
  CRadioButtonCtl(HWND father, BOOL trans = FALSE);
  CRadioButtonCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                  LPCTSTR lpszTooptip, DUI_Rect rcElement);
  ~CRadioButtonCtl(void);

  void SetCheck(BOOL bCheck);
  BOOL GetCheck();
  void Disable();
  void Enable(BOOL enable);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void OnMouseDown(UINT nFlags, CPoint point);
  void OnMouseUp(UINT nFlags, CPoint point);
  void OnMouseHover(UINT nFlags, CPoint point);
  void OnMouseLeave();

protected:
  COLORREF _txtColor;
  BOOL _bChecked;
};
