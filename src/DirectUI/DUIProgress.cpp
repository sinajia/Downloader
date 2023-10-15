#include "StdAfx.h"
#include "DUIProgress.h"

CProgressCtl::CProgressCtl(HWND father, BOOL trans)
    : _bmpLightPoint(NULL), _nCurPos(0.0), _nActualWidth(0),
      _lightpointdela(0) {
  _host = father;
  _handleMouse = FALSE;
  _class = _T("");
  _bmpBackground = NULL;
  _bmpProgress = NULL;
  _backcolor = RGB(53, 53, 61);
  _progresscolor = RGB(144, 197, 38);
  _transparent = trans;
}

CProgressCtl::CProgressCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                           LPCTSTR lpszTooptip, DUI_Rect rcElement, BOOL trans)
    : CBaseElementCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement),
      _bmpLightPoint(NULL), _nCurPos(0.0), _nActualWidth(0),
      _lightpointdela(0) {
  _host = father;
  _handleMouse = FALSE;
  _class = _T("");
  _bmpBackground = NULL;
  _bmpProgress = NULL;
  _backcolor = RGB(53, 53, 61);
  _progresscolor = RGB(144, 197, 38);
  _transparent = trans;
}

CProgressCtl::~CProgressCtl(void) {}

void CProgressCtl::CreateElement(XmlElementPtr element) {

  GetElementStyle(element);

  _bmpBackground = GetBitmapAttribute(element, "background");
  _bmpProgress = GetBitmapAttribute(element, "progress");
  _nActualWidth = GetIntegerAttribute(element, "progress_width");
  _bmpLightPoint = GetBitmapAttribute(element, "image");
  _lightpointdela = GetIntegerAttribute(element, "lightpointdela");
}

void CProgressCtl::SetPos(double nPos) {
  _nCurPos = nPos;
  RaiseInvalidate(TRUE);
}

void CProgressCtl::OnPaint(HDC hDC) {
  if (_visible) {
    if (_bmpBackground) {

      int nWidth = _size.cx;
      int nHeight = _nActualWidth;
      CGDIResource::GetInstance().GDI_DrawImage(
          hDC, _bmpBackground, _location.x, _location.y, nWidth, nHeight);
    } else {
      DUI_Rect rcDest(_location.x, _location.y, _location.x + _nActualWidth,
                      _location.y + _size.cy);
      if (_transparent)
        CGDIResource::GetInstance().GDI_DrawRectangle_Trans(
            hDC, &rcDest, _backcolor, _backcolor);
      else
        CGDIResource::GetInstance().GDI_DrawRectangle(hDC, &rcDest, _backcolor,
                                                      _backcolor);
    }

    if (_bmpProgress) {

      int nDrawWidth = static_cast<int>(_nActualWidth * _nCurPos);

      BITMAP bmpProgressInfor;
      GetObject(_bmpProgress, sizeof(bmpProgressInfor), &bmpProgressInfor);
      int nDrawLeft = _location.x + 8;
      int nDrawTop = _location.y + 7;
      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpProgress, nDrawLeft,
                                                nDrawTop, nDrawWidth,
                                                bmpProgressInfor.bmHeight);
    } else {
      int nDrawWidth = static_cast<int>(_nActualWidth * _nCurPos);
      DUI_Rect rcDest(_location.x, _location.y, _location.x + nDrawWidth,
                      _location.y + _size.cy);
      if (_transparent)
        CGDIResource::GetInstance().GDI_DrawRectangle_Trans(
            hDC, &rcDest, _progresscolor, _progresscolor);
      else
        CGDIResource::GetInstance().GDI_DrawRectangle(
            hDC, &rcDest, _progresscolor, _progresscolor);
    }

    if (_bmpLightPoint) {
      BITMAP bmpBackInfor;
      GetObject(_bmpLightPoint, sizeof(bmpBackInfor), &bmpBackInfor);
      int nWidth = bmpBackInfor.bmWidth;
      int nHeight = bmpBackInfor.bmHeight;
      int px = _location.x + static_cast<int>(_nActualWidth * _nCurPos) -
               nWidth / 2 + _lightpointdela;
      int py = _location.y + _size.cy / 2 - nHeight / 2;
      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpLightPoint, px, py,
                                                nWidth, nHeight);
    }

    TCHAR szArgs[25] = {0};
    _sntprintf_s(szArgs, 25, _TRUNCATE, _T("%.1f%%"), _nCurPos * 100);
    CRect rcText;
    rcText.left = _nActualWidth + 30;
    rcText.top = _location.y - 10;
    rcText.right = rcText.left + 55;
    rcText.bottom = rcText.top + 20;
    UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
    nFormat = nFormat | DT_LEFT;
    if (_transparent) {
      CGDIResource::GetInstance().GDI_DrawText_Trans(
          hDC, szArgs, &rcText, CGDIResource::GetInstance().GDI_GetSmallFont(),
          RGB(250, 250, 250), nFormat);
    } else {
      CGDIResource::GetInstance().GDI_DrawText(
          hDC, szArgs, &rcText, CGDIResource::GetInstance().GDI_GetSmallFont(),
          RGB(250, 250, 250), nFormat);
    }
  }
}
