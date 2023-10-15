#include "StdAfx.h"
#include "notification_registrar.h"

namespace base {

struct NotificationRegistrar::Record {
  bool operator==(const Record &other) const;

  NotificationObserver *observer;
  int type;
  NotificationSource source;
};

bool NotificationRegistrar::Record::operator==(const Record &other) const {
  return observer == other.observer && type == other.type &&
         source == other.source;
}

NotificationRegistrar::NotificationRegistrar() {
  // Force the NotificationService to be constructed (if it isn't already).
  // This ensures the NotificationService will be registered on the
  // AtExitManager before any objects which access it via NotificationRegistrar.
  // This in turn means it will be destroyed after these objects, so they will
  // never try to access the NotificationService after it's been destroyed.
  NotificationService::current();
}

NotificationRegistrar::~NotificationRegistrar() { RemoveAll(); }

void NotificationRegistrar::Add(NotificationObserver *observer, int type,
                                const NotificationSource &source) {
  Record record = {observer, type, source};
  registered_.push_back(record);

  NotificationService::current()->AddObserver(observer, type, source);
}

void NotificationRegistrar::Remove(NotificationObserver *observer, int type,
                                   const NotificationSource &source) {

  Record record = {observer, type, source};
  RecordVector::iterator found =
      std::find(registered_.begin(), registered_.end(), record);
  if (found == registered_.end()) {
    assert(0);
    //"Trying to remove unregistered observer of type "
    return;
  }
  registered_.erase(found);

  // This can be NULL if our owner outlives the NotificationService, e.g. if our
  // owner is a Singleton.
  NotificationService *service = NotificationService::current();
  if (service)
    service->RemoveObserver(observer, type, source);
}

void NotificationRegistrar::RemoveAll() {
  // Early-exit if no registrations, to avoid calling
  // NotificationService::current.  If we've constructed an object with a
  // NotificationRegistrar member, but haven't actually used the notification
  // service, and we reach prgram exit, then calling current() below could try
  // to initialize the service's lazy TLS pointer during exit, which throws
  // wrenches at things.
  if (registered_.empty())
    return;

  // This can be NULL if our owner outlives the NotificationService, e.g. if our
  // owner is a Singleton.
  NotificationService *service = NotificationService::current();
  if (service) {
    for (size_t i = 0; i < registered_.size(); i++) {
      service->RemoveObserver(registered_[i].observer, registered_[i].type,
                              registered_[i].source);
    }
  }
  registered_.clear();
}

bool NotificationRegistrar::IsEmpty() const { return registered_.empty(); }

bool NotificationRegistrar::IsRegistered(NotificationObserver *observer,
                                         int type,
                                         const NotificationSource &source) {
  Record record = {observer, type, source};
  return std::find(registered_.begin(), registered_.end(), record) !=
         registered_.end();
}

} // namespace base
