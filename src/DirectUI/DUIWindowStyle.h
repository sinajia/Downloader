#pragma once

#include <vector>
#include "DUIDef.h"

class __declspec(novtable) CWindowCtlStyle {
  using string = std::string;
public:
  CWindowCtlStyle();
  virtual ~CWindowCtlStyle();

public:
  virtual void LoadStyle(XmlElementPtr element);
  tstring GetStringAttribute(XmlElementPtr element, string attrName);
  HBITMAP GetBitmapAttribute(XmlElementPtr element, string attrName,
                             string key_mid_string = "");
  int GetIntegerAttribute(XmlElementPtr element, string attrName);
  COLORREF GetColorAttribute(XmlElementPtr element, string attrName);
  COLORREF GetRGBAttribute(XmlElementPtr element, string attrName);
  DUI_Rect GetRectAttribute(XmlElementPtr element, string attrName);
  DUI_Size StyleSizeToWindowSize(DUI_Size &eleSize, DUI_Size &wndChangingSize,
                                 DUI_Size &wndChangedSize, DUI_STRETCH xFlag,
                                 DUI_STRETCH yFlag);
  DUI_Point StyleLocationToWindowLocation(DUI_Rect &rc, DUI_Point oriPt,
                                          int alignX, int alignY,
                                          DUI_Size &windowSize);
  DUI_Rect StyleRectToWindowRect(DUI_Rect rc, DUI_Point oriPt,
                                 DUI_Size &wndChangingSize,
                                 DUI_Size &wndChangedSize, DUI_ALIGNMENT alignX,
                                 DUI_ALIGNMENT alignY, DUI_STRETCH stretchHor,
                                 DUI_STRETCH stretchVer);
  DUI_ALIGNMENT AlignmentStringToValue(tstring s, DUI_ALIGNMENT align);
  DUI_STRETCH StretchStringToValue(tstring s, DUI_STRETCH stretch);
  HCURSOR NameToCursor(tstring name);
  std::vector<tstring> split(const tstring src, tstring delimit,
                        tstring null_subst = _T(""));

public:
  tstring _NameID;

  DUI_ALIGNMENT _xAlignment;
  DUI_ALIGNMENT _yAlignment;
  DUI_STRETCH _xStretch;
  DUI_STRETCH _yStretch;
};
