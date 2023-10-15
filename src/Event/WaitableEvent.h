#pragma once

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "../time/time.h"

namespace base {
class TimeDelta;

class WaitableEvent {
public:
  WaitableEvent(bool manual_reset, bool initially_signaled);

  explicit WaitableEvent(HANDLE event_handle);
  HANDLE Release();

  ~WaitableEvent();

  void Reset();

  void Signal();

  void Wait();

  bool TimedWait(const TimeDelta &max_time);

  HANDLE handle() const { return handle_; }

private:
  HANDLE handle_;

};

} // namespace base
