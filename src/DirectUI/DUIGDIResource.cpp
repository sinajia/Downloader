#include "stdafx.h"
#include "Util/UtilApi.h"
#include "DUIGDIResource.h"

using namespace std;

static constexpr LPCTSTR glpszFontName[] = { _T("Tahoma"), _T("Microsoft Yahei") };

HFONT CGDIResource::m_hNormalFont = NULL;
HFONT CGDIResource::m_hNormalBoldFont = NULL;
HFONT CGDIResource::m_hBiggerBoldFont = NULL;
HFONT CGDIResource::m_hSmallFont = NULL;
HFONT CGDIResource::m_BiggerFont = NULL;
HFONT CGDIResource::m_hUnderLineFont = NULL;
ULONG_PTR CGDIResource::m_Token = 0;
map<tstring, HBITMAP> CGDIResource::m_mapAllRes;
int CGDIResource::m_nRef = 0;
CriticalSection CGDIResource::m_cs;

CGDIResource::CGDIResource() {}

CGDIResource::~CGDIResource() {}

void CGDIResource::Initialize(LPCTSTR lpszResPath) {
  m_strResPath = lpszResPath;
  m_cs.Lock();
  if (m_nRef == 0) {

    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&m_Token, &input, NULL);
  }
  m_nRef = m_nRef + 1;
  m_cs.UnLock();
}
void CGDIResource::UnInitialize() {
  m_cs.Lock();
  if (m_nRef > 0)
    m_nRef = m_nRef - 1;
  if (m_nRef == 0) {
    Dispose();

    Gdiplus::GdiplusShutdown(m_Token);
  }
  m_cs.UnLock();
}

void CGDIResource::Dispose() {
  HBITMAP hBitmap = NULL;
  map<tstring, HBITMAP>::iterator ite = m_mapAllRes.begin();
  for (ite; ite != m_mapAllRes.end(); ite++) {
    hBitmap = ite->second;
    if (hBitmap) {
      ::DeleteObject(hBitmap);
    }
  }
  m_mapAllRes.clear();

  if (m_hNormalFont) {
    ::DeleteObject(m_hNormalFont);
    m_hNormalFont = NULL;
  }
  if (m_hNormalBoldFont) {
    ::DeleteObject(m_hNormalBoldFont);
    m_hNormalBoldFont = NULL;
  }
  if (m_hBiggerBoldFont) {
    ::DeleteObject(m_hBiggerBoldFont);
    m_hBiggerBoldFont = NULL;
  }
  if (m_hSmallFont) {
    ::DeleteObject(m_hSmallFont);
    m_hSmallFont = NULL;
  }
  if (m_hUnderLineFont) {
    ::DeleteObject(m_hUnderLineFont);
    m_hUnderLineFont = NULL;
  }
}

void CGDIResource::GDI_DrawImageStretch(HDC hDC, HBITMAP hBitmap, CRect rcDest,
                                        CRect rcBmp, BYTE uFade,
                                        CRect rcCorners) {
  HDC hdcMem = CreateCompatibleDC(hDC);
  HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

  ATLASSERT(rcBmp.Width() > rcCorners.left + rcCorners.right);
  ATLASSERT(rcBmp.Height() > rcCorners.top + rcCorners.bottom);

  BLENDFUNCTION bf = {AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA};

  AlphaBlend(hDC, rcDest.left, rcDest.top, rcCorners.left, rcCorners.top,
             hdcMem, rcBmp.left, rcBmp.top, rcCorners.left, rcCorners.top, bf);

  AlphaBlend(hDC, rcDest.left + rcCorners.left, rcDest.top,
             rcDest.Width() - rcCorners.left - rcCorners.right, rcCorners.top,
             hdcMem, rcBmp.left + rcCorners.left, rcBmp.top,
             rcBmp.Width() - rcCorners.left - rcCorners.right, rcCorners.top,
             bf);

  AlphaBlend(hDC, rcDest.right - rcCorners.right, rcDest.top, rcCorners.right,
             rcCorners.top, hdcMem, rcBmp.right - rcCorners.right, rcBmp.top,
             rcCorners.right, rcCorners.top, bf);

  AlphaBlend(hDC, rcDest.left, rcDest.top + rcCorners.top, rcCorners.left,
             rcDest.Height() - rcCorners.top - rcCorners.bottom, hdcMem,
             rcBmp.left, rcBmp.top + rcCorners.top, rcCorners.left,
             rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);

  AlphaBlend(hDC, rcDest.left + rcCorners.left, rcDest.top + rcCorners.top,
             rcDest.Width() - rcCorners.left - rcCorners.right,
             rcDest.Height() - rcCorners.top - rcCorners.bottom, hdcMem,
             rcBmp.left + rcCorners.left, rcBmp.top + rcCorners.top,
             rcBmp.Width() - rcCorners.left - rcCorners.right,
             rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);

  AlphaBlend(
      hDC, rcDest.right - rcCorners.right, rcDest.top + rcCorners.top,
      rcCorners.right, rcDest.Height() - rcCorners.top - rcCorners.bottom,
      hdcMem, rcBmp.right - rcCorners.right, rcBmp.top + rcCorners.top,
      rcCorners.right, rcBmp.Height() - rcCorners.top - rcCorners.bottom, bf);

  AlphaBlend(hDC, rcDest.left, rcDest.bottom - rcCorners.bottom, rcCorners.left,
             rcCorners.bottom, hdcMem, rcBmp.left,
             rcBmp.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom,
             bf);

  AlphaBlend(
      hDC, rcDest.left + rcCorners.left, rcDest.bottom - rcCorners.bottom,
      rcDest.Width() - rcCorners.left - rcCorners.right, rcCorners.bottom,
      hdcMem, rcBmp.left + rcCorners.left, rcBmp.bottom - rcCorners.bottom,
      rcBmp.Width() - rcCorners.left - rcCorners.right, rcCorners.bottom, bf);

  AlphaBlend(
      hDC, rcDest.right - rcCorners.right, rcDest.bottom - rcCorners.bottom,
      rcCorners.right, rcCorners.bottom, hdcMem, rcBmp.right - rcCorners.right,
      rcBmp.bottom - rcCorners.bottom, rcCorners.right, rcCorners.bottom, bf);

  SelectObject(hdcMem, hBmpOld);
  DeleteDC(hdcMem);
}

void CGDIResource::GDI_DrawImage(HDC hDC, Bitmap *pBmp, int nLeft, int nTop,
                                 int nWidth, int nHeight, BOOL bTransparent) {
  Graphics g(hDC);
  if (bTransparent) {
    ImageAttributes imageAtt;
    imageAtt.SetColorKey(Color(255, 0, 255), Color(255, 0, 255));

    Rect rcDst(nLeft, nTop, nWidth, nHeight);
    g.DrawImage(pBmp, rcDst, 0, 0, pBmp->GetWidth(), pBmp->GetHeight(),
                UnitPixel, &imageAtt);
  } else {
    TextureBrush brush(pBmp, WrapModeTile,
                       Rect(0, 0, pBmp->GetWidth(), pBmp->GetHeight()));
    brush.TranslateTransform((REAL)nLeft, (REAL)nTop);
    g.FillRectangle(&brush, nLeft, nTop, nWidth, nHeight);
  }
}

void CGDIResource::GDI_DrawImage(HDC hDC, HBITMAP hBitmap, int nLeft, int nTop,
                                 int nWidth, int nHeight,
                                 DUI_BITMAP_TYPE bmpType, DWORD trans) {
  HDC hdcMem = CreateCompatibleDC(hDC);
  HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

  BITMAP bmInfor;
  GetObject(hBitmap, sizeof(bmInfor), &bmInfor);

  if (bmpType != DUI_BT_ALPHA) {

    TransparentBlt(hDC, nLeft, nTop, nWidth, nHeight, hdcMem, 0, 0,
                   bmInfor.bmWidth, bmInfor.bmHeight, TRANSPARENT_COLOR);
  } else {

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    if (trans < 0 || trans > 0xFF)
      trans = 0xFF;
    bf.SourceConstantAlpha = static_cast<BYTE>(trans);
    bf.AlphaFormat = AC_SRC_ALPHA;
    AlphaBlend(hDC, nLeft, nTop, nWidth, nHeight, hdcMem, 0, 0, bmInfor.bmWidth,
               bmInfor.bmHeight, bf);
  }
  SelectObject(hdcMem, hBmpOld);
  DeleteDC(hdcMem);
}

void CGDIResource::GDI_DrawImage(HDC hDC, HBITMAP hBitmap, LPRECT rcDest,
                                 DUI_BITMAP_TYPE bmpType, DWORD trans) {
  GDI_DrawImage(hDC, hBitmap, rcDest->left, rcDest->top,
                rcDest->right - rcDest->left, rcDest->bottom - rcDest->top,
                bmpType, trans);
}

void CGDIResource::GDI_DrawImage(HDC hDC, HBITMAP hBitmap, DUI_Rect &rcDest,
                                 DUI_Rect &rcSrc, DUI_BITMAP_TYPE bmpType) {
  HDC hdcMem = CreateCompatibleDC(hDC);
  HBITMAP hBmpOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

  if (bmpType != DUI_BT_ALPHA) {

    TransparentBlt(hDC, rcDest.left, rcDest.top, rcDest.Width(),
                   rcDest.Height(), hdcMem, rcSrc.left, rcSrc.top,
                   rcSrc.Width(), rcSrc.Height(), TRANSPARENT_COLOR);

  } else {

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0xFF;
    bf.AlphaFormat = AC_SRC_ALPHA;
    AlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
               hdcMem, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),
               bf);
  }
  SelectObject(hdcMem, hBmpOld);
  DeleteDC(hdcMem);
}

void CGDIResource::GDI_DrawIcon(HDC hDC, HICON hIcon, int nLeft, int nTop) {
  ::DrawIcon(hDC, nLeft, nTop, hIcon);
}

void CGDIResource::GDI_DrawIconEx(HDC hDC, HICON hIcon, int nLeft, int nTop,
                                  int nWidth, int nHeight) {
  ::DrawIconEx(hDC, nLeft, nTop, hIcon, nWidth, nHeight, 0, NULL,
               DI_IMAGE | DI_MASK);
}

void CGDIResource::GDI_DrawLine(HDC hDC, int nBegX, int nBegY, int nEndX,
                                int nEndY, COLORREF colLine) {
  HPEN hPen = CreatePen(PS_SOLID, 0, colLine);
  HPEN hPenOld = (HPEN)SelectObject(hDC, hPen);

  MoveToEx(hDC, nBegX, nBegY, (LPPOINT)NULL);
  LineTo(hDC, nEndX, nEndY);
  SelectObject(hDC, hPenOld);
  DeleteObject(hPen);
}

void CGDIResource::GDI_DrawLine_trans(HDC hDC, int nBegX, int nBegY, int nEndX,
                                      int nEndY, COLORREF colLine) {
  Graphics g(hDC);
  Color col(Color::MakeARGB(254, GetRValue(colLine), GetGValue(colLine),
                            GetBValue(colLine)));
  Pen p(col, 1.0);
  g.DrawLine(&p, nBegX, nBegY, nEndX, nEndY);
}

void CGDIResource::GDI_DrawDotLine(HDC hDC, int nBegX, int nBegY, int nEndX,
                                   int nEndY, COLORREF colLine) {
  HPEN hPen = CreatePen(PS_DOT, 0, colLine);
  HPEN hPenOld = (HPEN)SelectObject(hDC, hPen);

  MoveToEx(hDC, nBegX, nBegY, (LPPOINT)NULL);
  LineTo(hDC, nEndX, nEndY);
  SelectObject(hDC, hPenOld);
  DeleteObject(hPen);
}

void CGDIResource::GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                HFONT hFont, COLORREF colText, UINT nFormat) {
  CDCHandle dc(hDC);
  dc.SetBkMode(TRANSPARENT);
  HFONT hOldFont = dc.SelectFont(hFont);
  dc.SetTextColor(colText);
  dc.DrawText(strText, lstrlen(strText), rcText, nFormat);
  dc.SelectFont(hOldFont);
}

void CGDIResource::GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                UINT nFormat) {
  GDI_DrawText(hDC, strText, rcText, GDI_GetNormalFont(), RGB(0, 0, 0),
               nFormat);
}

void CGDIResource::GDI_DrawText(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                COLORREF colText, UINT nFormat) {
  GDI_DrawText(hDC, strText, rcText, GDI_GetNormalFont(), colText, nFormat);
}

void CGDIResource::GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                      HFONT hFont, COLORREF colText,
                                      UINT nFormat) {
  Graphics g(hDC);
  Color col(Color::MakeARGB(254, GetRValue(colText), GetGValue(colText),
                            GetBValue(colText)));
  SolidBrush brush(col);
  Gdiplus::Font font(hDC, hFont);
  RectF rf(rcText->left, rcText->top, rcText->right - rcText->left,
           rcText->bottom - rcText->top);
  StringFormat sf;
  if (nFormat & DT_LEFT) {
    sf.SetAlignment(StringAlignmentNear);
  } else if (nFormat & DT_CENTER) {
    sf.SetAlignment(StringAlignmentCenter);
  } else if (nFormat & DT_RIGHT) {
    sf.SetAlignment(StringAlignmentFar);
  }
  if (nFormat & DT_VCENTER) {
    sf.SetLineAlignment(StringAlignmentCenter);
  } else {
    sf.SetLineAlignment(StringAlignmentNear);
  }

  if (nFormat & DT_SINGLELINE) {
    sf.SetFormatFlags(StringFormatFlagsNoWrap);
  }
  if (nFormat & DT_END_ELLIPSIS) {
    sf.SetTrimming(StringTrimmingEllipsisCharacter);
  }

  g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  g.DrawString(strText, -1, &font, rf, &sf, &brush);
}

void CGDIResource::GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                      COLORREF colText, UINT nFormat) {
  GDI_DrawText_Trans(hDC, strText, rcText, GDI_GetNormalFont(), colText,
                     nFormat);
}

void CGDIResource::GDI_DrawText_Trans(HDC hDC, LPCTSTR strText, LPRECT rcText,
                                      UINT nFormat) {
  GDI_DrawText_Trans(hDC, strText, rcText, GDI_GetNormalFont(), RGB(0, 0, 0),
                     nFormat);
}

void CGDIResource::GDI_FillRectBack(HDC hDC, LPRECT rc, COLORREF colBrush) {
  CDCHandle dc(hDC);
  HBRUSH hBrush = CreateSolidBrush(colBrush);
  HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
  dc.FillRect(rc, hBrush);
  ::SelectObject(hDC, hOldBrush);
  DeleteObject(hBrush);
}

void CGDIResource::GDI_DrawRectangle(HDC hDC, LPRECT rc, COLORREF colBorder,
                                     COLORREF colBrush) {
  HPEN hPen = CreatePen(PS_SOLID, 1, colBorder);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
  HBRUSH hBrush = CreateSolidBrush(colBrush);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Rectangle(hDC, rc->left, rc->top, rc->right, rc->bottom);
  SelectObject(hDC, hOldPen);
  SelectObject(hDC, hOldBrush);
  DeleteObject(hPen);
  DeleteObject(hBrush);
}

void CGDIResource::GDI_DrawRectangle_Trans(HDC hDC, LPRECT rc,
                                           COLORREF colBorder,
                                           COLORREF colBrush) {
  Graphics g(hDC);
  Color colPen(Color::MakeARGB(254, GetRValue(colBorder), GetGValue(colBorder),
                               GetBValue(colBorder)));
  Color colBru(Color::MakeARGB(254, GetRValue(colBrush), GetGValue(colBrush),
                               GetBValue(colBrush)));
  SolidBrush brush(colBru);
  Pen pen(colPen);
  g.FillRectangle(&brush, rc->left, rc->top, rc->right - rc->left,
                  rc->bottom - rc->top);
  g.DrawRectangle(&pen, rc->left, rc->top, rc->right - rc->left,
                  rc->bottom - rc->top);
  g.ReleaseHDC(hDC);
}

void CGDIResource::GDI_DrawRoundRectangle(HDC hDC, LPRECT rc,
                                          COLORREF colBorder,
                                          COLORREF colBrush) {
  HPEN hPen = CreatePen(PS_SOLID, 1, colBorder);
  HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
  HBRUSH hBrush = CreateSolidBrush(colBrush);
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  RoundRect(hDC, rc->left, rc->top, rc->right, rc->bottom, 3, 3);
  SelectObject(hDC, hOldPen);
  SelectObject(hDC, hOldBrush);
  DeleteObject(hPen);
  DeleteObject(hBrush);
}

void CGDIResource::GDI_DrawRoundRectangle_Trans(HDC hDC, LPRECT rc,
                                                COLORREF colBorder,
                                                COLORREF colBrush) {
  Graphics g(hDC);
  Color colPen(Color::MakeARGB(254, GetRValue(colBorder), GetGValue(colBorder),
                               GetBValue(colBorder)));
  Color colBru(Color::MakeARGB(254, GetRValue(colBrush), GetGValue(colBrush),
                               GetBValue(colBrush)));
  SolidBrush brush(colBru);
  Pen pen(colPen);

  GraphicsPath *path = MakeRoundRect(rc, 6);
  g.FillPath(&brush, path);
  g.DrawPath(&pen, path);

  delete path;
}

HBITMAP CGDIResource::GDI_GetBitmap(HINSTANCE hInstance, LPCTSTR pszResName,
                                    int nResType) {

  map<tstring, HBITMAP>::iterator ite = m_mapAllRes.find(pszResName);
  if (ite != m_mapAllRes.end()) {
    return (HBITMAP)(ite->second);
  }

  HGLOBAL hGlobalReal = NULL;
  HRSRC hRes = FindResource(hInstance, pszResName, MAKEINTRESOURCE(nResType));
  if (hRes) {
    HGLOBAL hGlobal = LoadResource(hInstance, hRes);
    if (hGlobal) {
      DWORD dwSize = SizeofResource(hInstance, hRes);
      hGlobalReal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);

      LPVOID pDest = ::GlobalLock(hGlobalReal);
      LPVOID pSrc = ::LockResource(hGlobal);
      ::CopyMemory(pDest, pSrc, dwSize);
      ::FreeResource(hRes);
    }
  }

  Gdiplus::Bitmap *pBitmap = NULL;
  if (hGlobalReal) {
    CComPtr<IStream> pStm;

    HRESULT hr = CreateStreamOnHGlobal(hGlobalReal, TRUE, &pStm);
    if (hr == S_OK && pStm)
      pBitmap = new Gdiplus::Bitmap(pStm, FALSE);

    GlobalUnlock(hGlobalReal);
    GlobalFree(hGlobalReal);
    hGlobalReal = NULL;
  }

  if (pBitmap == NULL)
    return NULL;

  HBITMAP hBitmap = NULL;
  if (pBitmap->GetLastStatus() == Gdiplus::Ok)
    pBitmap->GetHBITMAP(NULL, &hBitmap);

  delete pBitmap;

  m_mapAllRes[pszResName] = hBitmap;

  return hBitmap;
}

HBITMAP CGDIResource::GDI_GetBitmap_NoCache(HINSTANCE hInstance,
                                            LPCTSTR pszResName, int nResType) {
  HGLOBAL hGlobalReal = NULL;
  HRSRC hRes = FindResource(hInstance, pszResName, MAKEINTRESOURCE(nResType));
  if (hRes) {
    HGLOBAL hGlobal = LoadResource(hInstance, hRes);
    if (hGlobal) {
      DWORD dwSize = SizeofResource(hInstance, hRes);
      hGlobalReal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);

      LPVOID pDest = ::GlobalLock(hGlobalReal);
      LPVOID pSrc = ::LockResource(hGlobal);
      ::CopyMemory(pDest, pSrc, dwSize);
      ::FreeResource(hRes);
    }
  }

  Gdiplus::Bitmap *pBitmap = NULL;
  if (hGlobalReal) {
    CComPtr<IStream> pStm;

    HRESULT hr = CreateStreamOnHGlobal(hGlobalReal, TRUE, &pStm);
    if (hr == S_OK && pStm)
      pBitmap = new Gdiplus::Bitmap(pStm, FALSE);

    GlobalUnlock(hGlobalReal);
    GlobalFree(hGlobalReal);
    hGlobalReal = NULL;
  }

  if (pBitmap == NULL)
    return NULL;

  HBITMAP hBitmap = NULL;
  if (pBitmap->GetLastStatus() == Gdiplus::Ok)
    pBitmap->GetHBITMAP(NULL, &hBitmap);

  delete pBitmap;

  return hBitmap;
}

HBITMAP CGDIResource::GDI_GetBitmap_NoCache(LPCTSTR lpszFileName) {
  if (lstrlen(lpszFileName) == 0) {
    return NULL;
  }

  HBITMAP hBitmap = NULL;

  Gdiplus::Bitmap *pBitmap = Gdiplus::Bitmap::FromFile(lpszFileName);
  if (pBitmap->GetLastStatus() == Gdiplus::Ok)
    pBitmap->GetHBITMAP(NULL, &hBitmap);
  delete pBitmap;

  return hBitmap;
}

HBITMAP CGDIResource::GDI_GetBitmap(HINSTANCE hInstance, LPCTSTR pszResName,
                                    LPCTSTR lpszResType) {
  map<tstring, HBITMAP>::iterator ite = m_mapAllRes.find(pszResName);
  if (ite != m_mapAllRes.end()) {
    return (HBITMAP)(ite->second);
  }

  HGLOBAL hGlobalReal = NULL;
  HRSRC hRes = FindResource(hInstance, pszResName, lpszResType);
  if (hRes) {
    HGLOBAL hGlobal = LoadResource(hInstance, hRes);
    if (hGlobal) {
      DWORD dwSize = SizeofResource(hInstance, hRes);
      hGlobalReal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);

      LPVOID pDest = ::GlobalLock(hGlobalReal);
      LPVOID pSrc = ::LockResource(hGlobal);
      ::CopyMemory(pDest, pSrc, dwSize);
      ::FreeResource(hRes);
    }
  }

  Gdiplus::Bitmap *pBitmap = NULL;
  if (hGlobalReal) {
    CComPtr<IStream> pStm;

    HRESULT hr = CreateStreamOnHGlobal(hGlobalReal, TRUE, &pStm);
    if (hr == S_OK && pStm)
      pBitmap = new Gdiplus::Bitmap(pStm, FALSE);

    GlobalUnlock(hGlobalReal);
    GlobalFree(hGlobalReal);
    hGlobalReal = NULL;
  }

  if (pBitmap == NULL)
    return NULL;

  HBITMAP hBitmap = NULL;
  if (pBitmap->GetLastStatus() == Gdiplus::Ok)
    pBitmap->GetHBITMAP(NULL, &hBitmap);

  delete pBitmap;

  m_mapAllRes[pszResName] = hBitmap;

  return hBitmap;
}

HBITMAP CGDIResource::GDI_GetBitmap(LPCTSTR lpszFileName) {
  if (lstrlen(lpszFileName) == 0) {
    return NULL;
  }

  map<tstring, HBITMAP>::iterator ite = m_mapAllRes.find(lpszFileName);
  if (ite != m_mapAllRes.end()) {
    return (HBITMAP)(ite->second);
  }

  HBITMAP hBitmap = NULL;

  Gdiplus::Bitmap *pBitmap = Gdiplus::Bitmap::FromFile(lpszFileName);
  if (pBitmap->GetLastStatus() == Gdiplus::Ok)
    pBitmap->GetHBITMAP(NULL, &hBitmap);

  delete pBitmap;

  m_mapAllRes[lpszFileName] = hBitmap;

  return hBitmap;
}

HFONT CGDIResource::GDI_GetNormalFont() {
  if (m_hNormalFont)
    return m_hNormalFont;

  LANGID lanId = GetSystemDefaultLangID();

  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {13, 18};

  if (0x804 == lanId) {
    m_hNormalFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, glpszFontName[nIndex]);
  } else {
    m_hNormalFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, _T("Arial"));
  }
  return m_hNormalFont;
}

HFONT CGDIResource::GDI_GetNormalBoldFont() {
  if (m_hNormalBoldFont)
    return m_hNormalBoldFont;

  LANGID lanId = GetSystemDefaultLangID();

  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {13, 18};

  if (0x804 == lanId) {
    m_hNormalBoldFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0,
                                   0, 0, 0, 0, 0, glpszFontName[nIndex]);
  } else {
    m_hNormalBoldFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0,
                                   0, 0, 0, 0, 0, L"Arial");
  }
  return m_hNormalBoldFont;
}

HFONT CGDIResource::GDI_GetBiggerFont() {
  if (m_BiggerFont)
    return m_BiggerFont;

  LANGID lanId = GetSystemDefaultLangID();
  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {16, 21};

  if (0x804 == lanId) {
    m_BiggerFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, glpszFontName[nIndex]);
  } else {
    m_BiggerFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, L"Arial");
  }
  return m_BiggerFont;
}

HFONT CGDIResource::GDI_GetBiggerBoldFont() {
  if (m_hBiggerBoldFont) {
    return m_hBiggerBoldFont;
  }

  LANGID lanId = GetSystemDefaultLangID();

  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {16, 20};

  if (0x804 == lanId) {
    m_hBiggerBoldFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0,
                                   0, 0, 0, 0, 0, glpszFontName[nIndex]);
  } else {
    m_hBiggerBoldFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0,
                                   0, 0, 0, 0, 0, L"Arial");
  }
  return m_hBiggerBoldFont;
}

HFONT CGDIResource::GDI_GetSmallFont() {
  if (m_hSmallFont)
    return m_hSmallFont;

  LANGID lanId = GetSystemDefaultLangID();
  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {12, 14};

  if (0x804 == lanId) {
    m_hSmallFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0, 0,
                              0, 0, 0, 0, glpszFontName[nIndex]);
  } else {
    m_hSmallFont = CreateFont(nFontSize[nIndex], 0, 0, 0, FW_BOLD, 0, 0, 0, 0,
                              0, 0, 0, 0, L"Arial");
  }
  return m_hSmallFont;
}

HFONT CGDIResource::GDI_GetUnderlineFont() {
  if (m_hUnderLineFont)
    return m_hUnderLineFont;

  LANGID lanId = GetSystemDefaultLangID();
  int nIndex = Util::Tools::IsWin7() ? 1 : 0;
  int nFontSize[] = {13, 18};

  if (0x804 == lanId) {
    m_hUnderLineFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, TRUE, 0, 0,
                                  0, 0, 0, 0, glpszFontName[nIndex]);
  } else {
    m_hUnderLineFont = CreateFont(nFontSize[nIndex], 0, 0, 0, 0, 0, TRUE, 0, 0,
                                  0, 0, 0, 0, L"Arial");
  }
  return m_hUnderLineFont;
}

tstring CGDIResource::GDI_GetIconPath(LPCTSTR lpszIconFileName) {
  TCHAR szPath[MAX_PATH] = {0};
  _sntprintf_s(szPath, MAX_PATH, _TRUNCATE, _T("%s"), m_strResPath.c_str());
  PathAppend(szPath, lpszIconFileName);

  return szPath;
}

HICON CGDIResource::GDI_GetIcon(LPCTSTR lpszIconFileName) {
  return (HICON)LoadImage(NULL, lpszIconFileName, IMAGE_ICON, 0, 0,
                          LR_LOADFROMFILE);
}

HICON CGDIResource::GDI_GetIcon(LPCTSTR lpszIconFileName, int cx, int cy) {
  return (HICON)LoadImage(NULL, lpszIconFileName, IMAGE_ICON, cx, cy,
                          LR_LOADFROMFILE);
}

HICON CGDIResource::GDI_GetIcon(UINT nResourceID) {
  return (HICON)LoadImage(_hInstance, MAKEINTRESOURCE(nResourceID), IMAGE_ICON,
                          0, 0, LR_LOADFROMFILE);
}

GraphicsPath *CGDIResource::MakeRoundRect(LPRECT rc, INT percentageRounded) {
  INT left = rc->left;
  INT right = rc->right;

  INT top = rc->top;
  INT bottom = rc->bottom;

  INT offsetX = (right - left) * percentageRounded / 100;
  INT offsetY = (bottom - top) * percentageRounded / 100;

  GraphicsPath pt;
  GraphicsPath *path = pt.Clone();

  path->AddArc(right - offsetX, top, offsetX, offsetY, 270, 90);
  path->AddArc(right - offsetX, bottom - offsetY, offsetX, offsetY, 0, 90);
  path->AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);
  path->AddArc(left, top, offsetX, offsetY, 180, 90);
  path->AddLine(left + offsetX, top, right - offsetX / 2, top);
  return path;
}
