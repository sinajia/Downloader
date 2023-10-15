#include "StdAfx.h"
#include "common_threads.h"

namespace base {
MessageLoop *Threads::threads_[Threads::COUNT] = { 0 };

MessageLoop *Threads::Get(ThreadType type) { return Threads::threads_[type]; }

void Threads::Set(ThreadType type, MessageLoop *loop) {
  assert(type < Threads::COUNT && !Threads::threads_[type]);
  Threads::threads_[type] = loop;
}
} // namespace base
