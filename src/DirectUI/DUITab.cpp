#include "StdAfx.h"
#include "DUITab.h"
#include "DUITabList.h"

CTabCtl::CTabCtl(HWND father, CTabListCtl *pList, BOOL trans)
    : CButtonCtl(father, trans) {
  _class = _T("");
  _tab_list = pList;
  _bSel = FALSE;
  _bmpUpdate = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
                                                         _T("TABUPDATE"), PNG);
  _nCount = 0;
  _transparent = trans;
}

CTabCtl::CTabCtl(HWND hWnd, LPCTSTR lpszID, LPCTSTR lpszTitle,
                 LPCTSTR lpszTooptip, DUI_Rect rcElement)
    : CButtonCtl(hWnd, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _class = _T("");
  _bSel = FALSE;
  _bmpUpdate = CGDIResource::GetInstance().GDI_GetBitmap(
      _hInstance, _T("CHANNELUPDATE"), PNG);
  _nCount = 0;
}

CTabCtl::~CTabCtl(void) {}

void CTabCtl::OnPaint(HDC hDC) { CButtonCtl::OnPaint(hDC); }

void CTabCtl::OnMouseHover(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    if (_down) {
      Refresh(DUI_STATE_DOWN);
    } else {
      Refresh(DUI_STATE_HOVER);
    }
  }
}

void CTabCtl::OnMouseLeave() {
  _tracking = FALSE;
  if (_state != DUI_STATE_DISABLE) {
    if (_down) {
      Refresh(DUI_STATE_DOWN);
    } else {
      Refresh(DUI_STATE_NORMAL);
    }
  }
}

void CTabCtl::OnMouseUp(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    DUI_BUTTON_STATE state = _state;

    if (_down && (state == DUI_STATE_DOWN)) {

      OnMouseClick();

      if (_tab_list)
        _tab_list->SelTab(_id);
    }
  }
}
void CTabCtl::SetSel(BOOL bSel) {
  _bSel = bSel;

  if (_state != DUI_STATE_DISABLE) {
    if (_bSel) {
      _down = TRUE;

      Refresh(DUI_STATE_DOWN);
    } else {
      _down = FALSE;

      Refresh(DUI_STATE_NORMAL);
    }
  }
}

BOOL CTabCtl::GetSel() { return _bSel; }

void CTabCtl::SetUpdate(int nCount) {
  _nCount = nCount;
  RaiseInvalidate(TRUE);
}

void CTabCtl::RaiseInvalidate(BOOL forceRedraw) {
  if (_host != NULL) {
    if (forceRedraw) {
      DUI_Rect rcTab = Rectangle();
      rcTab.top -= 2;
      rcTab.right += 2;
      ::InvalidateRect(_host, rcTab, TRUE);
    }
  }
}
