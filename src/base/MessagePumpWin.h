#pragma once

#include <windows.h>
#include "MessagePump.h"
#include "macros.h"
#include "observer_list.h"
#include "scoped_handle.h"

namespace base {

class MessagePumpWin : public MessagePump {
public:
  class __declspec(novtable) MessagePumpObserver {
  public:
    // This method is called before processing a NativeEvent. If the
    // method returns EVENT_HANDLED, it indicates the event has already
    // been handled, so the event is not processed any farther. If the
    // method returns EVENT_CONTINUE, the event dispatching proceeds as
    // normal.
    virtual void WillProcessEvent(const MSG &event) = 0;

    // This method is called after processing a message. This method
    // will not be called if WillProcessEvent returns EVENT_HANDLED.
    virtual void DidProcessEvent(const MSG &event) = 0;

  protected:
    virtual ~MessagePumpObserver() {}
  };

  MessagePumpWin() : keep_running_(true), have_work_(0) {}
  virtual ~MessagePumpWin() {}

  // Add an Observer, which will start receiving notifications immediately.
  void AddObserver(MessagePumpObserver *observer);

  // Remove an Observer.  It is safe to call this method while an Observer is
  // receiving a notification callback.
  void RemoveObserver(MessagePumpObserver *observer);

  // Give a chance to code processing additional messages to notify the
  // message loop observers that another message has been processed.
  void WillProcessMessage(const MSG &msg);
  void DidProcessMessage(const MSG &msg);

  // MessagePump method:
  virtual void Quit() override;

  virtual HANDLE io_port() { return NULL; }

protected:
  int GetCurrentDelay() const;

  ObserverList<MessagePumpObserver> observers_;

  TimeTicks delayed_work_time_;

  bool keep_running_;

  long have_work_;
};

class MessagePumpForUI : public MessagePumpWin {
public:
  MessagePumpForUI(Delegate *delegate = NULL, MessagePumpForUI *upLayer = NULL);
  virtual ~MessagePumpForUI();
  // MessagePump methods:
  virtual void Run(Delegate *delegate) override;
  virtual void ScheduleWork() override;
  virtual void ScheduleDelayedWork(const TimeTicks &delayed_work_time) override;
  Delegate *PumpDelegate() { return delegate_; }
  bool ProcessNextWindowsMessage();
  void WaitForWork();
  void upPumpLayer(MessagePumpForUI *upLayer) { upLayer_ = upLayer; }

private:
  static LRESULT CALLBACK WndProcThunk(HWND window_handle, UINT message,
                                       WPARAM wparam, LPARAM lparam);

  void InitMessageWnd();
  bool ProcessMessageHelper(const MSG &msg);
  void HandleWorkMessage();
  void HandleTimerMessage();

  // Atom representing the registered window class.
  ATOM atom_;

  // A hidden message-only window.
  HWND message_hwnd_;

  Delegate *delegate_;

  MessagePumpForUI *upLayer_;

  DISALLOW_COPY_AND_ASSIGN(MessagePumpForUI);
};

class MessagePumpForIO : public MessagePumpWin {
public:
  struct IOContext;

  class IOHandler {
  public:
    virtual ~IOHandler() {}

    virtual void OnIOCompleted(IOContext *context, DWORD bytes_transfered,
                               DWORD error) = 0;
  };

  struct IOContext {
    OVERLAPPED overlapped;
    IOHandler *handler;
  };

  MessagePumpForIO(Delegate *delegate = NULL);
  ~MessagePumpForIO();
  // MessagePump methods:
  virtual void Run(Delegate *delegate) override;
  virtual void Quit() override;
  virtual void ScheduleWork() override;
  virtual void ScheduleDelayedWork(const TimeTicks &delayed_work_time) override;

  void RegisterIOHandler(HANDLE file_handle, IOHandler *handler);

  virtual HANDLE io_port() override { return port_.Get(); }

private:
  struct IOItem {
    IOHandler *handler;
    IOContext *context;
    DWORD bytes_transfered;
    DWORD error;

    // In some cases |context| can be a non-pointer value casted to a pointer.
    // |has_valid_io_context| is true if |context| is a valid IOContext
    // pointer, and false otherwise.
    bool has_valid_io_context;
  };

  bool WaitForIOCompletion(DWORD timeout, IOHandler *filter);
  void WaitForWork();
  bool GetIOItem(DWORD timeout, IOItem *item);
  bool ProcessInternalIOItem(const IOItem &item);

  // static
  //  Converts an IOHandler pointer to a completion port key.
  //  |has_valid_io_context| specifies whether completion packets posted to
  //  |handler| will have valid OVERLAPPED pointers.
  static ULONG_PTR HandlerToKey(IOHandler *handler, bool has_valid_io_context);

  // Converts a completion port key to an IOHandler pointer.
  static IOHandler *KeyToHandler(ULONG_PTR key, bool *has_valid_io_context);

  std::list<IOItem> cmpleted_io_;

  ScopedWinHandle<HandleTraits> port_;

  Delegate *delegate_;
};

} // namespace base
