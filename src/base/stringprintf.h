#pragma once

#include "string_util_win.h"

namespace base {
std::string  StringPrintf(const char *format, ...);
std::wstring StringPrintf(const wchar_t *format, ...);
void StringAppendV(std::string *dst, const char *format, ::va_list ap);
void StringAppendV(std::wstring *dst, const wchar_t *format, ::va_list ap);
} // namespace base
