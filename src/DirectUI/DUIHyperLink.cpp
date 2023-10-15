#include "StdAfx.h"
#include "DUIHyperLink.h"

CHyperLinkCtl::CHyperLinkCtl(HWND father, BOOL trans) : CButtonCtl(father) {
  _host = father;
  _transparent = trans;
  _class = _T("");
  _textFormat = DT_LEFT;
  _bmpIcon = NULL;
  _textFont = CGDIResource::GetInstance().GDI_GetNormalFont();
  _textUnderLineFont = CGDIResource::GetInstance().GDI_GetUnderlineFont();
}

CHyperLinkCtl::CHyperLinkCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                             LPCTSTR lpszTooptip, DUI_Rect rcElement)
    : CButtonCtl(father, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _host = father;
  _class = _T("");
  _linkColor = RGB(54, 151, 242);
  _visitedColour = RGB(54, 151, 242);
  _hoverColor = RGB(54, 151, 242);
  _textFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
  _bmpIcon = NULL;
  _textFont = CGDIResource::GetInstance().GDI_GetNormalFont();
  _textUnderLineFont = CGDIResource::GetInstance().GDI_GetUnderlineFont();
}

CHyperLinkCtl::~CHyperLinkCtl(void) {}

void CHyperLinkCtl::CreateElement(XmlElementPtr element) {

  GetElementStyle(element);

  _linkColor = GetRGBAttribute(element, "linkcolor");
  _visitedColour = GetRGBAttribute(element, "visitedcolor");
  _hoverColor = GetRGBAttribute(element, "hovercolor");
}

void CHyperLinkCtl::SetLinkColor(COLORREF colLink) { _linkColor = colLink; }

void CHyperLinkCtl::SetHoverColor(COLORREF colHover) { _hoverColor = colHover; }

void CHyperLinkCtl::SetIcon(LPCTSTR lpszIconRes) {
  _bmpIcon =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszIconRes, PNG);
}

void CHyperLinkCtl::OnPaint(HDC hDC) {
  if (_visible) {
    HFONT textFont = _textFont;

    DUI_Rect rcLink(_location, _size);

    COLORREF color = RGB(0, 0, 0);
    switch (_state) {
    case DUI_STATE_NORMAL:
      color = _linkColor;
      break;
    case DUI_STATE_DOWN: {
      textFont = _textUnderLineFont;
      color = _visitedColour;
    } break;
    case DUI_STATE_HOVER: {
      textFont = _textUnderLineFont;
      color = _hoverColor;
    } break;
    case DUI_STATE_DISABLE:
      color = RGB(153, 153, 153);
      break;
    }

    if (_bmpIcon) {
      BITMAP bmpIconInfor;
      GetObject(_bmpIcon, sizeof(bmpIconInfor), &bmpIconInfor);

      int nY = _location.y + (_size.cy - bmpIconInfor.bmHeight) / 2;

      CGDIResource::GetInstance().GDI_DrawImage(hDC, _bmpIcon, _location.x, nY,
                                                bmpIconInfor.bmWidth,
                                                bmpIconInfor.bmHeight);

      rcLink.left = _location.x + bmpIconInfor.bmWidth + 2;
    }

    if (_transparent) {
      CGDIResource::GetInstance().GDI_DrawText_Trans(
          hDC, _title.c_str(), &rcLink, textFont, color, _textFormat);
    } else {
      CGDIResource::GetInstance().GDI_DrawText(hDC, _title.c_str(), &rcLink,
                                               textFont, color, _textFormat);
    }
  }
}
void CHyperLinkCtl::OnMouseHover(UINT nFlags, CPoint point) {
  if (_state != DUI_STATE_DISABLE) {
    CButtonCtl::OnMouseHover(nFlags, point);
    _cursor = LoadCursor(NULL, IDC_HAND);
  }
}

void CHyperLinkCtl::OnMouseLeave() {
  CButtonCtl::OnMouseLeave();

  _cursor = LoadCursor(NULL, IDC_ARROW);
}

void CHyperLinkCtl::OnCall_LeftUp() {
  _leftup_event.Invoke();
  _leftup_event.Invoke(m_strURL.c_str());
}

void CHyperLinkCtl::GotoURL() {

  HINSTANCE result =
      ShellExecute(NULL, _T("open"), m_strURL.c_str(), NULL, NULL, SW_SHOW);

  if ((UINT)result <= HINSTANCE_ERROR) {
  }
}

void CHyperLinkCtl::SetAlignRight() {
  _textFormat = DT_RIGHT;

  int nTextWidth = GetTextWidth(
      _title.c_str(), CGDIResource::GetInstance().GDI_GetNormalFont());
  _location.x = Rectangle().right - nTextWidth;
  if (_size.cx > nTextWidth) {
    _size.cx = nTextWidth;
  }
}
