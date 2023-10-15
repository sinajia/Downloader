#pragma once

#include "DUIPanel.h"
#include "DUITab.h"

class CTabListCtl : public CPanelCtl {
public:
  CTabListCtl(HWND father, BOOL trans = FALSE);
  ~CTabListCtl(void);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void AddPage(const tstring &strTabID, CBaseElementCtl *pElement);
  void SelTab(const tstring &strID);
  void UnSelTab(void);

protected:
  std::map<tstring, CBaseElementCtl *> _mapTabPage;
};
