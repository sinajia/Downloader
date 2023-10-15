#pragma once

#include "../net/url_fetcher.h"

struct DownCompleteInfo {
  explicit DownCompleteInfo(net::URLFetcher::ErrorType reason);
  net::URLFetcher::ErrorType reason_;
  union {
    DWORD file_totle_size_;
    struct {
      DWORD already_byte_infile_;
      DWORD endoffset_;
      DWORD original_begin_offset_;
    } mutDownParam;
  };
};

class FetchURLDelegate {
public:
  virtual void FetchURLComplete(std::string res) = 0;
};

class FetchFileDelegate {
public:
  virtual void FetchFileComplete(bool status, DownCompleteInfo &info) = 0;
  virtual void FetchFileProgress(double cur) = 0;
};
