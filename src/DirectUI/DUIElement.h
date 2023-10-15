#pragma once

#include "DUIDef.h"
#include "DUIGDIResource.h"
#include "DUIWindowStyle.h"
#include "EventHandler.h"

class __declspec(novtable) CBaseElementCtl : public CWindowCtlStyle {
public:
  CBaseElementCtl();
  CBaseElementCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                  LPCTSTR lpszTooptip, DUI_Rect rcElement);
  virtual ~CBaseElementCtl(void);

  virtual void CreateElement(XmlElementPtr element);

  virtual tstring GetID();
  virtual tstring GetName();
  virtual tstring GetTitle();
  virtual tstring GetTooltip();
  virtual tstring &GetWndClassName();
  virtual void SetTitle(const tstring &csName);
  virtual void SetTooltip(const tstring &strTip);
  virtual void SetID(const tstring &id);
  virtual void SetName(const tstring &id);
  virtual void SetHandleMouse(BOOL mouse);
  virtual BOOL GetVisible();
  virtual BOOL GetHandleMouse();
  virtual void SetVisible(BOOL bVisible);
  virtual void SetHandleKeyboard(BOOL keyboard);
  virtual BOOL GetHandleKeyboard();
  virtual void SetActive(BOOL active);
  virtual void SetActive(CBaseElementCtl *element);
  virtual BOOL GetActive();
  virtual void SetVisibleCur(BOOL bVisible);
  virtual BOOL GetVisibleCur();
  virtual void SetPos(WTL::CPoint pt, WTL::CSize sz);
  virtual void SetOriginPos(CPoint pt, CSize sz);
  virtual void SetPos(WTL::CRect rc);
  virtual void SetOriginPos(WTL::CRect rc);
  virtual CRect Rectangle();
  virtual CRect OriginRectangle();
  virtual HCURSOR GetCursor();
  virtual void SetCursor(HCURSOR cursor);
  virtual void RegisterLButtonDownEvent(EventHandler event);
  virtual void RegisterLButtonUpEvent(EventHandler event);
  virtual void RegisterRButtonUpEvent(EventHandler event);
  virtual void RegisterHoverEvent(EventHandler event);
  virtual void RegisterLeaveEvent(EventHandler event);
  virtual void RegisterEnterKeyEvent(EventHandler event);
  virtual void RegisterLButtonDbCEvent(EventHandler event);
  virtual void RaiseInvalidate(BOOL forceRedraw = TRUE);
  virtual int GetTextWidth(LPCTSTR lpszText, HFONT hFont);
  virtual HWND GetHost();
  virtual void DrawCursor() { ; }
  virtual void VerticalOffset(int i = 0) { _location.y += i; }

  virtual BOOL IsPanel() { return FALSE; }

  virtual void OnMove();
  virtual void OnKeyDown(DWORD dwFlag);
  virtual void OnKeyUp(DWORD dwFlag);
  virtual void OnMouseDClick();
  virtual void OnMouseClick();
  virtual void OnMouseLDbClick(UINT nFlags, CPoint point);
  virtual void OnMouseDown(UINT nFlags, CPoint point);
  virtual void OnMouseDblDown(UINT nFlags, CPoint point);
  virtual void OnMouseUp(UINT nFlags, CPoint point);
  virtual void OnMouseRUp(UINT nFlags, CPoint point);
  virtual void OnMouseHover(UINT nFlags, CPoint point);
  virtual void OnMouseLeave();
  virtual void OnPaint(HDC hDC);
  virtual void OnSizeChanged(DUI_Size wndChangingSize, int w, int h);
  virtual void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual BOOL PointInRect(CPoint pt, CRect rect);
  virtual void OnCall_LeftUp();
  virtual void OnCall_RightUp();
  virtual void OnCall_EnterKey();
  virtual void OnCall_LeftDown();
  virtual void OnEnterKey();
  virtual void OnTabKey() {}
  virtual void OnCall_LDbClick();
  virtual void OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

public:
  virtual void GetElementStyle(XmlElementPtr element);

  HWND _host;
  tstring _id;
  tstring _name;
  tstring _title;
  tstring _class;
  tstring _tip;
  DWORD _style;
  DWORD _ex_style;
  CPoint _location;
  CPoint _ori_location;
  CSize _size;
  CSize _ori_size;
  CPoint _base_pt;
  BOOL _handleMouse;
  BOOL _handleKeyboard;
  BOOL _active;
  BOOL _visible;
  BOOL _viscur;
  HCURSOR _cursor;
  BOOL _transparent;
  BOOL _editCheckPath;

  EventHandler _leftdown_event;
  EventHandler _leftup_event;
  EventHandler _rightup_event;
  EventHandler _hover_event;
  EventHandler _leave_event;
  EventHandler _enterkey_event;
  EventHandler _leftDbclick_event;
};

inline HWND CBaseElementCtl::GetHost() { return _host; }

inline void CBaseElementCtl::SetID(const tstring &id) { _id = id; }
inline tstring CBaseElementCtl::GetID() { return _id; }

inline tstring CBaseElementCtl::GetTitle() { return _title; }

inline void CBaseElementCtl::SetTitle(const tstring &title) { _title = title; }

inline void CBaseElementCtl::SetVisible(BOOL visible) {
  if (_visible != visible) {
    _visible = visible;
    RaiseInvalidate(TRUE);
  }
}

inline BOOL CBaseElementCtl::GetVisible() { return _visible; }

inline tstring CBaseElementCtl::GetTooltip() { return _tip; }

inline void CBaseElementCtl::SetTooltip(const tstring &csTooltip) {
  _tip = csTooltip;
}

inline void CBaseElementCtl::SetHandleMouse(BOOL mouse) {
  _handleMouse = mouse;
}

inline BOOL CBaseElementCtl::GetHandleMouse() { return _handleMouse; }

inline void CBaseElementCtl::SetHandleKeyboard(BOOL keyboard) {
  _handleKeyboard = keyboard;
}

inline BOOL CBaseElementCtl::GetHandleKeyboard() { return _handleKeyboard; }

inline void CBaseElementCtl::SetActive(BOOL active) {
  _active = active;
  if (_active) {
  }
}

inline void CBaseElementCtl::SetActive(CBaseElementCtl *element) {
  if (element == this)
    _active = TRUE;
  else
    _active = FALSE;
}

inline BOOL CBaseElementCtl::GetActive() { return _active; }

inline void CBaseElementCtl::SetVisibleCur(BOOL bVisible) {
  _viscur = bVisible;
}

inline BOOL CBaseElementCtl::GetVisibleCur() { return _viscur; }

inline void CBaseElementCtl::SetName(const tstring &name) { _name = name; }
inline tstring CBaseElementCtl::GetName() { return _name; }

inline CRect CBaseElementCtl::Rectangle() {
  CPoint pt(_base_pt.x + _location.x, _base_pt.y + _location.y);
  return CRect(pt.x, pt.y, pt.x + _size.cx, pt.y + _size.cy);
}

inline CRect CBaseElementCtl::OriginRectangle() {
  CPoint pt(_ori_location.x, _ori_location.y);
  return CRect(pt.x, pt.y, pt.x + _ori_size.cx, pt.y + _ori_size.cy);
}

inline void CBaseElementCtl::SetPos(CPoint pt, CSize sz) {
  _location.x = pt.x;
  _location.y = pt.y;
  _size.cx = sz.cx;
  _size.cy = sz.cy;
}

inline void CBaseElementCtl::SetOriginPos(CPoint pt, CSize sz) {
  _ori_location = pt;
  _ori_size = sz;
}

inline void CBaseElementCtl::SetPos(WTL::CRect rc) {
  _location.x = rc.left;
  _location.y = rc.top;
  _size.cx = rc.Width();
  _size.cy = rc.Height();
}

inline void CBaseElementCtl::SetOriginPos(WTL::CRect rc) {
  _ori_location.x = rc.left;
  _ori_location.y = rc.top;
  _ori_size.cx = rc.Width();
  _ori_size.cy = rc.Height();
}

inline HCURSOR CBaseElementCtl::GetCursor() { return _cursor; }

inline void CBaseElementCtl::SetCursor(HCURSOR cursor) { _cursor = cursor; }

inline tstring &CBaseElementCtl::GetWndClassName() { return _class; }

inline void CBaseElementCtl::RegisterLButtonUpEvent(EventHandler event) {
  _leftup_event = event;
}

inline void CBaseElementCtl::RegisterRButtonUpEvent(EventHandler event) {
  _rightup_event = event;
}

inline void CBaseElementCtl::RegisterLButtonDownEvent(EventHandler event) {
  _leftdown_event = event;
}

inline void CBaseElementCtl::RegisterHoverEvent(EventHandler event) {
  _hover_event = event;
}

inline void CBaseElementCtl::RegisterLeaveEvent(EventHandler event) {
  _leave_event = event;
}

inline void CBaseElementCtl::RegisterEnterKeyEvent(EventHandler event) {
  _enterkey_event = event;
}

inline void CBaseElementCtl::RegisterLButtonDbCEvent(EventHandler event) {
  _leftDbclick_event = event;
}

inline BOOL CBaseElementCtl::PointInRect(CPoint pt, CRect rect) {
  return (((pt.x >= rect.left) && (pt.x <= rect.right)) &&
          ((pt.y >= rect.top) && (pt.y <= rect.bottom)));
}
