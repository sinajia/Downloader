#pragma once

#include "MessageLoop.h"

namespace base {
class MessageLoop;

class Threads {
public:
  enum ThreadType {
    UI = 0,
    IO,
    FILE,
    DB,
    COUNT,
  };

  static MessageLoop *Get(ThreadType type);
  static void Set(ThreadType type, MessageLoop *loop);

private:
  static MessageLoop *threads_[COUNT];
};

} // namespace base
