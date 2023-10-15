#include "StdAfx.h"
#include "fetchfile.h"

using namespace net;

CFetchFile::CFetchFile(FetchFileDelegate *delegate, std::string downurl,
                       std::wstring downtopath)
    : retrycount_(0) {
  assert(delegate);
  delegate_ = delegate;
  fetcher_ = URLFetcher::Create(URLFetcher::Params(downurl, downtopath), this);
}

CFetchFile::~CFetchFile() {
  if (fetcher_.get())
    fetcher_->Stop();
}

void CFetchFile::OnURLFetchComplete(const net::URLFetcher *source) {
  if (delegate_) {
    DownCompleteInfo info(source->error_type_);
    delegate_->FetchFileComplete(source->status(), info);
  }
}

void CFetchFile::OnURLFetchDownloadProgress(const net::URLFetcher *source,
                                            double cur) {
  if (delegate_)
    delegate_->FetchFileProgress(cur);
}
