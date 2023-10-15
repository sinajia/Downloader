#include "StdAfx.h"
#include "fetcherurl.h"

using namespace net;

CFetchUrl::CFetchUrl(FetchURLDelegate *delegate, std::string downurl) {
  assert(delegate);
  delegate_ = delegate;
  fetcher_ = URLFetcher::Create(URLFetcher::Params(downurl), this);
}

CFetchUrl::~CFetchUrl() {
  if (fetcher_.get())
    fetcher_->Stop();
}

void CFetchUrl::OnURLFetchComplete(const net::URLFetcher *source) {
  std::string content;
  if (source->status()) {
    content = const_cast<net::URLFetcher *>(source)->data();
  }
  if (delegate_)
    delegate_->FetchURLComplete(content);
}
