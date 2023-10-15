#pragma once

#include "DUIButton.h"

class CTabListCtl;
class CTabCtl : public CButtonCtl {
public:
  CTabCtl(HWND father, CTabListCtl *pList = NULL, BOOL trans = FALSE);
  CTabCtl(HWND hWnd, LPCTSTR lpszID, LPCTSTR lpszTitle, LPCTSTR lpszTooptip,
          DUI_Rect rcElement);
  ~CTabCtl(void);

public:
  void SetUpdate(int nCount);
  void SetSel(BOOL bSel);
  BOOL GetSel();
  void OnPaint(HDC hDC);
  virtual void OnMouseHover(UINT nFlags, CPoint point);
  virtual void OnMouseLeave();
  virtual void OnMouseUp(UINT nFlags, CPoint point);

  void RaiseInvalidate(BOOL forceRedraw);

protected:
  HBITMAP _bmpUpdate;

  BOOL _bSel;

  int _nCount;

  CTabListCtl *_tab_list;
};
