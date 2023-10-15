#include "StdAfx.h"
#include "../Global.h"
#include "Language.h"

namespace lan {
Sentence AppLanguage::s_lanArr[] = {
    {L"Installing...Please wait!~", L"Server error, Try again!",
     L"Install directly", L"Disk space not enough",
     L"Downloading ... Please wait"},
};
DWORD AppLanguage::lanIndex_ = 0;

void AppLanguage::SysLanguage() {
  global.lanId_ = GetSystemDefaultLangID();
  switch (global.lanId_) {
  case 0x804: {
    global.m_zh = true;
  } break;
  default:
    break;
  }
}

} // namespace lan
