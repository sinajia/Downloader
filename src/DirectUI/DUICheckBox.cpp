#include "StdAfx.h"
#include "DUICheckBox.h"

CCheckBoxCtl::CCheckBoxCtl(HWND father, BOOL trans) : CButtonCtl(father) {
  _class = _T("");
  _transparent = trans;
  _txtColor = RGB(67, 75, 94);
  _bChecked = FALSE;
  _bmpBtn = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
                                                      _T("CHECK_BOX"), PNG);
}

CCheckBoxCtl::CCheckBoxCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                           LPCTSTR lpszTooptip, DUI_Rect rcElement)
    : CButtonCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _class = _T("");
  _txtColor = RGB(67, 75, 94);
  _bChecked = FALSE;
  _bmpBtn = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
                                                      _T("CHECK_BOX"), PNG);
}

CCheckBoxCtl::~CCheckBoxCtl(void) {}

void CCheckBoxCtl::CreateElement(XmlElementPtr element) {

  GetElementStyle(element);

  _bmpBtn = GetBitmapAttribute(element, "image");

  _txtColor = GetRGBAttribute(element, "textcolor");
}

void CCheckBoxCtl::Enable(BOOL enable) {
  if (enable) {
    switch (_state) {
    case DUI_STATE_NORMAL_DISABLE:
      _state = DUI_STATE_NORMAL;
      break;
    case DUI_STATE_DOWN_DISABLE:
      _state = DUI_STATE_DOWN;
      break;
    default:
      break;
    }
  } else {
    Disable();
  }
}

void CCheckBoxCtl::OnPaint(HDC hDC) {
  BITMAP bmpInfor;
  GetObject(_bmpBtn, sizeof(bmpInfor), &bmpInfor);

  int nCheckWidth = bmpInfor.bmWidth / 2;

  DUI_Size szCheckImage(nCheckWidth, bmpInfor.bmHeight);
  DUI_Point ptCheckImage(_location.x,
                         _location.y + (_size.cy - bmpInfor.bmHeight) / 2);
  DUI_Rect rcImage(ptCheckImage, szCheckImage);
  int nImageOffset = 0;
  switch (_state) {
  case DUI_STATE_NORMAL:
  case DUI_STATE_HOVER:
    nImageOffset = 0;
    break;
  case DUI_STATE_DOWN:
    nImageOffset = 1;
    break;
  case DUI_STATE_NORMAL_DISABLE:
    nImageOffset = 2;
    break;
  case DUI_STATE_DOWN_DISABLE:
    nImageOffset = 3;
    break;
  }

  DUI_Rect rcSrc(DUI_Point(nImageOffset * nCheckWidth, 0),
                 DUI_Size(nCheckWidth, bmpInfor.bmHeight));

  CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpBtn, rcImage, rcSrc);

  DUI_Rect rcText(DUI_Point(rcImage.right + 5, _location.y),
                  DUI_Size(_size.cx - rcImage.Width() - 1, _size.cy));
  if (_transparent) {
    CGDIResource::GetInstance().GDI_DrawText_Trans(
        hDC, _title.c_str(), &rcText, _txtColor,
        DT_LEFT | DT_SINGLELINE | DT_VCENTER);
  } else {
    CGDIResource::GetInstance().GDI_DrawText(
        hDC, _title.c_str(), &rcText, _txtColor,
        DT_LEFT | DT_SINGLELINE | DT_VCENTER);
  }
}

void CCheckBoxCtl::Disable() {
  switch (_state) {
  case DUI_STATE_NORMAL:
    _state = DUI_STATE_NORMAL_DISABLE;
    break;
  case DUI_STATE_DOWN:
    _state = DUI_STATE_DOWN_DISABLE;
    break;
  default:
    break;
  }
}

void CCheckBoxCtl::OnMouseDown(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    _down = !_down;
    if (_down) {
      Refresh(DUI_STATE_DOWN);
      _bChecked = TRUE;
    } else {
      Refresh(DUI_STATE_NORMAL);
      _bChecked = FALSE;
    }
    OnMouseDClick();
  }
}

void CCheckBoxCtl::OnMouseUp(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    OnMouseClick();
  }
}

void CCheckBoxCtl::OnMouseHover(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    if (_down) {
      Refresh(DUI_STATE_DOWN);
    } else {
      Refresh(DUI_STATE_HOVER);
    }
  }
}

void CCheckBoxCtl::OnMouseLeave() {
  _tracking = FALSE;
  if (_state != DUI_STATE_DISABLE) {
    if (!_down) {
      Refresh(DUI_STATE_NORMAL);
    }
  }
}

void CCheckBoxCtl::SetCheck(BOOL bCheck) {
  _bChecked = bCheck;

  if (_state != DUI_STATE_DISABLE) {
    if (_bChecked) {
      _state = DUI_STATE_DOWN;
      _down = TRUE;
    } else {
      _state = DUI_STATE_NORMAL;
      _down = FALSE;
    }
  }
}

BOOL CCheckBoxCtl::GetCheck() { return _bChecked; }
