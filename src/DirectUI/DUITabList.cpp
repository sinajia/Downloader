#include "StdAfx.h"
#include "DUITabList.h"

using namespace std;

CTabListCtl::CTabListCtl(HWND father, BOOL trans) : CPanelCtl(father) {
  _host = father;
  _class = _T("");
  _transparent = trans;
}

CTabListCtl::~CTabListCtl(void) {}

void CTabListCtl::CreateElement(XmlElementPtr element) {
  GetElementStyle(element);

  CXmlHelper helper;
  vector<XmlElementPtr> pAllElement = helper.GetChild(element);
  vector<XmlElementPtr>::iterator ite = pAllElement.begin();
  tstring strEleName;
  CBaseElementCtl *pElement = NULL;
  for (ite; ite != pAllElement.end(); ite++) {

    strEleName = helper.GetElementName(*ite);

    if (!strEleName.compare(_T("tab"))) {
      pElement = new CTabCtl(_host, this, _transparent);
      pElement->CreateElement(*ite);
      _elementmgr.AddElement(pElement);
    }
  }
}

void CTabListCtl::AddPage(const tstring &strTabID, CBaseElementCtl *pElement) {
  if (strTabID.empty())
    return;

  _mapTabPage[strTabID] = pElement;
}

void CTabListCtl::SelTab(const tstring &strTabID) {
  CTabCtl *pTab = NULL;
  ResetElement();
  while ((pTab = (CTabCtl *)GetElement()) != NULL) {
    if (pTab->GetID() == strTabID) {
      pTab->SetSel(TRUE);
    } else {
      pTab->SetSel(FALSE);
    }
  }

  map<tstring, CBaseElementCtl *>::iterator ite = _mapTabPage.begin();
  for (ite; ite != _mapTabPage.end(); ite++) {
    if (ite->first == strTabID) {
      ite->second->SetVisible(TRUE);
    } else {
      ite->second->SetVisible(FALSE);
    }
  }
}

void CTabListCtl::UnSelTab(void) {
  CTabCtl *pTab = NULL;
  ResetElement();
  while ((pTab = (CTabCtl *)GetElement()) != NULL) {
    pTab->SetSel(FALSE);
  }

  map<tstring, CBaseElementCtl *>::iterator ite = _mapTabPage.begin();
  for (ite; ite != _mapTabPage.end(); ite++) {
    ite->second->SetVisible(FALSE);
  }
}

void CTabListCtl::OnPaint(HDC hDC) {
  if (_visible) {
    _elementmgr.OnPaint(hDC);
  }
}
