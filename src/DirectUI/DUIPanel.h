#pragma once

#include "DUIElement.h"
#include "DUIElementsMgr.h"

class CPanelCtl : public CBaseElementCtl {
public:
  CPanelCtl(HWND father, BOOL trans = FALSE);
  CPanelCtl(HWND hWnd, LPCTSTR lpszID, LPCTSTR lpszTitle, LPCTSTR lpszTooptip,
            DUI_Rect rcElement);
  ~CPanelCtl(void);

  void SetPos(CPoint pt, CSize sz);
  void SetPos(WTL::CRect rc);

  void CreateElement(XmlElementPtr element);
  void SetBackImage(LPCTSTR lpszImageRes);
  void AddElement(CBaseElementCtl *element);
  void AddElement2(CBaseElementCtl *element);
  void OnMove();
  void OnKeyDown(DWORD dwFlag);
  void OnKeyUp(DWORD dwFlag);
  void OnMouseDown(UINT nFlags, CPoint pt);
  void OnMouseDblDown(UINT nFlags, CPoint pt);
  void OnMouseUp(UINT nFlags, CPoint pt);
  void OnMouseRUp(UINT nFlags, CPoint pt);
  void OnMouseLeave();
  void OnMouseHover(UINT nFlags, CPoint pt);
  void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void OnPaint(HDC hDC);
  void OnSizeChanged(DUI_Size wndChangingSize, int w, int h);
  void SetVisible(BOOL visible);
  void Invlidate();
  void Dispose();
  void Dispose(LPCTSTR lpszID);
  void ResetElement();
  CBaseElementCtl *GetElement();
  void DrawCursor();
  void SetActive(CBaseElementCtl *element);
  void OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  DWORD EleNum() { return _elementmgr.EleNum(); }

public:
  CAnyElementsMgr _elementmgr;

protected:
  HBITMAP _bmpBackground;
  COLORREF _colBack;
  COLORREF _colBorder;
  BOOL _draw_border;
};

inline void CPanelCtl::SetPos(CPoint pt, CSize sz) {
  _elementmgr.Reset();
  CBaseElementCtl *element = NULL;
  while ((element = _elementmgr.GetElement()) != NULL) {
    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(rcElement.left - _location.x, rcElement.top - _location.y),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }

  _location.x = pt.x;
  _location.y = pt.y;
  _size.cx = sz.cx;
  _size.cy = sz.cy;

  _elementmgr.Reset();
  while ((element = _elementmgr.GetElement()) != NULL) {
    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(_location.x + rcElement.left, _location.y + rcElement.top),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }
}

inline void CPanelCtl::SetPos(WTL::CRect rc) {
  _elementmgr.Reset();
  CBaseElementCtl *element = NULL;
  while ((element = _elementmgr.GetElement()) != NULL) {
    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(rcElement.left - _location.x, rcElement.top - _location.y),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }

  _location.x = rc.left;
  _location.y = rc.top;
  _size.cx = rc.Width();
  _size.cy = rc.Height();

  _elementmgr.Reset();
  while ((element = _elementmgr.GetElement()) != NULL) {
    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(_location.x + rcElement.left, _location.y + rcElement.top),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }
}
