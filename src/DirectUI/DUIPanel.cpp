#include "StdAfx.h"
#include "DUIButton.h"
#include "DUICheckBox.h"
#include "DUIHyperLink.h"
#include "DUILayeredEdit.h"
#include "DUIProgress.h"
#include "DUIRadioButton.h"
#include "DUIStatic.h"
#include "DUITabList.h"
#include "DUIPanel.h"

using namespace std;

CPanelCtl::CPanelCtl(HWND father, BOOL trans) {
  _host = father;
  _handleKeyboard = TRUE;
  _class = _T("Panel");
  _transparent = trans;
  _colBack = RGB(255, 255, 255);
  _colBorder = RGB(255, 255, 255);
  _bmpBackground = NULL;
  _elementmgr.InitOwner(_host);
  _draw_border = FALSE;
}

CPanelCtl::CPanelCtl(HWND hWnd, LPCTSTR lpszID, LPCTSTR lpszTitle,
                     LPCTSTR lpszTooptip, DUI_Rect rcElement)
    : CBaseElementCtl(hWnd, lpszID, lpszTitle, lpszTooptip, rcElement) {
  _class = _T("Panel");
  _host = hWnd;
  _handleKeyboard = TRUE;
  _colBack = RGB(255, 255, 255);
  _colBorder = RGB(255, 255, 255);
  _bmpBackground = NULL;
  _elementmgr.InitOwner(_host);
  _draw_border = FALSE;
}

CPanelCtl::~CPanelCtl(void) {
  CBaseElementCtl *pEle = NULL;
  _elementmgr.Reset();
  while ((pEle = _elementmgr.GetElement()) != NULL) {
    delete pEle;
    pEle = NULL;
  }
}

void CPanelCtl::CreateElement(XmlElementPtr element) {
  GetElementStyle(element);

  _bmpBackground = GetBitmapAttribute(element, "background");
  _colBack = GetRGBAttribute(element, "backcolor");
  _colBorder = GetRGBAttribute(element, "bordercolor");
  _draw_border = GetIntegerAttribute(element, "border") == 1 ? TRUE : FALSE;

  CXmlHelper helper;
  vector<XmlElementPtr> pAllElement = helper.GetChild(element);
  vector<XmlElementPtr>::iterator ite = pAllElement.begin();
  tstring strEleName;
  CBaseElementCtl *pElement = NULL;
  for (ite; ite != pAllElement.end(); ite++) {
    strEleName = helper.GetElementName(*ite);
    if (!strEleName.compare(_T("panel"))) {
      pElement = new CPanelCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("button"))) {
      pElement = new CButtonCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("tablist"))) {
      pElement = new CTabListCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("static"))) {
      pElement = new CStaticCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("link"))) {
      pElement = new CHyperLinkCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("progress"))) {
      pElement = new CProgressCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("checkbox"))) {
      pElement = new CCheckBoxCtl(_host, _transparent);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }
    if (!strEleName.compare(_T("radiobutton"))) {
      pElement = new CRadioButtonCtl(_host);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }

    if (!strEleName.compare(_T("layedit"))) {
      pElement = new CLayeredEditCtl(_host);
      pElement->CreateElement(*ite);
      AddElement(pElement);
      continue;
    }
  }
}

void CPanelCtl::SetBackImage(LPCTSTR lpszImageRes) {
  _bmpBackground =
      CGDIResource::GetInstance().GDI_GetBitmap(_hInstance, lpszImageRes, PNG);
}

void CPanelCtl::AddElement(CBaseElementCtl *element) {
  DUI_Rect rcElement = element->Rectangle();

  element->SetPos(
      DUI_Point(_location.x + rcElement.left, _location.y + rcElement.top),
      DUI_Size(rcElement.Width(), rcElement.Height()));
  _elementmgr.AddElement(element);
}

void CPanelCtl::AddElement2(CBaseElementCtl *element) {
  DUI_Rect rcElement = element->Rectangle();

  element->SetPos(
      DUI_Point(_location.x + rcElement.left, _location.y + rcElement.top),
      DUI_Size(rcElement.Width(), rcElement.Height()));
  _elementmgr.AddElement2(element);
}

void CPanelCtl::OnMove() { _elementmgr.OnMove(); }

void CPanelCtl::OnMouseDown(UINT nFlags, CPoint pt) {
  _elementmgr.OnMouseDown(nFlags, pt);
}

void CPanelCtl::OnKeyDown(DWORD dwFlag) { _elementmgr.OnKeyDown(dwFlag); }

void CPanelCtl::OnKeyUp(DWORD dwFlag) { _elementmgr.OnKeyUp(dwFlag); }

void CPanelCtl::OnMouseDblDown(UINT nFlags, CPoint pt) {
  _elementmgr.OnMouseDblDown(nFlags, pt);
}
void CPanelCtl::OnMouseUp(UINT nFlags, CPoint pt) {
  _elementmgr.OnMouseUp(nFlags, pt);
}

void CPanelCtl::OnMouseRUp(UINT nFlags, CPoint pt) {
  _elementmgr.OnMouseRUp(nFlags, pt);
}

void CPanelCtl::OnMouseLeave() { _elementmgr.OnMouseLeave(); }

void CPanelCtl::OnMouseHover(UINT nFlags, CPoint pt) {
  _elementmgr.OnMouseMove(nFlags, pt);
}

void CPanelCtl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
  _elementmgr.OnMouseWheel(nFlags, zDelta, pt);
}

void CPanelCtl::OnPaint(HDC hDC) {
  if (_visible) {
    if (!_transparent) {
      DUI_Rect rc(_location, _size);

      CGDIResource::GetInstance().GDI_DrawRectangle(hDC, &rc, _colBorder,
                                                    _colBack);

      if (_bmpBackground) {
        BITMAP bmpBackInfor;
        GetObject(_bmpBackground, sizeof(bmpBackInfor), &bmpBackInfor);

        DUI_Rect rcSrc;
        rcSrc.left = 0;
        rcSrc.top = 0;
        rcSrc.right = bmpBackInfor.bmWidth;
        rcSrc.bottom = bmpBackInfor.bmHeight;
        CGDIResource::GetInstance().GDI_DrawImageStretch(
            hDC, _bmpBackground, rc, rcSrc, 255, CRect(10, 10, 10, 10));
      }
    } else {
      DUI_Rect rc(_location, _size);
      if (_draw_border) {

        CGDIResource::GetInstance().GDI_DrawRectangle_Trans(
            hDC, &rc, _colBorder, _colBack);
      }
    }
    _elementmgr.OnPaint(hDC);
  }
}

void CPanelCtl::OnSizeChanged(DUI_Size wndChangingSize, int w, int h) {
  _elementmgr.Reset();
  CBaseElementCtl *element = NULL;
  while ((element = _elementmgr.GetElement()) != NULL) {
    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(rcElement.left - _location.x, rcElement.top - _location.y),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }

  DUI_Size szOldPanel = _size;

  CBaseElementCtl::OnSizeChanged(wndChangingSize, w, h);

  DUI_Size szNewPanel = _size;

  _elementmgr.Reset();
  while ((element = _elementmgr.GetElement()) != NULL) {
    element->OnSizeChanged(szOldPanel, szNewPanel.cx, szNewPanel.cy);

    DUI_Rect rcElement = element->Rectangle();

    element->SetPos(
        DUI_Point(_location.x + rcElement.left, _location.y + rcElement.top),
        DUI_Size(rcElement.Width(), rcElement.Height()));
  }
}

void CPanelCtl::SetVisible(BOOL visible) {
  if (_visible != visible) {
    _visible = visible;

    CBaseElementCtl *pEle = NULL;
    _elementmgr.Reset();
    while ((pEle = _elementmgr.GetElement()) != NULL) {
      pEle->SetVisible(_visible);
    }

    RaiseInvalidate(TRUE);
  }
}

void CPanelCtl::Invlidate() {
  if (_visible) {

    RaiseInvalidate(TRUE);

    CBaseElementCtl *pEle = NULL;
    _elementmgr.Reset();
    while ((pEle = _elementmgr.GetElement()) != NULL) {
      pEle->RaiseInvalidate(TRUE);
    }
  }
}

void CPanelCtl::Dispose() { _elementmgr.Dispose(); }

void CPanelCtl::Dispose(LPCTSTR lpszID) { _elementmgr.Dispose(lpszID); }

CBaseElementCtl *CPanelCtl::GetElement() { return _elementmgr.GetElement(); }

void CPanelCtl::ResetElement() { _elementmgr.Reset(); }

void CPanelCtl::DrawCursor() { _elementmgr.DrawCursor(); }

void CPanelCtl::SetActive(CBaseElementCtl *element) {
  _elementmgr.SetActive(element);
}

void CPanelCtl::OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) {
  _elementmgr.OnMsg(nMsg, wParam, lParam, bHandled);
}
