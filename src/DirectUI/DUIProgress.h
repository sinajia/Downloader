#pragma once

#include "DUIElement.h"

class CProgressCtl : public CBaseElementCtl {
public:
  CProgressCtl(HWND father, BOOL trans = FALSE);
  CProgressCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
               LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans = FALSE);
  ~CProgressCtl(void);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void SetPos(double nPos);

protected:
  HBITMAP _bmpBackground;
  HBITMAP _bmpProgress;
  HBITMAP _bmpLightPoint;
  COLORREF _backcolor;
  COLORREF _progresscolor;
  int _lightpointdela;
  int _nActualWidth;
  double _nCurPos;
};
