#pragma once

typedef DWORD HLSCOLOR;
#define HLS(h, l, s)                                                           \
  ((HLSCOLOR)(((BYTE)(h) | ((WORD)((BYTE)(l)) << 8)) |                         \
              (((DWORD)(BYTE)(s)) << 16)))

#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls) >> 16))

HLSCOLOR RGB2HLS(COLORREF rgb);
COLORREF HLS2RGB(HLSCOLOR hls);

COLORREF HLS_TRANSFORM(COLORREF rgb, int percent_L, int percent_S);

class CBufferDC : public CDCHandle {
  HDC m_hDestDC;
  CBitmap m_bitmap;
  CRect m_rect;
  HGDIOBJ m_hOldBitmap;

public:
  CBufferDC(HDC hDestDC, const CRect &rcPaint = CRect(0, 0, 0, 0));
  ~CBufferDC();
};

class CPenDC {
protected:
  CPen m_pen;
  HDC m_hDC;
  HPEN m_hOldPen;

public:
  CPenDC(HDC hDC, COLORREF crColor = CLR_NONE);
  ~CPenDC();

  void Color(COLORREF crColor);
  COLORREF Color() const;
};

class CBrushDC {
protected:
  CBrush m_brush;
  HDC m_hDC;
  HBRUSH m_hOldBrush;

public:
  CBrushDC(HDC hDC, COLORREF crColor = CLR_NONE);
  ~CBrushDC();

  void Color(COLORREF crColor);
  COLORREF Color() const;
};

class CBoldDC {
protected:
  CFont m_fontBold;
  HDC m_hDC;
  HFONT m_hDefFont;

public:
  CBoldDC(HDC hDC, bool bBold);
  ~CBoldDC();
};
