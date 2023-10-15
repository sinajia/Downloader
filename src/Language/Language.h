#pragma once

namespace lan {
struct Sentence {
  LPCWSTR pstrInstalling;
  LPCWSTR pstrServiceError;
  LPCWSTR pstrRepair;
  LPCWSTR pstrDiskSpace;
  LPCWSTR pstrApkInstalling;
};

class AppLanguage {
public:
  static Sentence s_lanArr[];
  static DWORD lanIndex_;
  static void SysLanguage();
};
} // namespace lan
