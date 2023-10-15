#include "stdafx.h"
#include "DUIWindowStyle.h"
#include "DUIGDIResource.h"

CWindowCtlStyle::CWindowCtlStyle()
    : _xAlignment(DUI_ALIGNMENT_NONE), _yAlignment(DUI_ALIGNMENT_NONE),
      _xStretch(DUI_STRETCH_NONE), _yStretch(DUI_STRETCH_NONE) {}

CWindowCtlStyle::~CWindowCtlStyle() {}

void CWindowCtlStyle::LoadStyle(XmlElementPtr element) {
  CXmlHelper helper;
  _xAlignment = AlignmentStringToValue(
      helper.GetAttributeString(element, "xAlignment").c_str(),
      DUI_ALIGNMENT_LEFT);
  _yAlignment = AlignmentStringToValue(
      helper.GetAttributeString(element, "yAlignment").c_str(),
      DUI_ALIGNMENT_TOP);
  _xStretch = StretchStringToValue(
      helper.GetAttributeString(element, "xStretch").c_str(), DUI_STRETCH_NONE);
  _yStretch = StretchStringToValue(
      helper.GetAttributeString(element, "yStretch").c_str(), DUI_STRETCH_NONE);
}

tstring CWindowCtlStyle::GetStringAttribute(XmlElementPtr element,
                                            string attrName) {
  CXmlHelper helper;
  return helper.GetAttributeString(element, attrName.c_str());
}

HBITMAP CWindowCtlStyle::GetBitmapAttribute(XmlElementPtr element,
                                            string attrName,
                                            string key_mid_string) {
  HBITMAP hBitmap = NULL;
  CXmlHelper helper;
  tstring attrValue = helper.GetAttributeString(element, attrName);
  if (attrValue != _T("")) {
    hBitmap = CGDIResource::GetInstance().GDI_GetBitmap(_hInstance,
                                                        attrValue.c_str(), PNG);
  }

  return hBitmap;
}

int CWindowCtlStyle::GetIntegerAttribute(XmlElementPtr element,
                                         string attrName) {
  CXmlHelper helper;
  return helper.GetAttributeInt(element, attrName);
}

COLORREF CWindowCtlStyle::GetColorAttribute(XmlElementPtr element,
                                            string attrName) {
  COLORREF color_ret;
  tstring s_value = GetStringAttribute(element, attrName);
  if (!s_value.empty()) {
    color_ret = _tcstol(s_value.c_str(), NULL, 16);
  }
  return color_ret;
}

std::vector<tstring> CWindowCtlStyle::split(const tstring src, tstring delimit,
                                       tstring null_subst) {
  std::vector<tstring> v;
  int npos = -1;
  if (src.empty() || delimit.empty())
    return v;

  int deli_len = delimit.size();
  long index = npos, last_search_position = 0;
  while ((index = src.find(delimit, last_search_position)) != npos) {
    if (index == last_search_position)
      v.push_back(null_subst);
    else
      v.push_back(
          src.substr(last_search_position, index - last_search_position));
    last_search_position = index + deli_len;
  }
  tstring last_one = src.substr(last_search_position);
  v.push_back(last_one.empty() ? null_subst : last_one);
  return v;
}

COLORREF CWindowCtlStyle::GetRGBAttribute(XmlElementPtr element,
                                          string attrName) {
  COLORREF colour_ret = 0;
  tstring s_value = GetStringAttribute(element, attrName);
  if (!s_value.empty()) {
    tstring src_value = s_value.c_str();
    std::vector<tstring> v = split(src_value, _T(","));
    if (v.size() == 3) {
      int r = _ttoi(v[0].c_str());
      int g = _ttoi(v[1].c_str());
      int b = _ttoi(v[2].c_str());
      colour_ret = RGB(r, g, b);
    }
  }
  return colour_ret;
}

DUI_Rect CWindowCtlStyle::GetRectAttribute(XmlElementPtr element,
                                           string attrName) {
  DUI_Rect rect_ret;
  tstring s_value = GetStringAttribute(element, attrName);
  if (!s_value.empty()) {
    tstring src_value = s_value.c_str();
    std::vector<tstring> v = split(src_value, _T(","));
    if (v.size() == 4) {
      rect_ret.left = _ttoi(v[0].c_str());
      rect_ret.top = _ttoi(v[1].c_str());
      rect_ret.right = _ttoi(v[2].c_str());
      rect_ret.bottom = _ttoi(v[3].c_str());
    }
  }
  return rect_ret;
}

DUI_Size CWindowCtlStyle::StyleSizeToWindowSize(DUI_Size &eleSize,
                                                DUI_Size &wndChangingSize,
                                                DUI_Size &wndChangedSize,
                                                DUI_STRETCH xFlag,
                                                DUI_STRETCH yFlag) {
  DUI_Size newSize(eleSize.cx, eleSize.cy);
  switch (xFlag) {
  case DUI_STRETCH_FILL:
    newSize.cx = eleSize.cx + wndChangedSize.cx - wndChangingSize.cx;
    break;
  case DUI_STRETCH_SCALE:
    newSize.cx = (int)((double)(eleSize.cx * wndChangedSize.cx) /
                       (double)wndChangingSize.cx);
    break;
  default:
    break;
  }

  switch (yFlag) {
  case DUI_STRETCH_FILL:
    newSize.cy = eleSize.cy + wndChangedSize.cy - wndChangingSize.cy;
    break;
  case DUI_STRETCH_SCALE:
    newSize.cy = (int)((double)(eleSize.cy * wndChangedSize.cy) /
                       (double)wndChangingSize.cy);
    break;
  default:
    break;
  }

  return newSize;
}

DUI_Point CWindowCtlStyle::StyleLocationToWindowLocation(DUI_Rect &rc,
                                                         DUI_Point fixPt,
                                                         int alignX, int alignY,
                                                         DUI_Size &windowSize) {
  DUI_Point point(rc.left, rc.top);
  DUI_Size size(rc.Width(), rc.Height());

  if (alignX == DUI_ALIGNMENT_LEFT)
    point.x = point.x;
  else if (alignX == DUI_ALIGNMENT_RIGHT)
    point.x = windowSize.cx - fixPt.x;
  if (alignY == DUI_ALIGNMENT_TOP)
    point.y = point.y;
  else if (alignY == DUI_ALIGNMENT_BOTTOM)
    point.y = windowSize.cy - fixPt.y;
  if (alignX == DUI_ALIGNMENT_CENTER)
    point.x = (windowSize.cx - size.cx) / 2;
  if (alignY == DUI_ALIGNMENT_CENTER)
    point.y = (windowSize.cy - size.cy) / 2;

  return DUI_Point(point.x, point.y);
}

DUI_Rect CWindowCtlStyle::StyleRectToWindowRect(
    DUI_Rect rc, DUI_Point oriPt, DUI_Size &wndChangingSize,
    DUI_Size &wndChangedSize, DUI_ALIGNMENT alignX, DUI_ALIGNMENT alignY,
    DUI_STRETCH stretchHor, DUI_STRETCH stretchVer) {

  DUI_Point newPos =
      StyleLocationToWindowLocation(rc, oriPt, alignX, alignY, wndChangedSize);

  SIZE newSize =
      StyleSizeToWindowSize(DUI_Size(rc.Width(), rc.Height()), wndChangingSize,
                            wndChangedSize, stretchHor, stretchVer);

  return DUI_Rect(newPos, newSize);
}

DUI_STRETCH CWindowCtlStyle::StretchStringToValue(tstring s,
                                                  DUI_STRETCH stretch) {
  if (s == _T("none")) {
    return DUI_STRETCH_NONE;
  } else if (s == _T("fill")) {
    return DUI_STRETCH_FILL;
  } else if (s == _T("scale")) {
    return DUI_STRETCH_SCALE;
  } else {
    return DUI_STRETCH_NONE;
  }

  return stretch;
}
DUI_ALIGNMENT CWindowCtlStyle::AlignmentStringToValue(tstring s,
                                                      DUI_ALIGNMENT align) {
  if (s.empty()) {
    return align;
  }
  std::map<tstring, DUI_ALIGNMENT> dict1;
  dict1[_T("left")] = DUI_ALIGNMENT_LEFT;
  dict1[_T("top")] = DUI_ALIGNMENT_TOP;
  dict1[_T("right")] = DUI_ALIGNMENT_RIGHT;
  dict1[_T("bottom")] = DUI_ALIGNMENT_BOTTOM;
  dict1[_T("center")] = DUI_ALIGNMENT_CENTER;

  if (dict1.find(s) != dict1.end()) {
    return dict1[s];
  }
  return align;
}

HCURSOR CWindowCtlStyle::NameToCursor(tstring name) {
  HCURSOR default_cursor = LoadCursor(NULL, IDC_ARROW);

  std::map<tstring, HCURSOR> dict1;
  dict1[_T("AppStarting")] = LoadCursor(NULL, IDC_APPSTARTING);
  dict1[_T("Arrow")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("Cross")] = LoadCursor(NULL, IDC_CROSS);
  dict1[_T("Default")] = LoadCursor(NULL, IDC_ARROW);

  dict1[_T("Help")] = LoadCursor(NULL, IDC_HELP);
  dict1[_T("HSplit")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("IBeam")] = LoadCursor(NULL, IDC_IBEAM);
  dict1[_T("No")] = LoadCursor(NULL, IDC_NO);
  dict1[_T("NoMove2D")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("NoMoveHoriz")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("NoMoveVert")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanEast")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanNE")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanNorth")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanNW")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanSE")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanSouth")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanSW")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("PanWest")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("SizeAll")] = LoadCursor(NULL, IDC_SIZEALL);
  dict1[_T("SizeNESW")] = LoadCursor(NULL, IDC_SIZENESW);
  dict1[_T("SizeNS")] = LoadCursor(NULL, IDC_SIZENS);
  dict1[_T("SizeNWSE")] = LoadCursor(NULL, IDC_SIZENWSE);
  dict1[_T("SizeWE")] = LoadCursor(NULL, IDC_SIZEWE);
  dict1[_T("UpArrow")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("VSplit")] = LoadCursor(NULL, IDC_ARROW);
  dict1[_T("WaitCursor")] = LoadCursor(NULL, IDC_WAIT);
  tstring key = name.c_str();
  if (dict1.find(key) != dict1.end()) {
    return HCURSOR(dict1[key]);
  }
  return default_cursor;
}
