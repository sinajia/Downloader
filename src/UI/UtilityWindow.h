#pragma once

#include <Mmsystem.h>
#include <strsafe.h>
#include "util/def.h"
#include "../Util/UtilApi.h"
#include "../base/ref_counted.h"
#include "../Global.h"
#include "UIMainWindowEx.h"

class CUtilityWindow : public CWindowImpl<CUtilityWindow> {
public:
  DECLARE_WND_CLASS(_T("AppInstallerWindow"))
  CUtilityWindow();
  virtual ~CUtilityWindow();
  BEGIN_MSG_MAP(CUtilityWindow)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_DEL_GDI_OBJ, OnDelGdi)
  MESSAGE_HANDLER(WM_IS_EXIST, OnRestore)
  END_MSG_MAP()

  static CUtilityWindow &GetInstance();
  BOOL Initialize();
  void UnInitialize();

private:
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/,
                   BOOL & /*bHandled*/);
  LRESULT OnDelGdi(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnRestore(UINT, WPARAM, LPARAM, BOOL &);

  scoped_refptr<CUIMainWindowEx> pUIMainWindow_;
};
