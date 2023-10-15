#pragma once

#include <Fcntl.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <sys/stat.h>
#include "../base/macros.h"
#include "../base.h"

#define FORMAT(x) applog::LogAssist::Print##x
#define INIT_LOG(obj, szDirectory) obj.OpenLog(3, szDirectory);
#define PRT_LOG(obj, alevel, y)  obj.LogString(__FILE__, __LINE__, alevel, FORMAT(y));
#define PRT_LOG3(obj, y) PRT_LOG(obj, 3, y)

namespace applog {
constexpr int DEF_MAX_BUFF_LEN = 2048;
constexpr int DEF_MAX_FILE_SIZE = 1024 * 1024 * 2;

class STRU_LOG_DATA {
public:
  STRU_LOG_DATA();
  STRU_LOG_DATA &operator<<(char acCharVal);
  STRU_LOG_DATA &operator<<(bool abBoolVal);
  STRU_LOG_DATA &operator<<(short asShortVal);
  STRU_LOG_DATA &operator<<(int aiIntVal);
  STRU_LOG_DATA &operator<<(unsigned int aiIntVal);
  STRU_LOG_DATA &operator<<(float afFloatVal);
  STRU_LOG_DATA &operator<<(double afdoubleVal);
  STRU_LOG_DATA &operator<<(__int64 aiInt64Val);
  STRU_LOG_DATA &operator<<(const char *apStrVal);

  char mszPrintBuff[DEF_MAX_BUFF_LEN + sizeof(short)];
  int mlDataLen = 0;
};

class LogThread {
public:
  base::Thread &Thread() { return thread_; }
  DISALLOW_COPY_AND_ASSIGN(LogThread);

private:
  friend class base::LazyInstance<LogThread>;
  friend class base::DefaultLazyInstanceTraits<LogThread>;
  LogThread();
  ~LogThread();
  base::Thread thread_;
};

class LogAssist {
public:
  LogAssist();
  ~LogAssist();

  enum Options {
    Timestamp = 1,

    LogLevel = 2,

    FileAndLine = 4,

    AppendToFile = 8,

    PrintToConsole = 16,

    PrintThreadID = 32
  };

public:
  void OpenLog(int anLogLevel, const tstring &strDirectory);

  void CloseLog();

  void SetOptions(unsigned options);

  void LogString(const std::string &strFile, int aiLine, int aiLogLevel,
                 const std::string &strContent);
  static std::string Print(const char *pFmt, ...);

private:
  void DoPrintLog(const std::string &strFile, int aiLine, int aiLogLevel,
                  const std::string &strContent);
  void PrintLogInUI(const std::string &strFile, int aiLine, int aiLogLevel,
                    const std::string &strContent);
  void InitDebug(int anLogLevel, const tstring &strDirectory);
  void InitLog(const tstring &strDirectory);
  bool CanTrace(int aiLogLevel);
  void SetTraceLevel(int aiTraceLevel);
  void SetTraceOptions(unsigned options);
  unsigned GetTraceOptions(void);
  void OptionTrace(STRU_LOG_DATA &astruDebugData, int aiLogLevel);
  void TraceFormat(STRU_LOG_DATA &astruDebugData, const char *pFmt, ...);
  void FileLineTrace(STRU_LOG_DATA &astruDebugData, char *apSrcFile,
                     int aiSrcLine);
  void EndTrace(STRU_LOG_DATA &astruDebugData);

  static tstring CreateLogFile(LPCTSTR szDirectory);

private:
  tstring mszLogFileName;

  unsigned muTraceOptions;
  long mnLogLevel = 3;
  bool mbIsOpen = false;
  bool mbInitial = false;
};

extern base::LazyInstance<LogThread> log_entity_;

} // namespace applog

extern applog::LogAssist DebugSet;
