#pragma once

#include "macros.h"

namespace base {
template <typename Type> class ThreadLocalPointer {
public:
  ThreadLocalPointer() {
    slot_ = TlsAlloc();
    assert(slot_ != TLS_OUT_OF_INDEXES);
  }

  ~ThreadLocalPointer() {
    if (!TlsFree(slot_)) {
      assert(0);
    }
  }

  Type *Get() { return static_cast<Type *>(TlsGetValue(slot_)); }

  void Set(Type *ptr) {
    if (!TlsSetValue(slot_,
                     const_cast<void *>(static_cast<const void *>(ptr)))) {
      assert(0);
    }
  }

private:
  DWORD slot_;

  DISALLOW_COPY_AND_ASSIGN(ThreadLocalPointer<Type>);
};
} // namespace base
