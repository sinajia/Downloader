#include "StdAfx.h"
#include "DUIOptionLine.h"

COptionLineCtl::COptionLineCtl(HWND father, BOOL trans) {
  _host = father;
  _class = _T("DUIOptionLine");
  _transparent = trans;
  _textFont = NULL;
  _textCol = RGB(0, 0, 0);
  _lineCol = RGB(0, 0, 0);
}

COptionLineCtl::COptionLineCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                               LPCTSTR lpszTooptip, DUI_Rect rcElement,
                               BOOL trans)
    : CBaseElementCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _host = father;
  _class = _T("DUIOptionLine");
  _transparent = trans;
  _textFont = NULL;
  _textCol = RGB(0, 0, 0);
  _lineCol = RGB(0, 0, 0);
}

COptionLineCtl::~COptionLineCtl(void) {}

void COptionLineCtl::CreateElement(XmlElementPtr element) {

  GetElementStyle(element);
  _textFont = CGDIResource::GetInstance().GDI_GetNormalFont();
  _textCol = GetRGBAttribute(element, "textcolor");
  _lineCol = GetRGBAttribute(element, "linecolor");
}
void COptionLineCtl::SetTextFont(HFONT font) { _textFont = font; }

void COptionLineCtl::OnPaint(HDC hDC) {
  if (_visible) {
    UINT nTextFormat = DT_LEFT;
    DUI_Rect rc(_location, _size);

    int nWidth = 0;
    if (_title.size() > 0) {
      nWidth = GetTextWidth(_title.c_str(), _textFont) + 10;
      DUI_Rect rcText;
      CopyRect(&rcText, &rc);
      rcText.right = rcText.left + nWidth;
      if (_transparent) {
        CGDIResource::GetInstance().GDI_DrawText_Trans(
            hDC, _title.c_str(), &rcText, _textFont, _textCol, DT_LEFT);
      } else {
        CGDIResource::GetInstance().GDI_DrawText(hDC, _title.c_str(), &rcText,
                                                 _textFont, _textCol, DT_LEFT);
      }
    }

    int nMiddle = rc.top + rc.Height() / 2;
    int nLeft = rc.left + nWidth;
    if (_transparent) {
      CGDIResource::GetInstance().GDI_DrawLine_trans(
          hDC, nLeft, nMiddle, rc.right, nMiddle, _lineCol);
    } else {
      CGDIResource::GetInstance().GDI_DrawLine(hDC, nLeft, nMiddle, rc.right,
                                               nMiddle, _lineCol);
    }
  }
}
