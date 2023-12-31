#pragma once

#include "../DirectUI/DUITransWindow.h"

class CUIContents : public CTransWindow {
public:
  CUIContents();
  virtual ~CUIContents();

  BEGIN_MSG_MAP(CUIContents)
  MESSAGE_HANDLER(WM_TIMER, OnTimer)
  CHAIN_MSG_MAP(CTransWindow)
  END_MSG_MAP()

  void OnWheel(WPARAM wParam);
  LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) override;

  virtual void OnFinalMessage(HWND) override;

  void CreateWnd(HWND hWnd);

  virtual void OnLoad();
  virtual void OnLoaded();

  HWND _parent;

  int _scrollDelta;
};

class CUIAgreement : public CTransWindow {
public:
  CUIAgreement();
  virtual ~CUIAgreement();

  BEGIN_MSG_MAP(CUIAgreement)
  MESSAGE_HANDLER(WM_MOUSEWHEEL, OnWheel)
  CHAIN_MSG_MAP(CTransWindow)
  END_MSG_MAP()

public:
  LRESULT OnWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

  virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL &bHandled) override;

  void CreateWnd(HWND hWnd);

  void DestroyWnd();

  virtual void OnLoad();
  virtual void OnLoaded();

  virtual void OnExit();

  virtual void OnFinalMessage(HWND);

  virtual void DrawBorder(CDCHandle dc) override;

private:
  CUIContents *_obj;
};
