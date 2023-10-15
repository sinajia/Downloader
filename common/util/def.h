#pragma once

#define WM_DEL_GDI_OBJ WM_USER + 0x10E0C
#define WM_IS_EXIST WM_USER + 0x10E0D

namespace app {
enum NotificationType {
  NOTIFICATION_APP_START = 0,
  NOTIFICATION_APP_DOWNLOAD_CANCAL,
};

}

extern UINT WM_TASKBARBUTTONCREATED;
