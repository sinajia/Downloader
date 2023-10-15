#include "StdAfx.h"
#include "DUIElement.h"

CBaseElementCtl::CBaseElementCtl() : _editCheckPath(FALSE) {
  _handleMouse = TRUE;
  _handleKeyboard = FALSE;
  _viscur = FALSE;
  _active = FALSE;
  _host = NULL;
  _visible = TRUE;
  _transparent = FALSE;
  _id = _T("");
  _name = _T("");
  _base_pt.x = 0;
  _base_pt.y = 0;
  _location.x = 0;
  _location.y = 0;
  _size.cx = 0;
  _size.cy = 0;
  _cursor = LoadCursor(NULL, IDC_ARROW);
}

CBaseElementCtl::CBaseElementCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                                 LPCTSTR lpszTooptip, DUI_Rect rcElement)
    : _editCheckPath(FALSE) {

  _handleMouse = TRUE;
  _handleKeyboard = FALSE;
  _viscur = FALSE;
  _active = FALSE;
  _host = father;
  _visible = TRUE;
  _id = lpszID;
  _name = lpszID;
  _title = lpszTitle;
  _tip = lpszTooptip;
  _base_pt.x = 0;
  _base_pt.y = 0;
  _location.x = rcElement.left;
  _location.y = rcElement.top;
  _size.cx = rcElement.Width();
  _size.cy = rcElement.Height();
  _cursor = LoadCursor(NULL, IDC_ARROW);
}

CBaseElementCtl::~CBaseElementCtl(void) {}

void CBaseElementCtl::CreateElement(XmlElementPtr element) {}

void CBaseElementCtl::GetElementStyle(XmlElementPtr element) {
  CXmlHelper helper;

  _id = helper.GetAttributeString(element, "id");

  _name = helper.GetAttributeString(element, "name");

  _title = helper.GetAttributeString(element, "title");

  _tip = helper.GetAttributeString(element, "tip");

  _style = helper.GetAttributeInt(element, "style");

  _ex_style = helper.GetAttributeInt(element, "style_ex");

  _location.x = helper.GetAttributeInt(element, "left");
  _location.y = helper.GetAttributeInt(element, "top");
  _size.cx = helper.GetAttributeInt(element, "width");
  _size.cy = helper.GetAttributeInt(element, "height");
  _ori_location = _location;
  _ori_size = _size;

  LoadStyle(element);
}

void CBaseElementCtl::OnKeyDown(DWORD dwFlag) {}

void CBaseElementCtl::OnKeyUp(DWORD dwFlag) {}

void CBaseElementCtl::OnMouseClick() {
  ::PostMessage(_host, WM_ASYNC_CALL_LUP, (WPARAM)this, 0);
}

void CBaseElementCtl::OnMouseDClick() {
  ::PostMessage(_host, WM_ASYNC_CALL_LDOWN, (WPARAM)this, 0);
}

void CBaseElementCtl::OnCall_LeftUp() { _leftup_event.Invoke(); }

void CBaseElementCtl::OnCall_LeftDown() { _leftdown_event.Invoke(); }

void CBaseElementCtl::OnMove() {}
void CBaseElementCtl::OnMouseDblDown(UINT nFlags, CPoint point) {
  OnMouseDClick();
}

void CBaseElementCtl::OnMouseDown(UINT nFlags, CPoint point) {}

void CBaseElementCtl::OnMouseUp(UINT nFlags, CPoint point) {}

void CBaseElementCtl::OnMouseLDbClick(UINT nFlags, CPoint point) {}

void CBaseElementCtl::OnMouseRUp(UINT nFlags, CPoint point) {
  ::PostMessage(_host, WM_ASYNC_CALL_RUP, (WPARAM)this, 0);
}
void CBaseElementCtl::OnCall_RightUp() { _rightup_event.Invoke(); }

void CBaseElementCtl::OnCall_LDbClick() {
  if (_leftDbclick_event.m_func_no_para)
    _leftDbclick_event.Invoke();
  else if (_leftDbclick_event.m_func_para_string)
    _leftDbclick_event.Invoke(_id.c_str());
}

void CBaseElementCtl::OnEnterKey() {
  ::PostMessage(_host, WM_ASYNC_CALL_ENTERKEY, (WPARAM)this, 0);
}

void CBaseElementCtl::OnCall_EnterKey() { _enterkey_event.Invoke(); }

void CBaseElementCtl::OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled) {}

void CBaseElementCtl::OnMouseHover(UINT nFlags, CPoint point) {
  _hover_event.Invoke();
}

void CBaseElementCtl::OnMouseLeave() { _leave_event.Invoke(); }

void CBaseElementCtl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {}

void CBaseElementCtl::OnPaint(HDC hDC) {}

void CBaseElementCtl::OnSizeChanged(DUI_Size wndChangingSize, int w, int h) {
  CRect rc;
  rc = StyleRectToWindowRect(DUI_Rect(_location, _size), _ori_location,
                             wndChangingSize, CSize(w, h), _xAlignment,
                             _yAlignment, _xStretch, _yStretch);

  _location.x = rc.left;
  _location.y = rc.top;
  _size.cx = rc.Width();
  _size.cy = rc.Height();
}

void CBaseElementCtl::RaiseInvalidate(BOOL forceRedraw) {
  if (_host != NULL) {
    if (forceRedraw) {
      if (_transparent) {
        ::PostMessage(_host, MSG_UPDATE_WINDOW_INVALIDATE, 0, 100);
      } else {
        ::InvalidateRect(_host, Rectangle(), TRUE);
      }
    }
  }
}

int CBaseElementCtl::GetTextWidth(LPCTSTR lpszText, HFONT hFont) {
  CPaintDC dc(_host);
  HFONT hOldFont = dc.SelectFont(hFont);
  ::SetBkMode(dc.m_hDC, TRANSPARENT);
  CSize szText;
  ::GetTextExtentPoint32(dc.m_hDC, lpszText, lstrlen(lpszText), &szText);
  dc.SelectFont(hOldFont);

  return szText.cx;
}
