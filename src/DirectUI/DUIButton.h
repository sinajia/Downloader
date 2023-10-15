#pragma once

#include "DUIDef.h"
#include "DUIElement.h"

class CButtonCtl : public CBaseElementCtl {
public:
  CButtonCtl(HWND father, BOOL trans = FALSE);
  CButtonCtl(HWND hWnd, LPCTSTR lpszID, LPCTSTR lpszTitle, LPCTSTR lpszTooptip,
             DUI_Rect rcElement, BOOL trans = FALSE);
  ~CButtonCtl(void);

  void Disable();
  void Enable(BOOL enable);
  void CreateElement(XmlElementPtr element);
  void SetImage(LPCTSTR lpszImageRes);
  void SetImage(HBITMAP bmp);
  void SetIcon(LPCTSTR lpszImageRes);
  virtual void SetIcon(HBITMAP hIcon);
  void SetSmallIcon(LPCTSTR lpszImageRes);
  void SetSmallIconFile(LPCTSTR lpszImageFile);
  void SetImageFile(LPCTSTR lpszImageFile);
  void SetVertical();
  void SetTitle(const tstring &title);
  void SetColor(COLORREF colText, COLORREF colHover, COLORREF colDown);
  void SetTextLimitLength(int nLength);
  void SetCorner(DUI_Rect rcCorners);
  void OnCall_LeftUp();
  void OnCall_RightUp();
  void SetTextFont(HFONT font) { _textFont = font; }

protected:
  BOOL _tracking;
  BOOL _down;
  DUI_BUTTON_STATE _state;
  HBITMAP _bmpBtn;
  HBITMAP _bmpIcon;
  HBITMAP _bmpSmallIcon;
  COLORREF _colText;
  COLORREF _colHover;
  COLORREF _colDown;
  COLORREF _colDisable;
  BOOL _vertical_flag;
  int _nTextLimitLength;
  DUI_Rect _rcCorners;
  HFONT _textFont;
  int _hdelta;

public:
  void Refresh(DUI_BUTTON_STATE nowstate);
  virtual void OnPaint(HDC hDC);
  virtual void OnMouseHover(UINT nFlags, CPoint point);
  virtual void OnMouseLeave();
  virtual void OnMouseDown(UINT nFlags, CPoint point);
  virtual void OnMouseUp(UINT nFlags, CPoint point);
  virtual void OnMouseDownClick();
};
