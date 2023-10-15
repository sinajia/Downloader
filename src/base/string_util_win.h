#pragma once

#include <stdio.h>
#include <string.h>
#include <wchar.h>

namespace base {

inline int vsnprintf(char *buffer, size_t size, const char *format,
                     ::va_list arguments) {
  int length = _vsprintf_p(buffer, size, format, arguments);
  if (length < 0) {
    if (size > 0)
      buffer[0] = 0;
    return _vscprintf_p(format, arguments);
  }
  return length;
}

inline int vswprintf(wchar_t *buffer, size_t size, const wchar_t *format,
                     ::va_list arguments) {
  int length = _vswprintf_p(buffer, size, format, arguments);
  if (length < 0) {
    if (size > 0)
      buffer[0] = 0;
    return _vscwprintf_p(format, arguments);
  }
  return length;
}

} // namespace base
