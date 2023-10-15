#include "stdafx.h"
#include "WaitableEvent.h"

namespace base {
WaitableEvent::WaitableEvent(bool manual_reset, bool signaled)
    : handle_(CreateEvent(NULL, manual_reset, signaled, NULL)) {

  assert(handle_);
}

WaitableEvent::WaitableEvent(HANDLE event_handle) : handle_(event_handle) {
  assert(handle_ != INVALID_HANDLE_VALUE);
}

HANDLE WaitableEvent::Release() {
  HANDLE rv = handle_;
  handle_ = NULL;
  return rv;
}

WaitableEvent::~WaitableEvent() {
  assert(handle_);
  CloseHandle(handle_);
  handle_ = NULL;
}

void WaitableEvent::Reset() { ResetEvent(handle_); }

void WaitableEvent::Signal() { SetEvent(handle_); }

void WaitableEvent::Wait() {
  DWORD result = WaitForSingleObject(handle_, INFINITE);
}

bool WaitableEvent::TimedWait(const TimeDelta &max_time) {
  double timeout = ceil(max_time.InMillisecondsF());
  DWORD result = WaitForSingleObject(handle_, static_cast<DWORD>(timeout));
  switch (result) {
  case WAIT_OBJECT_0:
    return true;
  case WAIT_TIMEOUT:
    return false;
  }

  assert(0);
  return false;
}

} // namespace base
