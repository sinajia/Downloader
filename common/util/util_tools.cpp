#include "stdafx.h"
#include "util_tools.h"

CriticalSection::CriticalSection(void) {
#ifdef _WIN32
  InitializeCriticalSection(&m_cs);
#else
  pthread_mutex_init(&m_cs, NULL);
#endif
}

CriticalSection::~CriticalSection(void) {
#ifdef _WIN32
  DeleteCriticalSection(&m_cs);
#else
  pthread_mutex_destroy(&m_cs);
#endif
}

void CriticalSection::Lock(void) {
#ifdef _WIN32
  EnterCriticalSection(&m_cs);
#else
  pthread_mutex_lock(&m_cs);
#endif
}

void CriticalSection::UnLock(void) {
#ifdef _WIN32
  LeaveCriticalSection(&m_cs);
#else
  pthread_mutex_unlock(&m_cs);
#endif
}

BOOL CriticalSection::TryLock(void) {
  BOOL bReturn = FALSE;

#ifdef _WIN32
  bReturn = TryEnterCriticalSection(&m_cs);
#else
  bReturn = pthread_mutex_trylock(&m_cs);
#endif

  return bReturn;
}
