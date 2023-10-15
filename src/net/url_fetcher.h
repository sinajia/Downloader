#pragma once

#include "../Event/WaitableEvent.h"
#include "../base/MessageLoop.h"
#include "../base/MessagePumpWin.h"
#include "../base/macros.h"
#include "../base/notification_observer.h"
#include "../base/notification_registrar.h"

namespace net {
class URLFetcher : public base::MessagePumpForIO::IOHandler,
                   public base::MessageLoop::DestructionObserver,
                   public base::RefCountedThreadSafe<URLFetcher> {
public:
  class URLFetcherDelegate {
  public:
    virtual void OnURLFetchComplete(const URLFetcher *source) = 0;
    virtual void OnURLFetchDownloadProgress(const URLFetcher *source,
                                            double cur);
    virtual ~URLFetcherDelegate();
  };
  enum ErrorType {
    ERROR_NO,
    DISK_NO_SPACE,
    FILE_NO_EXIST,
    NOT_SUPPORT_BREAKPOINT,
    ERROR_SERV,
  };

  enum DownType {
    STRING,
    FILE,
    TEST_BREAKPOINT_SUPPORT,
  };

  struct Params {
    explicit Params(const std::string& url);
    explicit Params(const std::string &url, std::wstring file_name);
    explicit Params(const std::string &url, DownType type);

    std::string url_;
    std::wstring name_;
    DownType type_;
    DWORD beginwoffset_;
    DWORD endoffset_;
  };

  struct State {
    explicit State(URLFetcher *fetcher) { context.handler = fetcher; }
    ~State() {}
    base::MessagePumpForIO::IOContext context;
  };

  static URLFetcher *Create(const Params &params, URLFetcherDelegate *delegate);

  ~URLFetcher();

  void Start();

  void Stop();

  virtual void WillDestroyCurrentMessageLoop() override;

  virtual void OnIOCompleted(base::MessagePumpForIO::IOContext *context,
                             DWORD bytes_transfered, DWORD error) override;

  void FetchComplete();
  void FetchDownloadProgress(double cur);

  void set_download_url(const std::string &url) { download_url_ = url; }
  void set_file_loadpath(const std::wstring &path) { download_file_name_ = path; }
  void set_status(const bool status) { status_ = status; }

  bool status() const { return status_; }
  bool quit_status() { return quit_status_; }
  std::string& data() { return data_; }
  const std::string& durl() { return download_url_; }
  const std::wstring& download_file_name() { return download_file_name_; }

  HANDLE io_port() { return io_port_; }
  base::WaitableEvent& event() { return event_; }
  State &state() { return input_state_; }
  DownType type() { return type_; }

private:
  explicit URLFetcher(const Params &params, URLFetcherDelegate *delegate);

  void StartInternal();
  void StopInternal();

  bool status_;
  bool quit_status_;

  std::string data_;
  std::string download_url_;
  std::wstring download_file_name_;

  HANDLE io_port_;
  State input_state_;
  DownType type_;
  URLFetcherDelegate *delegate_;
  base::WaitableEvent event_;

public:
  ErrorType error_type_;
  double fdownloadprogress_;
  bool support_breakpoint_;
  DWORD already_byteinfile_;
  DWORD file_totle_len_;

  DWORD beginwoffset_;
  DWORD endoffset_;
  DWORD orginal_begin_offset_;
};

} // namespace net
