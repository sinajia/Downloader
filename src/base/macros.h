#pragma once

#define DISALLOW_COPY_AND_ASSIGN(TypeName)        \
    TypeName(const TypeName &);                   \
    TypeName(const TypeName &&);                  \
    void operator=(const TypeName&);

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName)  \
  TypeName();                                     \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

template <typename T, size_t N> char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#if defined(COMPILER_GCC)
#define GG_VA_COPY(a, b) (va_copy(a, b))
#elif defined(COMPILER_MSVC)
#define GG_VA_COPY(a, b) (a = b)
#endif

#define DCHECK_GE(a, b) assert((a) >= (b));
#define DCHECK_LE(a, b) assert((a) <= (b));
#define DCHECK_NE(a, b) assert((a) != (b));
#define DCHECK_EQ(a, b) assert((a) == (b));
#define CHECK(x) assert(x);
