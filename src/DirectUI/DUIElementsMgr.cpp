#include "StdAfx.h"
#include "DUIPanel.h"
#include "DUIElementsMgr.h"

using namespace std;

CAnyElementsMgr::CAnyElementsMgr(void) {
  _last_mouseon_element = NULL;
  _owner = NULL;
  _rearrange_element = NULL;
}

CAnyElementsMgr::~CAnyElementsMgr(void) {}

void CAnyElementsMgr::AddElement(CBaseElementCtl *element) {
  _controls.push_back(element);
}

void CAnyElementsMgr::AddElement2(CBaseElementCtl *element) {
  if (_controls.size() == 0 || _controls.size() == 1)
    _controls.push_back(element);
  else {
    list<CBaseElementCtl *>::iterator Iter;
    Iter = _controls.begin();
    Iter++;
    _controls.insert(Iter, element);
  }
}

void CAnyElementsMgr::OnKeyDown(DWORD dwFlag) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    element->OnKeyDown(dwFlag);
  }
}

void CAnyElementsMgr::OnKeyUp(DWORD dwFlag) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    element->OnKeyUp(dwFlag);
  }
}

void CAnyElementsMgr::OnMove() {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if (element->GetVisible() && element->GetHandleMouse()) {
      element->OnMove();
    }
  }
}

BOOL CAnyElementsMgr::MouseOnElement(CPoint pt) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if ((element->GetVisible() && element->GetHandleMouse()) &&
        PointInRect(pt, element->Rectangle())) {
      return TRUE;
    }
  }
  return FALSE;
}

void CAnyElementsMgr::OnMouseClick(CPoint pt) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if (element->GetVisible() && element->GetHandleMouse()) {
      PointInRect(CPoint(pt.x, pt.y), element->Rectangle());
    }
  }
}

void CAnyElementsMgr::OnMouseDown(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if ((element->GetVisible() && element->GetHandleMouse()) &&
        PointInRect(pt, element->Rectangle())) {
      element->OnMouseDown(nFlags, pt);
    }
  }

  if (_rearrange_element != NULL) {
    list<CBaseElementCtl *>::iterator ite =
        find(_controls.begin(), _controls.end(), _rearrange_element);
    _controls.erase(ite);
    _controls.push_back(_rearrange_element);
    _rearrange_element = NULL;
  }
}

void CAnyElementsMgr::OnMouseLDbClick(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;
  CBaseElementCtl *cur_element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    cur_element = *ite;
    if (cur_element->GetVisible() && cur_element->GetHandleMouse()) {
      if (PointInRect(pt, cur_element->Rectangle())) {
        cur_element->OnMouseLDbClick(nFlags, pt);
      }
    }
  }
}

void CAnyElementsMgr::OnMouseUp(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;
  CBaseElementCtl *cur_element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (; ite != _controls.end(); ite++) {
    cur_element = *ite;
    if (cur_element->GetVisible() && cur_element->GetHandleMouse()) {
      if (PointInRect(pt, cur_element->Rectangle())) {
        cur_element->OnMouseUp(nFlags, pt);
        if (!cur_element->GetWndClassName().compare(_T("DirectButton"))) {
          break;
        }
      }
    }
  }
}

void CAnyElementsMgr::OnMouseRUp(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;
  CBaseElementCtl *cur_element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    cur_element = *ite;
    if (cur_element->GetVisible() && cur_element->GetHandleMouse()) {
      if (PointInRect(pt, cur_element->Rectangle())) {
        cur_element->OnMouseRUp(nFlags, pt);
      }
    }
  }
}

void CAnyElementsMgr::OnMouseLeave() {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if ((element->GetVisible() && element->GetHandleMouse()) &&
        (_last_mouseon_element == element)) {
      element->OnMouseLeave();
    }
  }
}

void CAnyElementsMgr::OnMouseMove(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;
  CBaseElementCtl *element2 = NULL;
  CBaseElementCtl *element3 = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element3 = *ite;
    if (element3->GetVisible() && element3->GetHandleMouse()) {
      if (PointInRect(pt, element3->Rectangle())) {
        element = element3;
      } else if (_last_mouseon_element == element3) {
        element2 = element3;
      }
    }
  }
  if (((element != NULL) && (_last_mouseon_element != element)) &&
      (element2 == NULL)) {
    element2 = _last_mouseon_element;
  }
  if (element2 != NULL) {
    element2->OnMouseLeave();
    ::SendMessage(_owner, WM_MSG_TIPS, FALSE, 0);
    ::SendMessage(_owner, WM_MSG_CURSOR, (WPARAM)element2->GetCursor(), 0);
  }

  if ((element != NULL)) {
    if (!lstrcmp(element->GetWndClassName().c_str(), _T("Panel"))) {
      element->OnMouseHover(nFlags, pt);
    } else {
      element->OnMouseHover(nFlags, pt);
      if (_last_mouseon_element != element) {
        ::SendMessage(_owner, WM_MSG_TIPS, TRUE,
                      (LPARAM)element->GetTooltip().c_str());
        ::SendMessage(_owner, WM_MSG_CURSOR, (WPARAM)element->GetCursor(), 0);
      }
    }
  }
  _last_mouseon_element = element;
}

void CAnyElementsMgr::DrawCursor() {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if ((element->GetVisible() && element->GetHandleKeyboard())) {
      element->DrawCursor();
    }
  }
}

void CAnyElementsMgr::SetActive(CBaseElementCtl *element) {
  CBaseElementCtl *ele = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    ele = *ite;
    ele->SetActive(element);
  }
}

void CAnyElementsMgr::OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam,
                            BOOL &bHandled) {
  CBaseElementCtl *ele = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    ele = *ite;
    ele->OnMsg(nMsg, wParam, lParam, bHandled);
  }
}

void CAnyElementsMgr::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
  CBaseElementCtl *element = NULL;
  CBaseElementCtl *cur_element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    cur_element = *ite;
    if (cur_element->GetVisible() && cur_element->GetHandleMouse()) {
      if (PointInRect(pt, cur_element->Rectangle())) {
        cur_element->OnMouseWheel(nFlags, zDelta, pt);
      }
    }
  }
}

void CAnyElementsMgr::OnPaint(HDC hDC) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if (element->GetVisible()) {
      element->OnPaint(hDC);
    }
  }
}

void CAnyElementsMgr::OnSizeChanged(DUI_Size wndChangingSize, int w, int h) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    element->OnSizeChanged(wndChangingSize, w, h);
  }
}

void CAnyElementsMgr::RearrangeElement(CBaseElementCtl *e) {
  if (e != NULL) {
    list<CBaseElementCtl *>::iterator ite =
        find(_controls.begin(), _controls.end(), e);
    _controls.erase(ite);
    _controls.push_back(e);
  }
}

CBaseElementCtl *CAnyElementsMgr::Search(const tstring id) {
  CBaseElementCtl *filterEle = NULL;
  CBaseElementCtl *element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;

    if (!lstrcmp(element->GetWndClassName().c_str(), _T("Panel"))) {
      filterEle = ((CPanelCtl *)element)->_elementmgr.Search(id);
      if (filterEle) {
        break;
      }
    }
    if (element->GetID() == id) {
      filterEle = element;
      break;
    }
  }

  return filterEle;
}

void CAnyElementsMgr::Reset() { _cur_pos = _controls.begin(); }

CBaseElementCtl *CAnyElementsMgr::GetElement() {
  CBaseElementCtl *element = NULL;
  while (_cur_pos != _controls.end()) {
    element = *_cur_pos;
    _cur_pos++;
    return element;
  }
  return NULL;
}

void CAnyElementsMgr::Dispose() {

  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (; ite != _controls.end(); ++ite) {
    element = *ite;
    if (element) {
      delete element;
    }
  }
  _controls.clear();

  _last_mouseon_element = NULL;
  _rearrange_element = NULL;
}

void CAnyElementsMgr::Dispose(const tstring id) {
  CBaseElementCtl *element = NULL;
  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end();) {
    element = *ite;

    if (element->GetID() == id) {
      delete element;
      _controls.erase(ite++);
    } else {
      ite++;
    }
  }
  _last_mouseon_element = NULL;
}

void CAnyElementsMgr::OnMouseDblDown(UINT nFlags, CPoint pt) {
  CBaseElementCtl *element = NULL;

  list<CBaseElementCtl *>::iterator ite = _controls.begin();
  for (ite; ite != _controls.end(); ite++) {
    element = *ite;
    if ((element->GetVisible() && element->GetHandleMouse()) &&
        PointInRect(pt, element->Rectangle())) {
      element->OnMouseDblDown(nFlags, pt);
    }
  }
}

list<CBaseElementCtl *> *CAnyElementsMgr::GetAllElement() { return &_controls; }
