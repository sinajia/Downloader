#pragma once

#include "DUIButton.h"

class CHyperLinkCtl : public CButtonCtl {
public:
  CHyperLinkCtl(HWND father, BOOL trans = FALSE);
  CHyperLinkCtl(HWND father, LPCTSTR lpszID, LPCTSTR lpszTitle,
                LPCTSTR lpszTooptip, DUI_Rect rcElement);
  ~CHyperLinkCtl(void);

  void CreateElement(XmlElementPtr element);
  void OnPaint(HDC hDC);
  void SetURL(tstring strURL);
  void SetAlignRight();
  void SetLinkColor(COLORREF colLink);
  void SetHoverColor(COLORREF colHover);
  void SetIcon(LPCTSTR lpszIconRes);

  virtual void OnMouseHover(UINT nFlags, CPoint point);
  virtual void OnMouseLeave();

  virtual void OnCall_LeftUp();

protected:
  tstring m_strURL;
  COLORREF _linkColor;
  COLORREF _visitedColour;
  COLORREF _hoverColor;
  UINT _textFormat;
  HBITMAP _bmpIcon;
  HFONT _textFont;
  HFONT _textUnderLineFont;

  void GotoURL();
};

inline void CHyperLinkCtl::SetURL(tstring strURL) { m_strURL = strURL; }
