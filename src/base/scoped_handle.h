#pragma once

#include <windows.h>
#include "macros.h"

namespace base {
class HandleTraits {
public:
  typedef HANDLE Handle;

  // Closes the handle.
  static bool CloseHandle(HANDLE handle) {
    return ::CloseHandle(handle) != FALSE;
  }

  // Returns true if the handle value is valid.
  static bool IsHandleValid(HANDLE handle) {
    return handle != NULL && handle != INVALID_HANDLE_VALUE;
  }

  // Returns NULL handle value.
  static HANDLE NullHandle() { return NULL; }

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(HandleTraits);
};

template <typename Traits = HandleTraits> class ScopedWinHandle {
public:
  typedef typename Traits::Handle Handle;

  ScopedWinHandle() : handle_(Traits::NullHandle()) {}

  explicit ScopedWinHandle(Handle handle) : handle_(Traits::NullHandle()) {
    Set(handle);
  }

  ScopedWinHandle &operator=(ScopedWinHandle &other) {
    if (handle_ != other.handle_ && IsValid()) {
      Close();
    }
    handle_ = other.Release();
    return *this;
  }

  ScopedWinHandle(ScopedWinHandle &other) { handle_ = other.Release(); }

  ~ScopedWinHandle() { Close(); }

  bool IsValid() const { return Traits::IsHandleValid(handle_); }

  void Set(Handle handle) {
    if (handle_ != handle) {
      Close();
    }
    handle_ = handle;
  }

  Handle Get() const { return handle_; }

  operator Handle() { return handle_; }

  void Close() {
    if (Traits::IsHandleValid(handle_)) {
      if (!Traits::CloseHandle(handle_)) {
        assert(0);
      }
      handle_ = Traits::NullHandle();
    }
  }

  Handle Release() {
    Handle tmp = handle_;
    handle_ = Traits::NullHandle();
    return tmp;
  }

private:
  Handle handle_;
};

} // namespace base
