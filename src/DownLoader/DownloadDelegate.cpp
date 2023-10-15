#include "StdAfx.h"
#include "DownloadDelegate.h"

DownCompleteInfo::DownCompleteInfo(net::URLFetcher::ErrorType reason)
    : reason_(reason) {}
