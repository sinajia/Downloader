#pragma once

namespace base {

enum NotificationType {

  NOTIFICATION_START = 0,

  // Special signal value to represent an interest in all notifications.
  // Not valid when posting a notification.
  NOTIFICATION_ALL = NOTIFICATION_START,

  // Add your code here

  // Custom notifications used by the embedder should start from here.
  NOTIFICATION_END,
};

} // namespace base
