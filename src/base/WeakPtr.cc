#include "StdAfx.h"
#include "WeakPtr.h"

namespace base {
WeakReference::Flag::~Flag() {}

WeakReference::WeakReference() {}

WeakReference::WeakReference(const Flag *flag) : flag_(flag) {}

WeakReference::~WeakReference() {}

bool WeakReference::is_valid() const { return flag_.get() && flag_->IsValid(); }

WeakReferenceOwner::WeakReferenceOwner() {}

WeakReferenceOwner::~WeakReferenceOwner() { Invalidate(); }

WeakReference WeakReferenceOwner::GetRef() const {
  // If we hold the last reference to the Flag then create a new one.
  if (!HasRefs())
    flag_ = new WeakReference::Flag();

  return WeakReference(flag_.get());
}

void WeakReferenceOwner::Invalidate() {
  if (flag_.get()) {
    flag_->Invalidate();
    flag_ = NULL;
  }
}

} // end namespace base
