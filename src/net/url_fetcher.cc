#include "StdAfx.h"
#include "../base.h"
#include "../LogAssist/LogAssist.h"
#include "util/system.h"
#include "url_fetcher.h"

namespace net {
#define DEFAULT_WININET_TIMEOUT (20 * 1000)

#define USER_AGENT                                                \
  "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Trident/4.0;)"

#define DEFAULT_CONNECT_RETRYS 2

static DWORD WINAPI ThreadProc(LPVOID lpParameter);

bool SetTimeout(HINTERNET &hConnect,
                ULONG uTimeoutSec = DEFAULT_WININET_TIMEOUT) {
  uTimeoutSec *= 2;
  bool bret = false;
  do {
    if (!::InternetSetOption(hConnect, INTERNET_OPTION_SEND_TIMEOUT,
                             &uTimeoutSec, sizeof(uTimeoutSec)))
      break;

    if (!::InternetSetOption(hConnect, INTERNET_OPTION_RECEIVE_TIMEOUT,
                             &uTimeoutSec, sizeof(uTimeoutSec)))
      break;

    uTimeoutSec /= 2;
    if (!::InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_TIMEOUT,
                             &uTimeoutSec, sizeof(uTimeoutSec)))
      break;

    ULONG uRetrys = DEFAULT_CONNECT_RETRYS;
    if (!::InternetSetOption(hConnect, INTERNET_OPTION_CONNECT_RETRIES,
                             &uRetrys, sizeof(uRetrys)))
      break;

    bret = true;
  } while (false);
  if (!bret) {
    PRT_LOG3(DebugSet, ("***failed***"))
  }
  return bret;
}

URLFetcher *URLFetcher::Create(const Params &params,
                               URLFetcherDelegate *delegate) {
  return (new URLFetcher(params, delegate));
}

URLFetcher::Params::Params(const std::string& url)
  : url_(url), type_(STRING), beginwoffset_(0), endoffset_(0) {}

URLFetcher::Params::Params(const std::string &url, std::wstring file_name)
    : url_(url), name_(file_name), type_(FILE), beginwoffset_(0), endoffset_(0) {}

URLFetcher::Params::Params(const std::string &url, DownType type)
    : url_(url), type_(type), beginwoffset_(0), endoffset_(0) {}

void URLFetcher::URLFetcherDelegate::OnURLFetchDownloadProgress(
    const URLFetcher *source, double cur) {}

URLFetcher::URLFetcherDelegate::~URLFetcherDelegate() {}

URLFetcher::URLFetcher(const Params &params, URLFetcherDelegate *delegate)
    : download_url_(params.url_), type_(params.type_),
      download_file_name_(params.name_), delegate_(delegate),
      input_state_(this), fdownloadprogress_(0.0), status_(false),
      event_(true, false), quit_status_(false),
      beginwoffset_(params.beginwoffset_), endoffset_(params.endoffset_),
      file_totle_len_(0), support_breakpoint_(true), error_type_(ERROR_NO),
      already_byteinfile_(0), orginal_begin_offset_(0) {
  assert(download_url_.length());
  io_port_ = base::Threads::Get(Threads::IO)->pump_win()->io_port();
  assert(io_port_);
}

URLFetcher::~URLFetcher() {
  if (type_ == DownType::FILE && download_file_name_.length()) {
    SystemCommon::FilePathHelper::DeepDeleteFile(download_file_name_);
  }
}

void URLFetcher::Start() {
  assert(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  base::Threads::Get(Threads::IO)
      ->PostTask(Bind(this, &URLFetcher::StartInternal));
}

void URLFetcher::Stop() {
  assert(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
  delegate_ = nullptr;
  quit_status_ = true;
}

void URLFetcher::WillDestroyCurrentMessageLoop() {
  quit_status_ = true;
  event_.Wait();
  Release();
}

void URLFetcher::OnIOCompleted(base::MessagePumpForIO::IOContext* context,
  DWORD bytes_transfered, DWORD error) {
  base::Threads::Get(Threads::IO)->RemoveDestructionObserver(this);

  if (delegate_) {
    base::Threads::Get(Threads::UI)
      ->PostTask(Bind(this, &URLFetcher::FetchComplete));
  }

  Release();
}

void URLFetcher::FetchComplete() {
  if (delegate_) {
    delegate_->OnURLFetchComplete(this);
  }
}

void URLFetcher::FetchDownloadProgress(double cur) {
  fdownloadprogress_ = cur;
  if (delegate_) {
    delegate_->OnURLFetchDownloadProgress(this, fdownloadprogress_);
  }
}

void URLFetcher::StartInternal() {
  AddRef();
  AddRef();
  base::Threads::Get(Threads::IO)->AddDestructionObserver(this);
  if (!QueueUserWorkItem(ThreadProc, this, WT_EXECUTEDEFAULT)) {
    Release();
    base::Threads::Get(Threads::IO)->RemoveDestructionObserver(this);
    status_ = false;
    base::Threads::Get(Threads::UI)
        ->PostTask(Bind(this, &URLFetcher::FetchComplete));
    Release();
    return;
  }
}

void  URLFetcher::StopInternal() {

}

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  URLFetcher *const url_fetcher = reinterpret_cast<URLFetcher *>(lpParameter);

  bool bResult = false;
  const std::string &url = url_fetcher->durl();

  switch (url_fetcher->type()) {
  case URLFetcher::STRING: {
    std::string &contents = url_fetcher->data();
    HINTERNET hSession = ::InternetOpenA(
        USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession) {
      SetTimeout(hSession);
      HINTERNET hRequest = ::InternetOpenUrlA(hSession, url.c_str(), NULL, 0,
                                              INTERNET_FLAG_RELOAD, NULL);
      if (hRequest) {
        TCHAR szBuffer[256] = {0};
        DWORD dwSize = _countof(szBuffer);
        if (HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, szBuffer, &dwSize,
                          NULL) &&
            _tcscmp(szBuffer, _T("200")) == 0) {

          for (;;) {
            if (url_fetcher->quit_status()) {
              break;
            }
            char btBuffer[1024 + 1] = {0};
            DWORD dwFileLength = 0;
            if (!::InternetReadFile(hRequest, btBuffer, 1024, &dwFileLength)) {
              break;
            }

            if (dwFileLength == 0) {
              bResult = true;
              break;
            }
            btBuffer[dwFileLength] = NULL;
            contents += btBuffer;
          }
        }

        ::InternetCloseHandle(hRequest);
      }

      ::InternetCloseHandle(hSession);
    }
  } break;
  case URLFetcher::FILE: {
    HINTERNET hSession = ::InternetOpenA(
        USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hSession) {
      SetTimeout(hSession);
      HINTERNET hRequest =
          ::InternetOpenUrlA(hSession, url.c_str(), NULL, 0, 0, NULL);
      if (hRequest) {
        TCHAR szBuffer[256] = {0};
        DWORD dwSize = _countof(szBuffer);
        DWORD dwContentLength = -1;
        DWORD dwAlreadyReadLen = 0;
        if (HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, szBuffer, &dwSize,
                          NULL) &&
            _tcscmp(szBuffer, _T("200")) == 0) {
          dwSize = sizeof(dwContentLength);

          BOOL issucc = HttpQueryInfo(
              hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
              &dwContentLength, &dwSize, NULL);
          if (!issucc || -1 == dwContentLength) {
            url_fetcher->error_type_ = URLFetcher::ERROR_SERV;
            ::InternetCloseHandle(hRequest);
            ::InternetCloseHandle(hSession);
            break;
          }
          std::wstring filepath = url_fetcher->download_file_name();
          HANDLE hFile =
              ::CreateFileW(filepath.c_str(), GENERIC_WRITE, NULL, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
          if (INVALID_HANDLE_VALUE != hFile) {
            do {
              SetFilePointer(hFile, dwContentLength, NULL, FILE_BEGIN);
              if (SetEndOfFile(hFile) == FALSE) {
                url_fetcher->error_type_ = URLFetcher::DISK_NO_SPACE;
                break;
              }
              SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
              DWORD dwSizePost = 0;
              for (;;) {
                if (url_fetcher->quit_status()) {
                  break;
                }
                char btBuffer[1024 * 8 + 1] = {0};
                DWORD dwFileLength = 0;
                if (!::InternetReadFile(hRequest, btBuffer, 1024 * 8,
                                        &dwFileLength)) {
                  break;
                }
                dwAlreadyReadLen += dwFileLength;
                dwSizePost += dwFileLength;
                if (dwFileLength == 0) {
                  base::Threads::Get(Threads::UI)
                      ->PostTask(Bind(url_fetcher,
                                      &URLFetcher::FetchDownloadProgress, 1.0));
                  bResult = true;
                  break;
                }

                DWORD dwWriteLength = 0;
                ::WriteFile(hFile, btBuffer, dwFileLength, &dwWriteLength,
                            NULL);
                url_fetcher->already_byteinfile_ += dwFileLength;
                if (dwSizePost > 1024 * 8) {
                  dwSizePost = 0;
                  double cur =
                      static_cast<double>(dwAlreadyReadLen) / dwContentLength;
                  base::Threads::Get(Threads::UI)
                      ->PostTask(Bind(url_fetcher,
                                      &URLFetcher::FetchDownloadProgress, cur));
                }
              }
            } while (false);
            ::CloseHandle(hFile);
          }
        }
        ::InternetCloseHandle(hRequest);
      }
      ::InternetCloseHandle(hSession);
    }
  } break;
  case URLFetcher::TEST_BREAKPOINT_SUPPORT: {
    if (!url_fetcher->support_breakpoint_) {
      url_fetcher->error_type_ = URLFetcher::NOT_SUPPORT_BREAKPOINT;
      break;
    }
    HINTERNET hSession = NULL;
    hSession = ::InternetOpenA(USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL,
                               NULL, 0);
    if (hSession) {
      SetTimeout(hSession);
      char vheaders[200] = {0};
      DWORD dwZero = 0;
      _snprintf_s(vheaders, 200, _TRUNCATE, "Range:bytes=%u-\r\n", dwZero);
      HINTERNET hRequest =
          ::InternetOpenUrlA(hSession, url.c_str(), vheaders, -1L, 0, NULL);
      if (hRequest) {
        TCHAR szBuffer[256] = {0};
        DWORD dwSize = _countof(szBuffer);
        if (HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, szBuffer, &dwSize,
                          NULL) &&
            _tcscmp(szBuffer, _T("206")) == 0) {
          DWORD dwContentLength = -1;
          DWORD dwSize = sizeof(dwContentLength);
          BOOL issucc = HttpQueryInfo(
              hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
              &dwContentLength, &dwSize, NULL);
          if (FALSE == issucc || -1 == dwContentLength) {
            url_fetcher->error_type_ = URLFetcher::ERROR_SERV;
            ::InternetCloseHandle(hRequest);
            ::InternetCloseHandle(hSession);
            break;
          }
          url_fetcher->file_totle_len_ = dwContentLength;
          bResult = true;
        } else {
          url_fetcher->support_breakpoint_ = false;
        }
        ::InternetCloseHandle(hRequest);
      }
      ::InternetCloseHandle(hSession);
    }
  } break;

  default:
    assert(0);
    break;
  }

  url_fetcher->set_status(bResult);

  PostQueuedCompletionStatus(
      url_fetcher->io_port(),
      (url_fetcher->type() == URLFetcher::STRING) ? url_fetcher->data().length()
                                                  : 0,
      reinterpret_cast<ULONG_PTR>(
          static_cast<base::MessagePumpForIO::IOHandler *>(url_fetcher)),
      &url_fetcher->state().context.overlapped);

  url_fetcher->event().Signal();

  url_fetcher->Release();

  return 0;
}

} // namespace net
