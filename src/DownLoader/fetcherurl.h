#pragma once

#include "../net/url_fetcher.h"
#include "../base.h"
#include "util/def.h"
#include "DownloadDelegate.h"

class CFetchUrl : public net::URLFetcher::URLFetcherDelegate {
public:
  explicit CFetchUrl(FetchURLDelegate *delegate, std::string downurl);
  ~CFetchUrl();
  virtual void OnURLFetchComplete(const net::URLFetcher *source) override;
  scoped_refptr<net::URLFetcher> fetcher_;

private:
  FetchURLDelegate *delegate_;
};
