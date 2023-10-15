#include "StdAfx.h"
#include <assert.h>
#include <io.h>
#include <Shlwapi.h>
#include "LogAssist.h"
#include "util/system.h"

#pragma comment(lib, "shlwapi")

applog::LogAssist DebugSet;

namespace applog {
base::LazyInstance<LogThread> log_entity_ = LAZY_INSTANCE_INITIALIZER;

LogThread::LogThread() : thread_() {}

LogThread::~LogThread() {
  if (thread_.IsRunning()) {
    thread_.Stop();
  }
}

STRU_LOG_DATA::STRU_LOG_DATA() {
  mlDataLen = 0;
  ZeroMemory(&mszPrintBuff, sizeof(STRU_LOG_DATA::mszPrintBuff));
};

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(char acCharVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 1) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 2, "%c", acCharVal);
    StringCchLengthA(lpWritePtr, 2, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(bool abBoolVal) {
  if (mlDataLen < DEF_MAX_BUFF_LEN - 5) {
    char *lpWritePtr = (mszPrintBuff) + mlDataLen;
    unsigned int len = 0;
    if (abBoolVal) {
      StringCchPrintfA(lpWritePtr, 6, "true");
      StringCchLengthA(lpWritePtr, 6, &len);
    } else {
      StringCchPrintfA(lpWritePtr, 6, "%s", "false");
      StringCchLengthA(lpWritePtr, 6, &len);
    }
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(short asShortVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 6) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 7, "%d", asShortVal);
    StringCchLengthA(lpWritePtr, 7, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(int aiIntVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 11) {
    char *lpWritePtr = (char *)mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 12, "%d", aiIntVal);
    StringCchLengthA(lpWritePtr, 12, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(unsigned int aiIntVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 11) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 12, "%u", aiIntVal);
    StringCchLengthA(lpWritePtr, 12, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(float afFloatVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 20) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 21, "%f", afFloatVal);
    StringCchLengthA(lpWritePtr, 21, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(double afdoubleVal) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 25) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 26, "%lf", afdoubleVal);
    StringCchLengthA(lpWritePtr, 26, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(__int64 aiInt64Val) {
  unsigned int len = 0;
  if (mlDataLen < DEF_MAX_BUFF_LEN - 20) {
    char *lpWritePtr = mszPrintBuff + mlDataLen;
    StringCchPrintfA(lpWritePtr, 21, "%I64d", aiInt64Val);
    StringCchLengthA(lpWritePtr, 21, &len);
    mlDataLen += len;
  }
  return *this;
}

STRU_LOG_DATA &STRU_LOG_DATA::operator<<(const char *apStrVal) {
  unsigned int len = 0;
  char *lpWritePtr = mszPrintBuff + mlDataLen;
  if (!apStrVal) {
    if (mlDataLen < DEF_MAX_BUFF_LEN - 4) {
      StringCchPrintfA(lpWritePtr, 5, "%s", "NULL");
      StringCchLengthA(lpWritePtr, 5, &len);
      mlDataLen += len;
    }
  } else {
    int iLen = static_cast<int>(strlen(apStrVal));
    if (mlDataLen < DEF_MAX_BUFF_LEN - iLen) {
      StringCchPrintfA(lpWritePtr, iLen + 1, "%s", apStrVal);
      StringCchLengthA(lpWritePtr, iLen + 1, &len);
      mlDataLen += len;
    } else if (DEF_MAX_BUFF_LEN - mlDataLen > 1) {
      StringCchCopyA(lpWritePtr, DEF_MAX_BUFF_LEN - mlDataLen, apStrVal);
      mlDataLen += (DEF_MAX_BUFF_LEN - mlDataLen - 1);
    }
  }
  return *this;
}

LogAssist::LogAssist()
    : mbIsOpen(false), mbInitial(false), mnLogLevel(3),
      muTraceOptions(LogAssist::Timestamp | LogAssist::AppendToFile |
                     LogAssist::FileAndLine | LogAssist::PrintThreadID) {}

LogAssist::~LogAssist() {}

void LogAssist::OpenLog(int anLogLevel, const tstring &strDirectory) {
  CHECK(strDirectory.length());
  base::Threads::Get(base::Threads::UI)
      ->PostTask(base::Bind(base::Unretained(this), &LogAssist::InitDebug,
                            anLogLevel, strDirectory));
}

void LogAssist::CloseLog() { mbIsOpen = false; }

void LogAssist::SetOptions(unsigned options) {
  base::Threads::Get(base::Threads::UI)
      ->PostTask(base::Bind(base::Unretained(this), &LogAssist::SetTraceOptions,
                            options));
}

void LogAssist::LogString(const std::string &strFile, int aiLine,
                          int aiLogLevel, const std::string &strContent) {
  base::Threads::Get(base::Threads::UI)
      ->PostTask(base::Bind(base::Unretained(this), &LogAssist::PrintLogInUI,
                            strFile, aiLine, aiLogLevel, strContent));
}

std::string LogAssist::Print(const char *pFmt, ...) {
  char lstrPrintBuff[DEF_MAX_BUFF_LEN] = {0};
  va_list argptr;
  va_start(argptr, pFmt);
  StringCchVPrintfA(lstrPrintBuff, ARRAYSIZE(lstrPrintBuff) - 1, pFmt, argptr);
  va_end(argptr);
  std::string strBuffer = lstrPrintBuff;
  return strBuffer;
}

void LogAssist::InitDebug(int anLogLevel, const tstring &strDirectory) {
#ifdef NDEBUG
  return;
#endif

  CHECK(base::Threads::Get(Threads::UI) == base::MessageLoop::current());
  if (0 == strDirectory.length()) {
    return;
  }

  if (mbInitial) {
    return;
  }

  char szVariable[100] = { 0 };

  SetTraceLevel(anLogLevel);
  if (&log_entity_.Get()) {
    if (!log_entity_.Get().Thread().IsRunning()) {
      log_entity_.Get().Thread().set_thread_name("LogThread");
      log_entity_.Get().Thread().StartWithOptions(base::Thread::Options());
    }
    if (!log_entity_.Get().Thread().IsRunning()) {
      return;
    }

    mbInitial = true;
    mbIsOpen = true;

    log_entity_.Get().Thread().message_loop()->PostTask(
        base::Bind(base::Unretained(this), &LogAssist::InitLog, strDirectory));
  }
}

void LogAssist::PrintLogInUI(const std::string &strFile, int aiLine,
                             int aiLogLevel, const std::string &strContent) {
  if (!CanTrace(aiLogLevel) || !mbInitial)
    return;

  if (&log_entity_.Get()) {
    if (!log_entity_.Get().Thread().IsRunning() ||
        !log_entity_.Get().Thread().message_loop())
      return;
    log_entity_.Get().Thread().message_loop()->PostTask(
        base::Bind(base::Unretained(this), &LogAssist::DoPrintLog, strFile,
                   aiLine, aiLogLevel, strContent));
  }
}

void LogAssist::DoPrintLog(const std::string &strFile, int aiLine,
                           int aiLogLevel, const std::string &strContent) {
  STRU_LOG_DATA loDebugData;

  OptionTrace(loDebugData, aiLogLevel);

  loDebugData << strContent.c_str();

  char *lszSrcFile = PathFindFileNameA(strFile.c_str());
  FileLineTrace(loDebugData, lszSrcFile, aiLine);

  EndTrace(loDebugData);
}

tstring LogAssist::CreateLogFile(LPCTSTR szDirectory) {
  tstring strReturn;
  tstring strFileName;

  tstring filename0;
  tstring filename1;
  FILETIME fileTime0 = {0};
  FILETIME fileTime1 = {0};

  TCHAR szAppData[MAX_PATH + 1] = {0};
  GetModuleFileName(NULL, szAppData, MAX_PATH);
  strFileName = szAppData;
  strFileName = SystemCommon::FilePathHelper::GetPath(strFileName);
  PathCombine(szAppData, strFileName.c_str(), szDirectory);
  strFileName = szAppData;
  SystemCommon::FilePathHelper::CreateDir(strFileName.c_str());
  if (PathFileExists(strFileName.c_str())) {
    int i = 0;
    for (; i < 2; ++i) {
      TCHAR lszFileName[MAX_PATH] = {0};
      StringCchPrintf(lszFileName, MAX_PATH, _T("%d.log"), i);
      PathAppend(szAppData, lszFileName);

      tstring &filename = (0 == i) ? filename0 : filename1;
      filename = szAppData;

      HANDLE file = ::CreateFile(filename.c_str(), GENERIC_READ, NULL, NULL,
                                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if (file != INVALID_HANDLE_VALUE) {
        FILETIME &filetime = i ? fileTime1 : fileTime0;
        GetFileTime(file, NULL, NULL, &filetime);
        CloseHandle(file);
      }

      long size = SystemCommon::FilePathHelper::GetFileSize(szAppData);
      if (size < DEF_MAX_FILE_SIZE) {
        strReturn = filename;
        break;
      }
      PathRemoveFileSpec(szAppData);
    }
    if (2 == i) {
      strReturn =
          (CompareFileTime(&fileTime0, &fileTime1) > 0) ? filename1 : filename0;
      DeleteFile(strReturn.c_str());
    }
  }
  return strReturn;
}

void LogAssist::InitLog(const tstring &strDirectory) {
  mszLogFileName = CreateLogFile(strDirectory.c_str());
}

void LogAssist::SetTraceLevel(int aiTraceLevel) { mnLogLevel = aiTraceLevel; }

void LogAssist::SetTraceOptions(unsigned options) { muTraceOptions = options; }

unsigned LogAssist::GetTraceOptions(void) { return muTraceOptions; }

bool LogAssist::CanTrace(int aiLogLevel) {
  return mbIsOpen && (aiLogLevel <= mnLogLevel);
}

void LogAssist::OptionTrace(STRU_LOG_DATA &astruDebugData, int aiLogLevel) {
  astruDebugData.mlDataLen = 0;

  if (muTraceOptions & Timestamp) {
    SYSTEMTIME loSystemTime;
    GetLocalTime(&loSystemTime);

    char lszTraceBuff[MAX_PATH] = {0};
    StringCchPrintfA(lszTraceBuff, ARRAYSIZE(lszTraceBuff) - 1,
                     "%d.%d.%d %02d:%02d:%02d:%03d ", loSystemTime.wYear,
                     loSystemTime.wMonth, loSystemTime.wDay, loSystemTime.wHour,
                     loSystemTime.wMinute, loSystemTime.wSecond,
                     loSystemTime.wMilliseconds);

    astruDebugData << lszTraceBuff;
  }
  if (muTraceOptions & LogLevel) {
    astruDebugData << aiLogLevel << ' ';
  }
  return;
}

void LogAssist::TraceFormat(STRU_LOG_DATA &astruDebugData, const char *pFmt,
                            ...) {
  char szTempContent[DEF_MAX_BUFF_LEN] = {0};
  va_list argptr;
  va_start(argptr, pFmt);
  int liLen = StringCchVPrintfA(szTempContent, ARRAYSIZE(szTempContent) - 1,
                                pFmt, argptr);
  va_end(argptr);

  astruDebugData << szTempContent;
}

void LogAssist::FileLineTrace(STRU_LOG_DATA &astruDebugData, char *apSrcFile,
                              int aiSrcLine) {
  if (muTraceOptions & FileAndLine) {
    astruDebugData << "  " << apSrcFile << "[" << aiSrcLine << "]";
  }
  return;
}

void LogAssist::EndTrace(STRU_LOG_DATA &astruDebugData) {
  try {
    astruDebugData.mszPrintBuff[astruDebugData.mlDataLen] = '\n';

    if (muTraceOptions & PrintToConsole) {
      printf("%s", astruDebugData.mszPrintBuff);
    }

    if ((muTraceOptions & AppendToFile) && mszLogFileName.length()) {
      FILE *lfpTraceFile = NULL;
      lfpTraceFile = _tfopen(mszLogFileName.c_str(), _T("a"));
      if (NULL != lfpTraceFile) {
        fprintf(lfpTraceFile, "%s", astruDebugData.mszPrintBuff);
        fclose(lfpTraceFile);
      }
    }
  } catch (...) {
  }
}

} // namespace applog
