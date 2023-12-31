#pragma once

#include "../DirectUI/DUITransWindow.h"

class CUIUpDate : public CTransWindow {
public:
  enum Type {
    QUIT = 0,
    CHECKSYSTEM,
    NOTICE = CHECKSYSTEM,
  };
  CUIUpDate();
  virtual ~CUIUpDate();

  BEGIN_MSG_MAP(CUIUpDate)
  CHAIN_MSG_MAP(CTransWindow)
  END_MSG_MAP()

public:
  virtual void DrawBorder(CDCHandle dc);

  void CreateWnd(HWND hWnd);

  void DestroyWnd();

  virtual void OnLoad();
  virtual void OnLoaded();

  virtual void OnExit();

  void SetCustomTitle(std::wstring &title);

  virtual void OnFinalMessage(HWND);

public:
  void OnYes();
  void OnCancal();

public:
  Type infostyle_;
};
