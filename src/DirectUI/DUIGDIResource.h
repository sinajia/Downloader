#pragma once

#include <gdiplus.h>
#include "util/util_tools.h"
#include "DUIDef.h"

using namespace Gdiplus;

class CGDIResource {
public:
  CGDIResource();
  virtual ~CGDIResource();

  static CGDIResource &GetInstance() {
    static CGDIResource This;
    return This;
  }

  void Initialize(LPCTSTR lpszResPath);
  void UnInitialize();

  void Dispose();

  virtual void GDI_DrawImageStretch(HDC hDC, HBITMAP hBitmap, CRect rcDest,
                                    CRect rcBmp, BYTE uFade, CRect rcCorners);

  virtual void GDI_DrawImage(HDC hDC, Bitmap *pBmp, int nLeft, int nTop,
                             int nWidth, int nHeight, BOOL bTransparent = TRUE);
  virtual void GDI_DrawImage(HDC hDC, HBITMAP hBitmap, int nLeft, int nTop,
                             int nWidth, int nHeight,
                             DUI_BITMAP_TYPE bmpType = DUI_BT_ALPHA,
                             DWORD dwtrans = 0xFF);
  virtual void GDI_DrawImage(HDC hDC, HBITMAP hBitmap, LPRECT rcDest,
                             DUI_BITMAP_TYPE bmpType = DUI_BT_ALPHA,
                             DWORD dwtrans = 0xFF);
  virtual void GDI_DrawImage(HDC hDC, HBITMAP hBitmap, DUI_Rect &rcDest,
                             DUI_Rect &rcSrc,
                             DUI_BITMAP_TYPE bmpType = DUI_BT_ALPHA);

  virtual void GDI_DrawIcon(HDC hDC, HICON hIcon, int nLeft, int nTop);
  virtual void GDI_DrawIconEx(HDC hDC, HICON hIcon, int nLeft, int nTop,
                              int nWidth, int nHeight);
  virtual void GDI_DrawLine(HDC hDC, int nBegX, int nBegY, int nEndX, int nEndY,
                            COLORREF colLine = RGB(0, 0, 0));
  virtual void GDI_DrawDotLine(HDC hDC, int nBegX, int nBegY, int nEndX,
                               int nEndY, COLORREF colLine = RGB(0, 0, 0));

  virtual void GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                            HFONT hFont, COLORREF colText = RGB(0, 0, 0),
                            UINT nFormat = DT_LEFT);
  virtual void GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                            COLORREF colText, UINT nFormat = DT_LEFT);
  virtual void GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                            UINT nFormat = DT_LEFT);

  virtual void GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                  HFONT hFont, COLORREF colText = RGB(0, 0, 0),
                                  UINT nFormat = DT_LEFT);
  virtual void GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                  COLORREF colText, UINT nFormat = DT_LEFT);
  virtual void GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                  UINT nFormat = DT_LEFT);

  virtual void GDI_FillRectBack(HDC hDC, LPRECT rc, COLORREF colBrush);

  virtual void GDI_DrawRectangle(HDC hDC, LPRECT rc,
                                 COLORREF colBorder = RGB(0, 0, 0),
                                 COLORREF colBrush = RGB(255, 255, 255));
  virtual void GDI_DrawRectangle_Trans(HDC hDC, LPRECT rc,
                                       COLORREF colBorder = RGB(0, 0, 0),
                                       COLORREF colBrush = RGB(255, 255, 255));
  virtual void GDI_DrawRoundRectangle(HDC hDC, LPRECT rc,
                                      COLORREF colBorder = RGB(0, 0, 0),
                                      COLORREF colBrush = RGB(255, 255, 255));
  virtual void GDI_DrawRoundRectangle_Trans(HDC hDC, LPRECT rc,
                                            COLORREF colBorder = RGB(0, 0, 0),
                                            COLORREF colBrush = RGB(255, 255,
                                                                    255));

  virtual HBITMAP GDI_GetBitmap(HINSTANCE hInstance, LPCTSTR pszResName,
                                int nResType);
  virtual HBITMAP GDI_GetBitmap(HINSTANCE hInstance, LPCTSTR pszResName,
                                LPCTSTR lpszResType);
  virtual HBITMAP GDI_GetBitmap(LPCTSTR lpszFileName);
  virtual HBITMAP GDI_GetBitmap_NoCache(HINSTANCE hInstance, LPCTSTR pszResName,
                                        int nResType);
  virtual HBITMAP GDI_GetBitmap_NoCache(LPCTSTR lpszFileName);

  virtual HFONT GDI_GetNormalFont();
  virtual HFONT GDI_GetNormalBoldFont();
  virtual HFONT GDI_GetBiggerBoldFont();
  virtual HFONT GDI_GetBiggerFont();
  virtual HFONT GDI_GetSmallFont();
  virtual HFONT GDI_GetUnderlineFont();

  tstring GDI_GetIconPath(LPCTSTR lpszIconFileName);

  virtual HICON GDI_GetIcon(LPCTSTR lpszIconFileName);
  virtual HICON GDI_GetIcon(LPCTSTR lpszIconFileName, int cx, int cy);

  virtual HICON GDI_GetIcon(UINT nResourceID);

  virtual void GDI_DrawLine_trans(HDC hDC, int nBegX, int nBegY, int nEndX,
                                  int nEndY, COLORREF colLine = RGB(0, 0, 0));

protected:
  GraphicsPath *MakeRoundRect(LPRECT rc, INT percentageRounded);

protected:
  tstring m_strResPath;
  static HFONT m_hNormalFont;
  static HFONT m_hNormalBoldFont;
  static HFONT m_hBiggerBoldFont;
  static HFONT m_hSmallFont;
  static HFONT m_BiggerFont;

  static HFONT m_hUnderLineFont;
  static ULONG_PTR m_Token;
  static std::map<tstring, HBITMAP> m_mapAllRes;
  static int m_nRef;
  static CriticalSection m_cs;
};
