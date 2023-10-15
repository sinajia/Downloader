#pragma once

#include "macros.h"
#include "WrapperObj.h"
#include "ref_counted.h"

namespace base {
class WeakReference {
public:
  // Although Flag is bound to a specific thread, it may be deleted from another
  // via base::WeakPtr::~WeakPtr().
  class Flag : public RefCountedThreadSafe<Flag> {
  public:
    Flag() : is_valid_(true) {}

    void Invalidate() { is_valid_ = false; }
    bool IsValid() const { return is_valid_; }

  private:
    friend class base::RefCountedThreadSafe<Flag>;

    ~Flag();

    bool is_valid_;
  };

  WeakReference();
  explicit WeakReference(const Flag *flag);
  ~WeakReference();

  bool is_valid() const;

private:
  scoped_refptr<const Flag> flag_;
};

class WeakReferenceOwner {
public:
  WeakReferenceOwner();
  ~WeakReferenceOwner();

  WeakReference GetRef() const;

  bool HasRefs() const { return flag_.get() && !flag_->HasOneRef(); }

  void Invalidate();

private:
  mutable scoped_refptr<WeakReference::Flag> flag_;
};

template <typename T> class WeakPtr : public ObjWrapper<T> {
public:
  WeakPtr() : ptr_(NULL) {}
  explicit WeakPtr(WeakReference &ref, T *o);
  T *get() const override { return ref_.is_valid() ? ptr_ : NULL; }
  void AddRef() const override {}
  void Release() const override {}
  bool IsWeakPtr() const override;

  T &operator*() const {
    assert(get() != NULL);
    return *get();
  }
  T *operator->() const {
    assert(get() != NULL);
    return get();
  }

  WeakReference ref_;
  T *ptr_;
};

template <typename T>
WeakPtr<T>::WeakPtr(WeakReference &ref, T *o) : ref_(ref), ptr_(o) {}

template <typename T> bool WeakPtr<T>::IsWeakPtr() const { return true; }

// A class may extend from SupportsWeakPtr to let others take weak pointers to
// it. This avoids the class itself implementing boilerplate to dispense weak
// pointers.  However, since SupportsWeakPtr's destructor won't invalidate
// weak pointers to the class until after the derived class' members have been
// destroyed, its use can lead to subtle use-after-destroy issues.
template <class T> class SupportsWeakPtr {
public:
  SupportsWeakPtr() {}

  // for posttask
  WeakPtr<T> *BldWeakPtr() {
    return new WeakPtr<T>(weak_reference_owner_.GetRef(),
                          static_cast<T *>(this));
  }

  WeakPtr<T> AsWeakPtr() {
    return WeakPtr<T>(weak_reference_owner_.GetRef(), static_cast<T *>(this));
  }

protected:
  ~SupportsWeakPtr() {}

private:
  WeakReferenceOwner weak_reference_owner_;
  DISALLOW_COPY_AND_ASSIGN(SupportsWeakPtr);
};

} // end namespace base
