#include "StdAfx.h"
#include <regex>
#include "util/def.h"
#include "../Global.h"
#include "DUILayeredEdit.h"

CLayeredEditCtl::CLayeredEditCtl(HWND father) : _bmpBack(NULL) {
  _host = father;
  _class = _T("");
  _state = DUI_STATE_NORMAL;
  _borderColor = RGB(60, 152, 208);
  _bmpNormal = NULL;
  _bmpHover = NULL;
  _bmpIcon = NULL;
  _readonly = FALSE;
  _transparent = TRUE;
  _password = FALSE;
  _index = 0;
  _hPopUpWnd = NULL;
  _LimitText = 5000;
  _brushColor = RGB(255, 255, 255);
  _textcolor = RGB(0, 0, 0);
}

void CLayeredEditCtl::CreateElement(XmlElementPtr element) {
  GetElementStyle(element);
  _tab = CXmlHelper::GetInstance().GetAttributeInt(element, "tab");
  _index = CXmlHelper::GetInstance().GetAttributeInt(element, "index");
  _bmpNormal = GetBitmapAttribute(element, "normalBack");
  _bmpHover = GetBitmapAttribute(element, "hoverBack");
  _bmpBack = GetBitmapAttribute(element, "image");
  _bmpIcon = GetBitmapAttribute(element, "icon");
  _password = CXmlHelper::GetInstance().GetAttributeInt(element, "pWd");
  _readonly = CXmlHelper::GetInstance().GetAttributeInt(element, "readonly");
  _borderColor = GetRGBAttribute(element, "bordercolor");
  _brushColor = GetRGBAttribute(element, "backcolor");
  _textcolor = GetRGBAttribute(element, "textcolor");
  _editCheckPath = GetIntegerAttribute(element, "checkpath");

  if (!_bmpIcon) {
    DUI_Rect rcEdit(_location, _size);
    rcEdit.left += 10;
    rcEdit.right -= 10;
    rcEdit.top += 5;
    rcEdit.bottom -= 2;
    _rcEdit = rcEdit;
  } else {
    BITMAP bmpInfor;
    GetObject(_bmpIcon, sizeof(bmpInfor), &bmpInfor);

    DUI_Rect rcEdit(_location, _size);
    rcEdit.left += bmpInfor.bmWidth + 10;
    rcEdit.right -= 10;
    rcEdit.top += 5;
    rcEdit.bottom -= 2;
    _rcEdit = rcEdit;
  }
}

void CLayeredEditCtl::SetLimitText(int uMax) { _LimitText = uMax; }

void CLayeredEditCtl::OnPaint(HDC hDC) {
  if (_visible) {
    DUI_Rect rc(_location, _size);

    if (_bmpBack)
      CGDIResource::GetInstance().GDI_DrawImage(
          hDC, _bmpBack, _location.x, _location.y, _size.cx, _size.cy);
    else
      CGDIResource::GetInstance().GDI_DrawRectangle_Trans(
          hDC, &rc, _borderColor, _brushColor);
    if (_password) {
      const int c = _title.length();
      int i = c;
      WCHAR *s = new WCHAR[++i];
      for (int k = 0; k < c; ++k) {
        s[k] = _T('*');
      }
      s[c] = NULL;
      if (c > 0) {
        CGDIResource::GetInstance().GDI_DrawText_Trans(
            hDC, s, &_rcEdit, _textcolor, DT_SINGLELINE);
      }
      delete[] s;
      return;
    }
    CGDIResource::GetInstance().GDI_DrawText_Trans(
        hDC, _title.c_str(), &_rcEdit, _textcolor, DT_SINGLELINE);
  }
}

void CLayeredEditCtl::OnSizeChanged(DUI_Size wndChangingSize, int w, int h) {}

void CLayeredEditCtl::OnMove() {}

LRESULT EditPopWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) {
  if (::IsWindow(m_hWnd)) {
    RECT rc;
    GetClientRect(&rc);
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL;
    m_edit.Create(m_hWnd, rc, NULL, dwStyle, WS_EX_TOOLWINDOW);
    if (::IsWindow(m_edit.m_hWnd)) {
      m_edit.SetFont(CGDIResource::GetInstance().GDI_GetNormalFont(), TRUE);
      ::SetFocus(m_edit.m_hWnd);
    }
  }
  return 0;
}

LRESULT EditPopWindow::OnPaintChild(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL &bHandled) {
  ::SetBkColor((HDC)wParam, RGB(69, 68, 73));
  ::SetTextColor((HDC)wParam, RGB(255, 255, 255));
  return (LRESULT)_hbrush;
}

LRESULT EditPopWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL &bHandled) {
  DestroyWindow();
  return 0;
}

void EditPopWindow::OnFinalMessage(HWND hWnd) { delete this; }

void CLayeredEditCtl::GetTitleIn() {
  BSTR strText = NULL;
  _hPopUpWnd->m_edit.GetWindowText(strText);
  CString strRes = strText;
  _title = (LPCTSTR)strRes;
  SysFreeString(strText);
}

void CMyEditEx::DeleObj(void *p) { delete p; }

LRESULT CMyEditEx::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam,
                          BOOL &bHandled) {
  WCHAR nChar = (WCHAR)wParam;
  if (nChar == 13) {
    if (m_pOwner) {
      m_pOwner->OnTabKey();
      m_pOwner->OnEnterKey();
    }
  }
  if (nChar == 9) {
    if (m_pOwner) {
      m_pOwner->OnTabKey();
    }
  }

  if (m_pOwner && ::IsWindow(m_pOwner->_host) && m_pOwner->_editCheckPath) {
    ::PostMessage(m_pOwner->_host, WM_PATH_ILLCHARACTER,
                  (WPARAM)m_pOwner->_editCheckPath, 0);
  }
  bHandled = FALSE;
  return 0;
}

LRESULT CMyEditEx::OnActiving(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) {
  if (m_pOwner) {
    ((CLayeredEditCtl *)m_pOwner)->GetTitleIn();
    HWND hParent = ::GetParent(m_hWnd);

    ((CLayeredEditCtl *)m_pOwner)->_hPopUpWnd = NULL;
    ::ShowWindow(hParent, SW_HIDE);
    m_pOwner->RaiseInvalidate(TRUE);
    ::PostMessage(global.hUitlWnd_, WM_DEL_GDI_OBJ, 0, (LPARAM)hParent);
  }
  return 0;
}

void CLayeredEditCtl::OnMouseDown(UINT nFlags, CPoint point) { ForceCreate(); }

void CLayeredEditCtl::ForceCreate() {
  if (NULL == _hPopUpWnd) {
    _hPopUpWnd = new EditPopWindow;
  }
  if (!_hPopUpWnd) {
    ATLASSERT(0);
    return;
  }
  if (::IsWindow(_hPopUpWnd->m_hWnd)) {
    return;
  }
  _hPopUpWnd->m_edit.SetOwner(this);
  _hPopUpWnd->m_hWnd = NULL;
  RECT src = {0};
  DUI_Rect rcParent = src;
  ::GetWindowRect(_host, &rcParent);
  DUI_Rect rc(
      DUI_Point(rcParent.left + _rcEdit.left, rcParent.top + _rcEdit.top),
      _rcEdit.Size());
  DWORD style = WS_POPUP | WS_VISIBLE;
  _hPopUpWnd->Create(_host, rc, _T(""), style);
  if (::IsWindow(_hPopUpWnd->m_edit.m_hWnd)) {
    _hPopUpWnd->m_edit.SetLimitText(_LimitText);
    ::SetWindowText(_hPopUpWnd->m_edit.m_hWnd, _title.c_str());
  } else {
    ATLASSERT(0);
  }
}

void CLayeredEditCtl::DestroyEditWindow() {
  if (_hPopUpWnd && IsWindow(_hPopUpWnd->m_hWnd)) {
    GetTitleIn();
    void *p = _hPopUpWnd;
    RaiseInvalidate(TRUE);
    ::DestroyWindow(_hPopUpWnd->m_hWnd);
    ::PostMessage(global.hUitlWnd_, WM_DEL_GDI_OBJ, 0, (LPARAM)p);
  }
}
