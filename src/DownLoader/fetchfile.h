#pragma once

#include "../net/url_fetcher.h"
#include "../base.h"
#include "util/def.h"
#include "DownloadDelegate.h"

class CFetchFile : public net::URLFetcher::URLFetcherDelegate {
public:
  explicit CFetchFile(FetchFileDelegate *delegate, std::string downurl,
                      std::wstring downtopath);
  ~CFetchFile();
  virtual void OnURLFetchComplete(const net::URLFetcher *source) override;
  virtual void OnURLFetchDownloadProgress(const net::URLFetcher *source,
                                          double cur) override;
  scoped_refptr<net::URLFetcher> fetcher_;
  char retrycount_;

private:
  FetchFileDelegate *delegate_;
  
};
