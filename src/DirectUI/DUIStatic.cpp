#include "StdAfx.h"
#include "DUIStatic.h"

CStaticCtl::CStaticCtl(HWND father, BOOL trans) {
  _host = father;

  _down = FALSE;
  _class = _T("");
  _transparent = trans;
  _bmpBackground = NULL;
  _bmpIcon = NULL;
  _textFont = CGDIResource::GetInstance().GDI_GetNormalFont();
  _textCol = RGB(0, 0, 0);
  _textFormat = DT_LEFT;
  _viewport = 0;
  _scrollDelta = 0;
}

CStaticCtl::CStaticCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                       LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans)
    : CBaseElementCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _host = father;

  _class = _T("");
  _bmpBackground = NULL;
  _bmpIcon = NULL;
  _textFont = CGDIResource::GetInstance().GDI_GetNormalFont();
  _textCol = RGB(0, 0, 0);
  _textFormat = DT_LEFT;
  _transparent = trans;
  _viewport = 0;
  _scrollDelta = 0;
}

CStaticCtl::~CStaticCtl(void) {}

void CStaticCtl::CreateElement(XmlElementPtr element) {
  GetElementStyle(element);

  _bmpBackground = GetBitmapAttribute(element, "background");
  _textCol = GetRGBAttribute(element, "color");
  _bmpIcon = GetBitmapAttribute(element, "icon");
  if (GetIntegerAttribute(element, "bigfont")) {
    _textFont = CGDIResource::GetInstance().GDI_GetBiggerFont();
  }
  if (GetIntegerAttribute(element, "samllfont")) {
    _textFont = CGDIResource::GetInstance().GDI_GetSmallFont();
  }
  _viewport = GetIntegerAttribute(element, "viewport");
}
void CStaticCtl::SetTextFont(HFONT font) { _textFont = font; }

void CStaticCtl::SetTextColor(COLORREF colText) { _textCol = colText; }

void CStaticCtl::SetImage(LPCTSTR lpszImageRes) {
  _bmpBackground =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszImageRes, PNG);
}

void CStaticCtl::SetIcon(LPCTSTR lpszIconRes) {
  _bmpIcon =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszIconRes, PNG);
}

void CStaticCtl::SetTextFormat(UINT nTextFormat) { _textFormat = nTextFormat; }

void CStaticCtl::OnPaint(HDC hDC) {
  if (_visible) {
    UINT nTextFormat = DT_LEFT;
    DUI_Rect rcText(_location, _size);
    if (_bmpIcon) {
      BITMAP bmpIconInfor;
      GetObject(_bmpIcon, sizeof(bmpIconInfor), &bmpIconInfor);

      int nY = _location.y + (_size.cy - bmpIconInfor.bmHeight) / 2;

      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpIcon, _location.x, nY,
                                                bmpIconInfor.bmWidth,
                                                bmpIconInfor.bmHeight);

      rcText.left = _location.x + bmpIconInfor.bmWidth + 2;
      nTextFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
    }
    if (_title.size() > 0) {
      if (_viewport) {
        ::SetWindowOrgEx(hDC, 0, _scrollDelta, NULL);
        rcText.bottom += _scrollDelta;
      }
      if (_textFormat != 0) {
        nTextFormat = _textFormat;
      }

      if (_transparent) {
        CGDIResource::GetInstance().GDI_DrawText_Trans(
            hDC, _title.c_str(), &rcText, _textFont, _textCol, nTextFormat);
      } else {
        CGDIResource::GetInstance().GDI_DrawText(
            hDC, _title.c_str(), &rcText, _textFont, _textCol, nTextFormat);
      }
      if (_viewport) {
        ::SetWindowOrgEx(hDC, 0, 0, NULL);
      }
    }

    if (_bmpBackground) {

      int nWidth = _size.cx;

      int nHeight = _size.cy;
      CGDIResource::GetInstance().GDI_DrawImage(
          hDC, _bmpBackground, _location.x, _location.y, nWidth, nHeight);
    }
  }
}

void CStaticCtl::OnMouseDown(UINT nFlags, CPoint point) {
  if (_visible) {
    _down = TRUE;
  }
}

void CStaticCtl::OnMouseUp(UINT nFlags, CPoint point) {
  if (_visible) {
    if (_down) {
      OnMouseClick();
    }
    _down = FALSE;
  }
}

void CStaticCtl::OnCall_LeftUp() {
  if (_leftup_event.m_func_no_para)
    _leftup_event.Invoke();
}
