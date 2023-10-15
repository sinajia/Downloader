#include "StdAfx.h"
#include "stringprintf.h"
#include "MessagePumpWin.h"

namespace base {
static const wchar_t kWndClassFormat[] = L"Chrome_MessagePumpWindow_%p";
static const int kMsgHaveWork = WM_USER + 1;

HMODULE GetModuleFromAddress(void *address) {
  HMODULE instance = NULL;
  if (!::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            static_cast<char *>(address), &instance)) {
    assert(0);
  }
  return instance;
}
// Win
void MessagePumpWin::AddObserver(MessagePumpObserver *observer) {
  observers_.AddObserver(observer);
}

void MessagePumpWin::RemoveObserver(MessagePumpObserver *observer) {
  observers_.RemoveObserver(observer);
}

void MessagePumpWin::WillProcessMessage(const MSG &msg) {
  FOR_EACH_OBSERVER(MessagePumpObserver, observers_, WillProcessEvent(msg));
}

void MessagePumpWin::DidProcessMessage(const MSG &msg) {
  FOR_EACH_OBSERVER(MessagePumpObserver, observers_, DidProcessEvent(msg));
}

void MessagePumpWin::Quit() { keep_running_ = false; }

int MessagePumpWin::GetCurrentDelay() const {
  if (delayed_work_time_.is_null())
    return -1;

  // Be careful here.  TimeDelta has a precision of microseconds, but we want a
  // value in milliseconds.  If there are 5.5ms left, should the delay be 5 or
  // 6?  It should be 6 to avoid executing delayed work too early.
  double timeout =
      ceil((delayed_work_time_ - TimeTicks::Now()).InMillisecondsF());

  // If this value is negative, then we need to run delayed work soon.
  int delay = static_cast<int>(timeout);
  if (delay < 0)
    delay = 0;

  return delay;
}

// UI
MessagePumpForUI::MessagePumpForUI(Delegate *delegate,
                                   MessagePumpForUI *upLayer)
    : atom_(0), message_hwnd_(NULL), delegate_(delegate), upLayer_(upLayer) {
  if (!upLayer_)
    InitMessageWnd();
}

MessagePumpForUI::~MessagePumpForUI() {
  DestroyWindow(message_hwnd_);
  UnregisterClass(MAKEINTATOM(atom_), GetModuleFromAddress(&WndProcThunk));
}

void MessagePumpForUI::Run(Delegate *delegate) {
  assert(delegate_ == NULL || delegate == delegate_);
  delegate_ = delegate;

  if (!upLayer_) {
    // for module dialog
    assert(delegate_);
    for (;;) {
      // If we do any work, we may create more messages etc., and more work may
      // possibly be waiting in another task group.  When we (for example)
      // ProcessNextWindowsMessage(), there is a good chance there are still
      // more messages waiting.  On the other hand, when any of these methods
      // return having done no work, then it is pretty unlikely that calling
      // them again quickly will find any work to do.  Finally, if they all say
      // they had no work, then it is a good time to consider sleeping (waiting)
      // for more work.

      bool more_work_is_plausible = ProcessNextWindowsMessage();
      if (!keep_running_)
        break;

      more_work_is_plausible |= delegate->DoWork();

      if (!keep_running_)
        break;

      more_work_is_plausible |= delegate->DoDelayedWork(&delayed_work_time_);

      if (!keep_running_)
        break;

      if (more_work_is_plausible)
        continue;

      more_work_is_plausible = delegate->DoIdleWork();
      if (!keep_running_)
        break;

      if (more_work_is_plausible)
        continue;

      WaitForWork(); // Wait (sleep) until we have work to do again.
    }
  } else {
    assert(!delegate_);
    for (;;) {
      bool more_work_is_plausible = upLayer_->ProcessNextWindowsMessage();
      if (!keep_running_)
        break;

      more_work_is_plausible |= upLayer_->PumpDelegate()->DoWork();

      if (!keep_running_)
        break;

      more_work_is_plausible |=
          upLayer_->PumpDelegate()->DoDelayedWork(&delayed_work_time_);

      if (!keep_running_)
        break;

      if (more_work_is_plausible)
        continue;

      more_work_is_plausible = upLayer_->PumpDelegate()->DoIdleWork();
      if (!keep_running_)
        break;

      if (more_work_is_plausible)
        continue;

      upLayer_->WaitForWork(); // Wait (sleep) until we have work to do again.
    }
  }
  keep_running_ = true;
}

void MessagePumpForUI::ScheduleWork() {
  if (InterlockedExchange(&have_work_, 1))
    return;

  BOOL ret = PostMessage(message_hwnd_, kMsgHaveWork, (WPARAM)this, 0);

  if (ret)
    return; // There was room in the Window Message queue.

  InterlockedExchange(&have_work_, 0);
}

void MessagePumpForUI::ScheduleDelayedWork(const TimeTicks &delayed_work_time) {
  delayed_work_time_ = delayed_work_time;

  int delay_msec = GetCurrentDelay();

  assert(delay_msec >= 0);

  if (delay_msec < USER_TIMER_MINIMUM)
    delay_msec = USER_TIMER_MINIMUM;

  KillTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this));
  BOOL ret = SetTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this),
                      delay_msec, NULL);
}

void MessagePumpForUI::InitMessageWnd() {
  std::wstring class_name = base::StringPrintf(kWndClassFormat, this);
  HINSTANCE instance = GetModuleFromAddress(&WndProcThunk);
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = WndProcThunk;
  wc.hInstance = instance;
  wc.lpszClassName = class_name.c_str();
  atom_ = RegisterClassEx(&wc);
  assert(atom_);
  message_hwnd_ = ::CreateWindow(MAKEINTATOM(atom_), 0, 0, 0, 0, 0, 0,
                                 HWND_MESSAGE, 0, instance, 0);
  assert(message_hwnd_);
}

bool MessagePumpForUI::ProcessMessageHelper(const MSG &msg) {
  if (WM_QUIT == msg.message) {
    // Repost the QUIT message so that it will be retrieved by the primary
    // GetMessage() loop.
    keep_running_ = false;
    PostQuitMessage(static_cast<int>(msg.wParam));
    return false;
  }

  WillProcessMessage(msg);

  TranslateMessage(&msg);
  DispatchMessage(&msg);

  DidProcessMessage(msg);
  return true;
}

bool MessagePumpForUI::ProcessNextWindowsMessage() {
  // If there are sent messages in the queue then PeekMessage internally
  // dispatches the message and returns false. We return true in this
  // case to ensure that the message loop peeks again instead of calling
  // MsgWaitForMultipleObjectsEx again.
  bool sent_messages_in_queue = false;
  DWORD queue_status = GetQueueStatus(QS_SENDMESSAGE);
  if (HIWORD(queue_status) & QS_SENDMESSAGE)
    sent_messages_in_queue = true;

  MSG msg;
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != FALSE)
    return ProcessMessageHelper(msg);

  return sent_messages_in_queue;
}
// static
LRESULT CALLBACK MessagePumpForUI::WndProcThunk(HWND hwnd, UINT message,
                                                WPARAM wparam, LPARAM lparam) {
  switch (message) {
  case kMsgHaveWork:
    reinterpret_cast<MessagePumpForUI *>(wparam)->HandleWorkMessage();
    break;
  case WM_TIMER:
    reinterpret_cast<MessagePumpForUI *>(wparam)->HandleTimerMessage();
    break;
  }
  return DefWindowProc(hwnd, message, wparam, lparam);
}

void MessagePumpForUI::WaitForWork() {
  int delay = GetCurrentDelay();
  if (delay < 0) // Negative value means no timers waiting.
    delay = INFINITE;

  DWORD result;
  result = MsgWaitForMultipleObjectsEx(0, NULL, delay, QS_ALLINPUT,
                                       MWMO_INPUTAVAILABLE);

  if (WAIT_OBJECT_0 == result) {
    // A WM_* message is available.
    // If a parent child relationship exists between windows across threads
    // then their thread inputs are implicitly attached.
    // This causes the MsgWaitForMultipleObjectsEx API to return indicating
    // that messages are ready for processing (Specifically, mouse messages
    // intended for the child window may appear if the child window has
    // capture).
    // The subsequent PeekMessages call may fail to return any messages thus
    // causing us to enter a tight loop at times.
    // The WaitMessage call below is a workaround to give the child window
    // some time to process its input messages.
    MSG msg = {0};
    DWORD queue_status = GetQueueStatus(QS_MOUSE);
    if (HIWORD(queue_status) & QS_MOUSE &&
        !PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) {
      WaitMessage();
    }
    return;
  }
}

void MessagePumpForUI::HandleWorkMessage() {
  InterlockedExchange(&have_work_, 0);
  delegate_->DoWork();
}

void MessagePumpForUI::HandleTimerMessage() {
  KillTimer(message_hwnd_, reinterpret_cast<UINT_PTR>(this));

  delegate_->DoDelayedWork(&delayed_work_time_);
  if (!delayed_work_time_.is_null()) {
    ScheduleDelayedWork(delayed_work_time_);
  }
}

// IO
MessagePumpForIO::MessagePumpForIO(Delegate *delegate) : delegate_(delegate) {
  port_.Set(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0));
  assert(port_.IsValid());
}

MessagePumpForIO::~MessagePumpForIO() {}

void MessagePumpForIO::Run(Delegate *delegate) {
  assert(delegate_ == NULL || delegate == delegate_);
  delegate_ = delegate;

  for (;;) {
    bool more_work_is_plausible = delegate->DoWork();
    if (!keep_running_)
      break;

    more_work_is_plausible |= WaitForIOCompletion(0, NULL);
    if (!keep_running_)
      break;

    more_work_is_plausible |= delegate->DoDelayedWork(&delayed_work_time_);
    if (!keep_running_)
      break;

    if (more_work_is_plausible)
      continue;

    more_work_is_plausible = delegate->DoIdleWork();
    if (!keep_running_)
      break;

    if (more_work_is_plausible)
      continue;

    WaitForWork();
  }
}

void MessagePumpForIO::ScheduleWork() {
  if (InterlockedExchange(&have_work_, 1))
    return; // Someone else continued the pumping.

  BOOL ret = PostQueuedCompletionStatus(port_.Get(), 0,
                                        reinterpret_cast<ULONG_PTR>(this),
                                        reinterpret_cast<OVERLAPPED *>(this));

  if (ret)
    return;

  InterlockedExchange(&have_work_, 0);
}

void MessagePumpForIO::ScheduleDelayedWork(const TimeTicks &delayed_work_time) {
  delayed_work_time_ = delayed_work_time;
}

void MessagePumpForIO::Quit() { keep_running_ = false; }

void MessagePumpForIO::RegisterIOHandler(HANDLE file_handle,
                                         IOHandler *handler) {
  ULONG_PTR key = HandlerToKey(handler, true);
  HANDLE port = CreateIoCompletionPort(file_handle, port_, key, 0);
  assert(port_.Get() == port);
}

bool MessagePumpForIO::WaitForIOCompletion(DWORD timeout, IOHandler *filter) {
  IOItem item;
  if (!GetIOItem(timeout, &item))
    return false;

  if (ProcessInternalIOItem(item))
    return true;

  assert(item.context->handler == item.handler);
  item.handler->OnIOCompleted(item.context, item.bytes_transfered, item.error);

  return true;
}

void MessagePumpForIO::WaitForWork() {
  int timeout = GetCurrentDelay();
  if (timeout < 0) {
    timeout = INFINITE;
  }

  WaitForIOCompletion(timeout, NULL);
}

bool MessagePumpForIO::GetIOItem(DWORD timeout, IOItem *item) {
  memset(item, 0, sizeof(*item));
  ULONG_PTR key = NULL;
  OVERLAPPED *overlapped = NULL;
  if (!GetQueuedCompletionStatus(port_.Get(), &item->bytes_transfered, &key,
                                 &overlapped, timeout)) {
    if (!overlapped)
      return false; // Nothing in the queue.

    item->error = GetLastError();
    item->bytes_transfered = 0;
  }

  item->handler = KeyToHandler(key, &item->has_valid_io_context);
  item->context = reinterpret_cast<IOContext *>(overlapped);
  return true;
}

bool MessagePumpForIO::ProcessInternalIOItem(const IOItem &item) {
  if (this == reinterpret_cast<MessagePumpForIO *>(item.context) &&
      this == reinterpret_cast<MessagePumpForIO *>(item.handler)) {
    // This is our internal completion.
    assert(!item.bytes_transfered);
    InterlockedExchange(&have_work_, 0);
    return true;
  }
  return false;
}

// static
ULONG_PTR MessagePumpForIO::HandlerToKey(IOHandler *handler,
                                         bool has_valid_io_context) {
  ULONG_PTR key = reinterpret_cast<ULONG_PTR>(handler);

  // |IOHandler| is at least pointer-size aligned, so the lowest two bits are
  // always cleared. We use the lowest bit to distinguish completion keys with
  // and without the associated |IOContext|.
  assert((key & 1) == 0);

  // Mark the completion key as context-less.
  if (!has_valid_io_context)
    key = key | 1;
  return key;
}

// static
MessagePumpForIO::IOHandler *
MessagePumpForIO::KeyToHandler(ULONG_PTR key, bool *has_valid_io_context) {
  *has_valid_io_context = ((key & 1) == 0);
  return reinterpret_cast<IOHandler *>(key & ~static_cast<ULONG_PTR>(1));
}

} // namespace base
