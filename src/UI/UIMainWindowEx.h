#pragma once

#include "util/def.h"
#include "../DirectUI/DUITransWindowEx.h"
#include "../DownLoader/DownloadDelegate.h"
#include "../DownLoader/fetcherurl.h"
#include "../DownLoader/fetchfile.h"
#include "../Event/WaitableEvent.h"
#include "../base/MessageLoop.h"
#include "../base/MessagePumpWin.h"
#include "../base/macros.h"
#include "../base/notification_observer.h"
#include "../base/notification_registrar.h"
#include "FrameShowState.h"

class CUIMainWindowEx : public CTransWindowEx<CUIMainWindowEx>,
                         public base::RefCountedThreadSafe<CUIMainWindowEx>,
                         public FetchURLDelegate,
                         public FetchFileDelegate {
public:
  enum InstallStep {
    BEFOREDOWN = 0,
    DOWNLOADING,
    INSTALLING,
    INSTALLED,
  };

  enum ShortCutType {
    SHORTCUTBEGIN = 0,
    DESK,
    STARTMENU,
    TASKBAR,
  };

  enum UserInstallType {
    DEFAULT = 0,
    SELECT,
  };

  enum DownLoadWay {
    UNKNOW = 0,
    SINGLE,
    MULTI,
  };

  enum DownLoadFail {
    NO_SPACE = 0,
    CONNECT_ERROR,
  };

  BEGIN_MSG_MAP(CUIMainWindowEx)
  MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
  CHAIN_MSG_MAP(CTransWindowEx<CUIMainWindowEx>)
  MESSAGE_HANDLER(WM_TASKBARBUTTONCREATED, OnTaskBarCreated)
  END_MSG_MAP()

  CUIMainWindowEx();
  virtual ~CUIMainWindowEx();

  LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                              BOOL &bHandled) override;
  virtual LRESULT OnPathIllCharacter(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL &bHandled) override;
  LRESULT OnTaskBarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam,
                           BOOL &bHandled);

  HWND CreateWnd(HWND hParent, const WCHAR* wstr);
  void DestroyWnd();
  void RestoreWindow();

  virtual void OnLoad() override;
  virtual void OnLoaded() override;
  virtual void OnExit() override {
    _elementManager.Dispose();
    DestroyWnd();
    ::PostQuitMessage(0);
  }

  virtual void IsWindowShowed() override;

  virtual void StartShake() override;

  virtual void Shaked() override;

  virtual void DrawBorder(CDCHandle dc) override;

  virtual void FetchURLComplete(std::string res) override;

  virtual void FetchFileComplete(bool status, DownCompleteInfo &rest) override;

  virtual void FetchFileProgress(double cur) override;

private:
  static void DeletePack(const std::wstring& str_path);

  void OnMin();
  void OnClose();
  void OnInstall();
  void OnInstallSelected();
  void OnAgreement();
  void OnCheck();
  void OpenInstallDir();
  void OnSelected();
  void OnBackMain();
  void OnUseNow();
  void OnInstallComplete();

  void CreateShortCut(LPCTSTR strPath, const ShortCutType Stype = DESK);
  void SetProgressTaskBar(DWORD now, DWORD total);
  void Rotational();
  void Ready2DownloadZip();
  void ClearDownloads(const std::wstring str_path);
  void FileInit();
  void QueryDownLoadUrl();
  void DoInstall();
  void InstallIng();
  void CheckIFTureWillDownLoad();
  void DownLoadingAUserQuit();
  void CombineRealInstallPath();
  void ForceCreateInstallPath(std::wstring path);
  bool CheckCanInstall();
  void RetryDownLoadAgain();
  bool GetPathEditError();
  void DownLoadingFileUrl();
  void UnZipComplete(BOOL res);
  bool GetJsonTokenByDownLoadUrl(const std::string &res,
                                 std::map<std::string, std::string> &map_token);

  CFetchUrl urldownloader_;
  CFetchFile filedownloader_;

  InstallStep instep_;
  UserInstallType installtype_;
  DownLoadWay downloadway_;
  ShowState showstate_;

  DWORD time_download_;
  DWORD time_install_;

  bool b_user_quit_;
  bool window_showing_;
  bool is_pack_complete_;
  double f_down_process_;

};
