#include "StdAfx.h"
#include "lazy_instance.h"

namespace base {
void CompleteLazyInstance(intptr_t *state, intptr_t new_instance,
                          void *lazy_instance, void (*dtor)(void *)) {

  *state = new_instance;

  // Make sure that the lazily instantiated object will get destroyed at exit.
  if (dtor)
    AtExitManager::RegisterCallback(dtor, lazy_instance);
}
} // namespace base
