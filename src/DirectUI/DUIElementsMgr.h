#pragma once

#include "DUIElement.h"

class CAnyElementsMgr {
public:
  CAnyElementsMgr(void);
  CAnyElementsMgr(HWND owner) { _owner = owner; }
  ~CAnyElementsMgr(void);

  void InitOwner(HWND hWnd) { _owner = hWnd; }

  void OnMove();
  BOOL MouseOnElement(CPoint pt);
  void OnKeyDown(DWORD dwFlag);
  void OnKeyUp(DWORD dwFlag);
  void OnMouseClick(CPoint pt);
  void OnMouseDown(UINT nFlags, CPoint pt);
  void OnMouseDblDown(UINT nFlags, CPoint pt);
  void OnMouseUp(UINT nFlags, CPoint pt);
  void OnMouseRUp(UINT nFlags, CPoint pt);
  void OnMouseLDbClick(UINT nFlags, CPoint pt);
  void OnMouseLeave();
  void OnMouseMove(UINT nFlags, CPoint pt);
  void OnPaint(HDC hDC);
  void OnSizeChanged(DUI_Size wndChangingSize, int w, int h);
  void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  void RearrangeElement(CBaseElementCtl *e);
  void AddElement(CBaseElementCtl *element);
  void AddElement2(CBaseElementCtl *element);
  void DrawCursor();
  void SetActive(CBaseElementCtl *element);
  void OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  CBaseElementCtl *Search(const tstring id);
  CBaseElementCtl *GetElement();
  void Reset();
  void Dispose();
  void Dispose(const tstring id);
  std::list<CBaseElementCtl *> *GetAllElement();
  DWORD EleNum() { return _controls.size(); }

private:
  std::list<CBaseElementCtl *> _controls;
  CBaseElementCtl *_last_mouseon_element;
  HWND _owner;
  CBaseElementCtl *_rearrange_element;
  BOOL _visible;
  std::list<CBaseElementCtl *>::iterator _cur_pos;

protected:
  BOOL PointInRect(CPoint pt, CRect rect);
};

inline BOOL CAnyElementsMgr::PointInRect(CPoint pt, CRect rect) {
  return (((pt.x >= rect.left) && (pt.x <= rect.right)) &&
          ((pt.y >= rect.top) && (pt.y <= rect.bottom)));
}

