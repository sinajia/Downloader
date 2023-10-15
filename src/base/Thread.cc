#include "StdAfx.h"
#include "Thread.h"

namespace base {
unsigned int __stdcall ThreadFunc(void *params);

Thread::Options::Options()
    : message_loop_type_(MessageLoop::TYPE_DEFAULT), stack_size_(0) {}
Thread::Options::Options(MessageLoop::Type type, size_t size)
    : message_loop_type_(type), stack_size_(size) {}

Thread::Thread()
    : com_status_(NONE), started_(false), stopping_(false), running_(false),
      thread_(NULL), message_loop_(NULL), thread_id_(kInvalidThreadId),
      startup_data_(NULL), thread_context_(NULL) {}

Thread::Thread(const char *name)
    : com_status_(NONE), started_(false), stopping_(false), running_(false),
      thread_(NULL), message_loop_(NULL), name_(name),
      thread_id_(kInvalidThreadId), startup_data_(NULL), thread_context_(NULL) {

}

Thread::~Thread() { Stop(); }

bool Thread::StartWithOptions(const Options &options) {
  assert(!message_loop_);
  StartupData startup_data(options);
  startup_data_ = &startup_data;
  if (!CreateThreadInternal(options.stack_size_, this, &thread_)) {
    startup_data_ = NULL;
    return false;
  }
  startup_data.event_.Wait();
  startup_data_ = NULL;
  started_ = true;
  assert(message_loop_);
  return true;
}

bool Thread::CreateThreadInternal(size_t stack_size, Thread *thread,
                                  HANDLE *out_thread_handle) {
  unsigned int flags = 0;
  if (stack_size > 0)
    flags = STACK_SIZE_PARAM_IS_A_RESERVATION;

  Thread::ThreadParams *params = new Thread::ThreadParams;
  params->thread_ = thread;

  HANDLE thread_handle =
      (HANDLE)_beginthreadex(NULL, stack_size, ThreadFunc, params, flags, NULL);
  if (NULL == thread_handle) {
    delete params;
    return false;
  }
  if (out_thread_handle) {
    *out_thread_handle = thread_handle;
  } else {
    CloseHandle(thread_handle);
  }
  return true;
}

void Thread::ThreadMain() {
  std::auto_ptr<MessageLoop> message_loop;
  message_loop.reset(
      new MessageLoop(startup_data_->options_.message_loop_type_));
  assert(message_loop.get());

  thread_id_ = GetCurrentThreadId();
  message_loop->set_thread_name(name_);
  message_loop_ = message_loop.get();

  std::auto_ptr<ScopedCOMInitializer> com_initializer;
  if (com_status_ != NONE) {
    com_initializer.reset((com_status_ == STA) ? new ScopedCOMInitializer()
                                               : new ScopedCOMInitializer(MTA));
  }
  running_ = true;
  startup_data_->event_.Signal();
  Run(message_loop_);
  running_ = false;

  // Let the thread do extra cleanup.
  CleanUp();
  com_initializer.reset();

  message_loop_ = NULL;
}

void Thread::Run(MessageLoop *message_loop) { message_loop->Run(); }

bool Thread::IsRunning() const { return running_; }

void Thread::SetPriority(ThreadPriority priority) {
  switch (priority) {
  case kThreadPriority_Normal:
    ::SetThreadPriority(thread_, THREAD_PRIORITY_NORMAL);
    break;
  case kThreadPriority_RealtimeAudio:
    ::SetThreadPriority(thread_, THREAD_PRIORITY_TIME_CRITICAL);
    break;
  default:
    break;
  }
}

void Thread::Stop() {
  if (!started_)
    return;

  if (stopping_ || !message_loop_)
    return;

  stopping_ = true;

  message_loop_->Quit();

  HANDLE thread_handle = thread_;
  DWORD result = WaitForSingleObject(thread_handle, INFINITE);
  if (result != WAIT_OBJECT_0) {
    assert(0);
  }
  CloseHandle(thread_handle);
  assert(!message_loop_);
  started_ = false;
  stopping_ = false;
}

bool Thread::set_thread_name(std::string name) {
  if (name_.empty()) {
    name_ = name;
    return true;
  }
  return false;
}

} // namespace base

namespace base {

unsigned int __stdcall ThreadFunc(void *params) {
  Thread::ThreadParams *thread_params =
      static_cast<Thread::ThreadParams *>(params);
  base::Thread *thread = thread_params->thread_;
  delete thread_params;

  thread->ThreadMain();

  return NULL;
}

} // namespace base
