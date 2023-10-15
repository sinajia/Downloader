#ifndef BASE_MEMORY_ALIGNED_MEMORY_H_
#define BASE_MEMORY_ALIGNED_MEMORY_H_

#include <malloc.h>

namespace base {
// AlignedMemory is specialized for all supported alignments.
// Make sure we get a compiler error if someone uses an unsupported alignment.
template <size_t Size, size_t ByteAlignment> struct AlignedMemory {};

#define BASE_DECL_ALIGNED_MEMORY(byte_alignment)                               \
  template <size_t Size> class AlignedMemory<Size, byte_alignment> {           \
  public:                                                                      \
    __declspec(align(byte_alignment)) unsigned char data_[Size];               \
    void *void_data() { return static_cast<void *>(data_); }                   \
    const void *void_data() const { return static_cast<const void *>(data_); } \
    template <typename Type> Type *data_as() {                                 \
      return static_cast<Type *>(void_data());                                 \
    }                                                                          \
    template <typename Type> const Type *data_as() const {                     \
      return static_cast<const Type *>(void_data());                           \
    }                                                                          \
                                                                               \
  private:                                                                     \
    void *operator new(size_t);                                                \
    void operator delete(void *);                                              \
  }

// Specialization for all alignments is required because MSVC (as of VS 2008)
// does not understand ALIGNAS(ALIGNOF(Type)) or ALIGNAS(template_param).
// Greater than 4096 alignment is not supported by some compilers, so 4096 is
// the maximum specified here.
BASE_DECL_ALIGNED_MEMORY(1);
BASE_DECL_ALIGNED_MEMORY(2);
BASE_DECL_ALIGNED_MEMORY(4);
BASE_DECL_ALIGNED_MEMORY(8);
BASE_DECL_ALIGNED_MEMORY(16);
BASE_DECL_ALIGNED_MEMORY(32);
BASE_DECL_ALIGNED_MEMORY(64);
BASE_DECL_ALIGNED_MEMORY(128);
BASE_DECL_ALIGNED_MEMORY(256);
BASE_DECL_ALIGNED_MEMORY(512);
BASE_DECL_ALIGNED_MEMORY(1024);
BASE_DECL_ALIGNED_MEMORY(2048);
BASE_DECL_ALIGNED_MEMORY(4096);

} // namespace base

#endif // BASE_MEMORY_ALIGNED_MEMORY_H_
