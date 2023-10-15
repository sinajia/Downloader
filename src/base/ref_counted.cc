// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "StdAfx.h"
#include "ref_counted.h"

namespace base {

namespace subtle {

RefCountedThreadSafeBase::RefCountedThreadSafeBase() : ref_count_(0) {}

RefCountedThreadSafeBase::~RefCountedThreadSafeBase() {}

void RefCountedThreadSafeBase::AddRef() const {
  InterlockedExchangeAdd(reinterpret_cast<volatile LONG *>(&ref_count_),
                         static_cast<LONG>(1));
}

bool RefCountedThreadSafeBase::Release() const {

  if (0 ==
      (InterlockedExchangeAdd(reinterpret_cast<volatile LONG *>(&ref_count_),
                              static_cast<LONG>(-1)) - 1)) {
    return true;
  }
  return false;
}

bool RefCountedThreadSafeBase::HasOneRef() const { return (ref_count_ == 1); }

} // namespace subtle

} // namespace base
