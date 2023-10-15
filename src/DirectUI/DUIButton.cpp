#include "StdAfx.h"
#include "DUIButton.h"

CButtonCtl::CButtonCtl(HWND father, BOOL trans) {
  _host = father;
  _class = _T("DirectButton");
  _transparent = trans;
  _tracking = FALSE;
  _down = FALSE;
  _state = DUI_STATE_NORMAL;
  _bmpBtn = NULL;
  _bmpIcon = NULL;
  _bmpSmallIcon = NULL;
  _colText = RGB(0, 0, 0);
  _colHover = RGB(0, 0, 0);
  _colDown = RGB(0, 0, 0);
  _colDisable = RGB(0, 0, 0);
  _vertical_flag = FALSE;
  _nTextLimitLength = 0;
  _textFont = NULL;
  _hdelta = 0;
}

CButtonCtl::CButtonCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                       LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans)
    : CBaseElementCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _host = father;
  _class = _T("DirectButton");
  _transparent = trans;
  _tracking = FALSE;
  _down = FALSE;
  _state = DUI_STATE_NORMAL;
  _bmpBtn = NULL;
  _bmpIcon = NULL;
  _bmpSmallIcon = NULL;
  _colText = RGB(0, 0, 0);
  _colHover = RGB(0, 0, 0);
  _colDown = RGB(0, 0, 0);
  _colDisable = RGB(0, 0, 0);
  _vertical_flag = FALSE;
  _nTextLimitLength = 0;
  _textFont = NULL;
  _hdelta = 0;
}

CButtonCtl::~CButtonCtl(void) {}

void CButtonCtl::SetColor(COLORREF colText, COLORREF colHover,
                          COLORREF colDown) {
  _colText = colText;
  _colHover = colHover;
  _colDown = colDown;
}

void CButtonCtl::CreateElement(XmlElementPtr element) {

  GetElementStyle(element);

  _bmpBtn = GetBitmapAttribute(element, "image");
  _bmpIcon = GetBitmapAttribute(element, "icon");
  _bmpSmallIcon = GetBitmapAttribute(element, "smallicon");
  _colText = GetRGBAttribute(element, "textcolor");
  _colHover = GetRGBAttribute(element, "hovercolor");
  _colDown = GetRGBAttribute(element, "downcolor");
  _colDisable = GetRGBAttribute(element, "discolor");
  _rcCorners = GetRectAttribute(element, "corner");
  _hdelta = GetIntegerAttribute(element, "hdelta");
  if (GetIntegerAttribute(element, "bigfont")) {
    _textFont = CGDIResource::GetInstance().GDI_GetBiggerFont();
  }
}

void CButtonCtl::SetCorner(DUI_Rect rcCorners) {
  CopyRect(&_rcCorners, &rcCorners);
}
void CButtonCtl::Disable() {
  _state = DUI_STATE_DISABLE;
  Refresh(DUI_STATE_DISABLE);
}

void CButtonCtl::Enable(BOOL enable) {
  DUI_BUTTON_STATE state;
  if (enable)
    state = DUI_STATE_NORMAL;
  else
    state = DUI_STATE_DISABLE;
  if (_state != state) {
    _state = state;
    RaiseInvalidate(TRUE);
  }
}

void CButtonCtl::SetImage(LPCTSTR lpszImageRes) {
  _bmpBtn =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszImageRes, PNG);
}

void CButtonCtl::SetImage(HBITMAP bmp) { _bmpBtn = bmp; }

void CButtonCtl::SetImageFile(LPCTSTR lpszImageFile) {
  _bmpBtn = CGDIResource::GetInstance().GDI_GetBitmap(lpszImageFile);
}

void CButtonCtl::SetIcon(LPCTSTR lpszImageRes) {
  _bmpIcon =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszImageRes, PNG);
}

void CButtonCtl::SetIcon(HBITMAP hIcon) { _bmpIcon = hIcon; }

void CButtonCtl::SetSmallIcon(LPCTSTR lpszImageRes) {
  _bmpSmallIcon =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszImageRes, PNG);
}

void CButtonCtl::SetSmallIconFile(LPCTSTR lpszImageFile) {
  _bmpSmallIcon = CGDIResource::GetInstance().GDI_GetBitmap(lpszImageFile);
}

void CButtonCtl::SetVertical() { _vertical_flag = TRUE; }

void CButtonCtl::SetTitle(const tstring &title) { _title = title; }

void CButtonCtl::OnPaint(HDC hDC) {
  if (_visible) {

    BITMAP bmpInfor;
    GetObject(_bmpBtn, sizeof(bmpInfor), &bmpInfor);

    DUI_Rect rcDest(_location, _size);
    int nImageOffset = 0;
    COLORREF col = _colText;
    switch (_state) {
    case DUI_STATE_NORMAL:
      nImageOffset = 0;
      col = _colText;
      break;
    case DUI_STATE_HOVER:
      nImageOffset = 1;
      col = _colHover;
      break;
    case DUI_STATE_DOWN:
      nImageOffset = 2;
      col = _colDown;
      break;
    case DUI_STATE_DISABLE:
      nImageOffset = 3;
      col = _colDisable;
      break;
    }

    DUI_Rect rcSrc;
    DUI_Size sz;
    if (!_vertical_flag) {
      sz.cx = bmpInfor.bmWidth / 4;
      sz.cy = bmpInfor.bmHeight;

      rcSrc.left = nImageOffset * sz.cx;
      rcSrc.top = 0;
      rcSrc.right = rcSrc.left + sz.cx;
      rcSrc.bottom = rcSrc.top + sz.cy;
    } else {
      sz.cx = bmpInfor.bmWidth;
      sz.cy = bmpInfor.bmHeight / 4;

      rcSrc.left = 0;
      rcSrc.top = nImageOffset * sz.cy;
      rcSrc.right = rcSrc.left + sz.cx;
      rcSrc.bottom = rcSrc.top + sz.cy;
    }

    if (_bmpBtn) {

      if (rcDest.Width() > rcSrc.Width() || rcDest.Height() > rcSrc.Height()) {
        if (_rcCorners.Width() == 0 && _rcCorners.Height() == 0) {
          CopyRect(&_rcCorners, CRect(3, 6, 3, 6));
        }
        CGDIResource::GetInstance().GDI_DrawImageStretch(
            hDC, _bmpBtn, rcDest, rcSrc, 255, _rcCorners);
      } else {
        CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpBtn, rcDest, rcSrc,
                                                  DUI_BT_ALPHA);
      }
    }

    DUI_Rect rcIcon;
    if (_bmpIcon) {
      BITMAP bmpInfo;
      GetObject(_bmpIcon, sizeof(bmpInfo), &bmpInfo);
      DUI_Rect rcBmp;
      sz.cx = bmpInfo.bmWidth / 4;
      sz.cy = bmpInfo.bmHeight;

      rcBmp.left = nImageOffset * sz.cx;
      rcBmp.top = 0;
      rcBmp.right = rcBmp.left + sz.cx;
      rcBmp.bottom = rcBmp.top + sz.cy;
      rcIcon.CopyRect(&rcBmp);

      POINT point;
      point.y = rcDest.top;
      point.x = rcDest.left;

      CSize size;
      SelectObject(hDC, CGDIResource::GetInstance().GDI_GetNormalFont());
      if (::GetTextExtentPoint32(hDC, _title.c_str(), _title.size(), &size)) {
        if (size.cx > 0)
          size.cx += 5;

        int nLeft = (rcDest.Width() - size.cx - rcIcon.Width()) / 2;
        point.x = nLeft > 0 ? point.x + nLeft : point.x;
        int nTop = (rcDest.Height() - rcIcon.Height()) / 2;
        point.y = nTop > 0 ? point.y + nTop : point.y;
      }

      rcIcon.MoveToXY(point);
      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpIcon, rcIcon, rcBmp,
                                                DUI_BT_ALPHA);
    }

    if (_bmpSmallIcon) {
      BITMAP bmpInfo;
      GetObject(_bmpSmallIcon, sizeof(bmpInfo), &bmpInfo);

      CSize Txtsize;
      ::GetTextExtentPoint32(hDC, _title.c_str(), _title.length(), &Txtsize);

      rcIcon.left =
          rcDest.left +
          (rcDest.right - rcDest.left - (bmpInfo.bmWidth + Txtsize.cx + 5)) / 2;
      rcIcon.top = rcDest.top + (rcDest.Height() - bmpInfo.bmHeight) / 2;
      rcIcon.right = rcIcon.left + bmpInfo.bmWidth;
      rcIcon.bottom = rcIcon.top + bmpInfo.bmHeight;

      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpSmallIcon, rcIcon);
    }

    if (!_title.empty()) {
      rcDest.top += _hdelta;
      UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
      if (_bmpIcon || _bmpSmallIcon) {
        rcDest.left = rcIcon.right + 5;
        if (_nTextLimitLength > 0)
          rcDest.right = rcDest.left + _nTextLimitLength;
        nFormat = nFormat | DT_LEFT;
      } else
        nFormat = nFormat | DT_CENTER;

      if (_transparent) {
        if (!_textFont)
          CGDIResource::GetInstance().GDI_DrawText_Trans(hDC, _title.c_str(),
                                                         &rcDest, col, nFormat);
        else if (DUI_STATE_DISABLE == _state)
          CGDIResource::GetInstance().GDI_DrawText_Trans(
              hDC, _title.c_str(), &rcDest, _textFont, col, nFormat);
        else
          CGDIResource::GetInstance().GDI_DrawText_Trans(
              hDC, _title.c_str(), &rcDest, _textFont, col, nFormat);
      } else {
        CGDIResource::GetInstance().GDI_DrawText(hDC, _title.c_str(), &rcDest,
                                                 _textFont, col, nFormat);
      }
    }
  }
}

void CButtonCtl::OnMouseHover(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    if (_down) {
      Refresh(DUI_STATE_DOWN);
    } else {
      Refresh(DUI_STATE_HOVER);
    }
  }
}

void CButtonCtl::OnMouseLeave() {
  _down = FALSE;
  _tracking = FALSE;
  if (_state != DUI_STATE_DISABLE) {
    Refresh(DUI_STATE_NORMAL);
  }
}

void CButtonCtl::Refresh(DUI_BUTTON_STATE nowstate) {
  if (_state != nowstate) {
    _state = nowstate;
    RaiseInvalidate(TRUE);
  }
}

void CButtonCtl::OnMouseDown(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    _down = TRUE;
    Refresh(DUI_STATE_DOWN);
    OnMouseDownClick();
  }
}

void CButtonCtl::OnMouseUp(UINT nFlags, CPoint point) {
  if (_visible) {
    if (_state != DUI_STATE_DISABLE) {
      DUI_BUTTON_STATE state = _state;
      Refresh(DUI_STATE_HOVER);

      if (_down && (state == DUI_STATE_DOWN)) {
        OnMouseClick();
      }

      _down = FALSE;
    }
  }
}

void CButtonCtl::OnMouseDownClick() { _leftdown_event.Invoke(); }

void CButtonCtl::SetTextLimitLength(int nLength) {
  _nTextLimitLength = nLength;
}

void CButtonCtl::OnCall_LeftUp() {
  if (_leftup_event.m_func_no_para)
    _leftup_event.Invoke();

  else if (_leftup_event.m_func_para_string)
    _leftup_event.Invoke(_id.c_str());
}

void CButtonCtl::OnCall_RightUp() {
  if (_rightup_event.m_func_no_para)
    _rightup_event.Invoke();

  else if (_rightup_event.m_func_para_string)
    _rightup_event.Invoke(_id.c_str());
}
