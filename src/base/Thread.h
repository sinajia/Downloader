#pragma once

#include <string>
#include "../Event/WaitableEvent.h"
#include "MessageLoop.h"

namespace base {
class Thread final {
public:
  typedef bool (*ThreadContextInit)(void *);

  static const DWORD kInvalidThreadId = 0;

  class Options {
  public:
    Options();
    Options(MessageLoop::Type type, size_t size = 0);

    size_t stack_size_;
    MessageLoop::Type message_loop_type_;
  };
  enum ThreadPriority {
    kThreadPriority_Normal,
    // Suitable for low-latency, glitch-resistant audio.
    kThreadPriority_RealtimeAudio,
    // Suitable for threads which generate data for the display (at ~60Hz).
    kThreadPriority_Display,
    // Suitable for threads that shouldn't disrupt high priority work.
    kThreadPriority_Background
  };

  enum ComStatus {
    NONE,
    STA,
    MTA,
  };
  class ScopedCOMInitializer {
  public:
    ScopedCOMInitializer() : thread_id_(Thread::kInvalidThreadId) {
      Initialize(COINIT_APARTMENTTHREADED);
    }
    explicit ScopedCOMInitializer(ComStatus mta)
        : thread_id_(Thread::kInvalidThreadId) {
      Initialize(COINIT_MULTITHREADED);
    }
    ~ScopedCOMInitializer() {
      assert(GetCurrentThreadId() == thread_id_);
      CoUninitialize();
    }

  private:
    void Initialize(COINIT init) {
      thread_id_ = GetCurrentThreadId();
      CoInitializeEx(NULL, init);
    }
    // tid
    DWORD thread_id_;
  };

  class StartupData {
  public:
    const Options &options_;
    WaitableEvent event_;

    explicit StartupData(const Options &options)
        : options_(options), event_(false, false) {}
  };

  class ThreadParams {
  public:
    Thread *thread_;
  };
  explicit Thread();
  explicit Thread(const char *name);
  virtual ~Thread();

  bool StartWithOptions(const Options &options);
  void Stop();
  MessageLoop *message_loop() const { return message_loop_; }
  const std::string &thread_name() const { return name_; }
  bool set_thread_name(std::string name);
  HANDLE thread_handle() { return thread_; }
  DWORD thread_id() { return thread_id_; }
  bool IsRunning() const;
  void SetPriority(ThreadPriority priority);

  void init_com_with_mta(bool use_mta) {
    assert(!started_);
    com_status_ = use_mta ? MTA : STA;
  }

  virtual void ThreadMain();

  // static
  static bool CreateThreadInternal(size_t stack_size, Thread *thread,
                                   HANDLE *out_thread_handle);

protected:
  virtual void CleanUp() {}

private:
  void Run(MessageLoop *message_loop);

  MessageLoop *message_loop_;
  HANDLE thread_;
  std::string name_;
  DWORD thread_id_;
  // Whether we successfully started the thread.
  bool started_;

  // If true, we're in the middle of stopping, and shouldn't access
  // |message_loop_|. It may non-NULL and invalid.
  bool stopping_;

  // True while inside of Run().
  bool running_;

  ComStatus com_status_;

  StartupData *startup_data_;

  ThreadContextInit thread_context_;

  DISALLOW_COPY_AND_ASSIGN(Thread);
};
} // namespace base
