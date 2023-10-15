#pragma once

namespace base {

class NotificationDetails;
class NotificationSource;

// This is the base class for notification observers. When a matching
// notification is posted to the notification service, Observe is called.
class NotificationObserver {
public:
  virtual void Observe(int type, const NotificationSource &source,
                       const NotificationDetails &details) = 0;

protected:
  virtual ~NotificationObserver() {}
};

} // namespace base
