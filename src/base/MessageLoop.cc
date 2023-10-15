#include "StdAfx.h"
#include "MessageLoop.h"

namespace base {
static base::ThreadLocalPointer<MessageLoop> tls_ptr;

void MessageLoop::AddDestructionObserver(
    DestructionObserver *destruction_observer) {
  assert(this == current());
  destruction_observers_.AddObserver(destruction_observer);
}

void MessageLoop::RemoveDestructionObserver(
    DestructionObserver *destruction_observer) {
  assert(this == current());
  destruction_observers_.RemoveObserver(destruction_observer);
}

MessageLoop::MessageLoop(Type type)
    : type_(type), running_(false), recent_time_() /*,upLayer_(NULL)*/ {
  Init();
  pump_.reset(CreateMessagePumpForType(type));
}

MessageLoop *MessageLoop::current() { return tls_ptr.Get(); }

void MessageLoop::Init() {
  assert(tls_ptr.Get() == NULL);
  tls_ptr.Set(this);
}

void MessageLoop::PostTask(fastdelegate::Task<void> *task) {
  PostDelayedTask(task, TimeDelta());
}

void MessageLoop::PostDelayedTask(fastdelegate::Task<void> *task,
                                  TimeDelta delay) {
  assert(delay >= TimeDelta());

  do {
    if (delay == TimeDelta()) {
      AppendTask(PendingTask(task));
      break;
    }
    AppendTask(PendingTask(task, CalculateDelayedRuntime(delay)));

  } while (false);

  pump_->ScheduleWork();
}

void MessageLoop::PostQuitTask(fastdelegate::Task<void> *task) {
  AutoCritSecLock<CriticalSection> lock(m_cs, false);
  lock.Lock();
  quitwork_queue_.push_back(PendingTask(task));
}

void MessageLoop::Run() {
  running_ = true;
  pump_->Run(this);
  running_ = false;
}

void MessageLoop::Quit() {
  PostTask(Bind(Unretained(this), &MessageLoop::QuitInternal));
}

bool MessageLoop::is_running() const { return running_; }

MessagePump *MessageLoop::CreateMessagePumpForType(Type type) {
  if (MessageLoop::TYPE_UI == type) {
    return new MessagePumpForUI();
  }
  if (MessageLoop::TYPE_IO == type) {
    return new MessagePumpForIO();
  }
  if (MessageLoop::TYPE_DEFAULT == type) {
    return new MessagePumpDefault();
  }
  return NULL;
}

void MessageLoop::AppendTask(PendingTask &task) {
  AutoCritSecLock<CriticalSection> lock(m_cs, false);
  lock.Lock();
  incoming_queue_.push_back(task);
}

void MessageLoop::AddToDelayedWorkQueue(const PendingTask &pending_task) {
  delayed_work_queue_.push(pending_task);
}

void MessageLoop::QuitInternal() { pump_->Quit(); }

MessageLoop::~MessageLoop() {
  assert(this == current());
  // Clean up any unprocessed tasks
  DeletePendingTasks();
  ReloadWorkQueue();
  DeletePendingTasks();
  // Let interested parties have one last shot at accessing this.
  FOR_EACH_OBSERVER(DestructionObserver, destruction_observers_,
                    WillDestroyCurrentMessageLoop());
  // OK, now make it so that no one can find us.
  tls_ptr.Set(NULL);

  ExeQuitQueue();
}

void MessageLoop::ExeQuitQueue() {
  for (; !quitwork_queue_.empty();) {
    PendingTask pending_task = quitwork_queue_.pick_front();
    RunPendingTask(pending_task);
  }
}

void MessageLoop::DeletePendingTasks() {
  size_t size = working_queue_.size();
  for (size_t i = 0; i < size; ++i) {
    PendingTask pending_task = working_queue_.pick_front();
    pending_task.Reset();
  }
  while (!delayed_work_queue_.empty()) {
    PendingTask pending_task = delayed_work_queue_.top();
    delayed_work_queue_.pop();
    pending_task.Reset();
  }
}

void MessageLoop::ReloadWorkQueue() {
  if (!working_queue_.empty())
    return;

  AutoCritSecLock<CriticalSection> lock(m_cs, false);
  lock.Lock();
  if (0 == incoming_queue_.size()) {
  } else {
    incoming_queue_.swap(working_queue_);
  }
}

bool MessageLoop::RunPendingTask(PendingTask &pending_task) {
  pending_task.Run();
  return true;
}

TimeTicks MessageLoop::CalculateDelayedRuntime(TimeDelta delay) {
  TimeTicks delayed_run_time = TimeTicks::Now() + delay;
  return delayed_run_time;
}

bool MessageLoop::DoWork() {
  for (;;) {
    ReloadWorkQueue();
    if (working_queue_.empty())
      break;

    // Execute oldest task.
    do {
      PendingTask pending_task = working_queue_.pick_front();
      if (!pending_task.delayed_run_time_.is_null()) {
        AddToDelayedWorkQueue(pending_task);
        // If we changed the topmost task, then it is time to reschedule.
        if (delayed_work_queue_.top().Equals(pending_task))
          pump_->ScheduleDelayedWork(pending_task.delayed_run_time_);
      } else {
        if (RunPendingTask(pending_task))
          return true;
      }
    } while (!working_queue_.empty());
  }
  // Nothing runned.
  return false;
}

bool MessageLoop::DoDelayedWork(TimeTicks *next_delayed_work_time) {
  if (delayed_work_queue_.empty()) {
    recent_time_ = *next_delayed_work_time = TimeTicks();
    return false;
  }

  TimeTicks next_run_time = delayed_work_queue_.top().delayed_run_time_;
  if (next_run_time > recent_time_) {
    recent_time_ = TimeTicks::Now();
    if (next_run_time > recent_time_) {
      *next_delayed_work_time = next_run_time;
      return false;
    }
  }

  PendingTask pending_task = delayed_work_queue_.top();
  delayed_work_queue_.pop();

  if (!delayed_work_queue_.empty()) {
    *next_delayed_work_time = delayed_work_queue_.top().delayed_run_time_;
  }

  RunPendingTask(pending_task);
  return true;
}

bool MessageLoop::DoIdleWork() { return false; }

MessagePumpWin *MessageLoop::pump_win() {
  return static_cast<MessagePumpWin *>(pump_.get());
}

// LayerLoop
std::vector<MessageLoop::LayerLoop *> MessageLoop::LayerLoop::layer_;

MessageLoop::LayerLoop::LayerLoop(Type type) {
  pump_.reset(new MessagePumpForUI(NULL, (MessagePumpForUI *)1));
}

MessagePumpWin *MessageLoop::LayerLoop::pump_win() {
  return static_cast<MessagePumpWin *>(pump_.get());
}

bool MessageLoop::LayerLoop::DoWork() { return false; }

bool MessageLoop::LayerLoop::DoDelayedWork(TimeTicks *next_delayed_work_time) {
  return false;
}

bool MessageLoop::LayerLoop::DoIdleWork() { return false; }

void MessageLoop::LayerLoop::Run() { pump_->Run(NULL); }

void MessageLoop::LayerLoop::PushBackLoop(LayerLoop *loop) {
  layer_.push_back(loop);
}

void MessageLoop::LayerLoop::PopBackLoop() { layer_.pop_back(); }

MessageLoop::LayerLoop *MessageLoop::LayerLoop::Back() {
  LayerLoop *back = layer_.back();
  return back;
}

} // namespace base
