#pragma once

#include "DUIElement.h"

class CLayeredEditCtl;

class CMyEditEx : public CWindowImpl<CMyEditEx, CEdit> {
public:
  CMyEditEx() { m_pOwner = NULL; }
  static void DeleObj(void *p);
  BEGIN_MSG_MAP(CMyEditEx)
  MESSAGE_HANDLER(WM_CHAR, OnChar)
  MESSAGE_HANDLER(WM_KILLFOCUS, OnActiving)
  END_MSG_MAP()

  LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnActiving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  void SetOwner(CBaseElementCtl *pOwner) { m_pOwner = pOwner; }
  void OnFinalMessage(HWND hWnd) {}

private:
  CBaseElementCtl *m_pOwner;
};

class EditPopWindow : public CWindowImpl<EditPopWindow> {
public:
  EditPopWindow() { _hbrush = CreateSolidBrush(RGB(69, 68, 73)); }
  ~EditPopWindow() {
    if (_hbrush)
      DeleteObject((HGDIOBJ)_hbrush);
  }
  BEGIN_MSG_MAP(EditPopWindow)

  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnPaintChild)
  MESSAGE_HANDLER(WM_CLOSE, OnClose)
  END_MSG_MAP()

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnPaintChild(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

  void OnFinalMessage(HWND hWnd);
  CMyEditEx m_edit;

private:
  HBRUSH _hbrush;
};

class CLayeredEditCtl : public CBaseElementCtl {
public:
  friend CMyEditEx;
  CLayeredEditCtl(HWND father);
  ~CLayeredEditCtl(void) {}
  void OnMouseDown(UINT nFlags, CPoint point);
  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void OnSizeChanged(DUI_Size wndChangingSize, int w, int h);
  void OnMove();
  void SetVisible(BOOL visible);
  void SetPos(WTL::CRect rc);
  void SetPos(CPoint pt, CSize sz);
  void SetLimitText(int uMax);
  tstring GetTitle() override;
  void SetTitle(const tstring &csName);
  virtual void OnTabKey();
  EditPopWindow *_hPopUpWnd;
  CRect _rcEdit;
  DUI_BUTTON_STATE _state;
  void DestroyEditWindow();
  void ForceCreate();

protected:
  void GetTitleIn();
  COLORREF _borderColor;
  COLORREF _brushColor;
  COLORREF _textcolor;
  HBITMAP _bmpNormal;
  HBITMAP _bmpHover;
  HBITMAP _bmpIcon;
  BOOL _password;
  BOOL _tab;
  BOOL _readonly;
  HBITMAP _bmpBack;
  int _index;
  int _LimitText;
};
inline void CLayeredEditCtl::OnTabKey() {
  if (_tab) {
    ::PostMessage(_host, WM_ASYNC_CALL_TABKEY, (WPARAM)this, _index);
  }
}

inline void CLayeredEditCtl::SetTitle(const tstring &csName) {
  _title = csName;

  RaiseInvalidate();
}

inline void CLayeredEditCtl::SetVisible(BOOL visible) {
  if (_visible != visible) {
    _visible = visible;
  }
}

inline tstring CLayeredEditCtl::GetTitle() {
  if (_hPopUpWnd && ::IsWindow(_hPopUpWnd->m_hWnd)) {
    BSTR strText = NULL;
    _hPopUpWnd->m_edit.GetWindowText(strText);
    CString strRes = strText;
    _title = (LPCTSTR)strRes;
    SysFreeString(strText);
  }
  return _title;
}

inline void CLayeredEditCtl::SetPos(CPoint pt, CSize sz) {
  _location.x = pt.x;
  _location.y = pt.y;
  _size.cx = sz.cx;
  _size.cy = sz.cy;

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

inline void CLayeredEditCtl::SetPos(WTL::CRect rc) {
  DUI_Point pt;
  pt.x = rc.left;
  pt.y = rc.top;

  DUI_Size sz;
  sz.cx = rc.Width();
  sz.cy = rc.Height();
  SetPos(pt, sz);
}
