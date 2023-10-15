#include "StdAfx.h"
#include "PendingTask.h"

namespace base {
PendingTask::PendingTask() : task_(NULL), sequence_num_(0) {}

PendingTask::PendingTask(fastdelegate::Task<void> *task)
    : task_(task), sequence_num_(0) {}

PendingTask::PendingTask(fastdelegate::Task<void> *task,
                         const TimeTicks delayed_run_time)
    : task_(task), delayed_run_time_(delayed_run_time), sequence_num_(0) {}

PendingTask::~PendingTask() {}

bool PendingTask::is_null() const { return NULL == task_.get(); }

void PendingTask::Reset() { task_ = NULL; }

bool PendingTask::Equals(const PendingTask &other) const {
  return task_.get() == other.task_.get();
}

void PendingTask::Run() {
  if (task_.get()) {
    task_->Run();
  }
}

bool PendingTask::operator<(const PendingTask &other) const {
  // Since the top of a priority queue is defined as the "greatest" element, we
  // need to invert the comparison here.  We want the smaller time to be at the
  // top of the heap.

  if (delayed_run_time_ < other.delayed_run_time_)
    return false;

  if (delayed_run_time_ > other.delayed_run_time_)
    return true;

  // If the times happen to match, then we use the sequence number to decide.
  // Compare the difference to support integer roll-over.
  return (sequence_num_ > other.sequence_num_);
}

// task queue
namespace taskqueue {
bool queue::empty() { return _val.empty(); }

int queue::size() { return _val.size(); }

void queue::push_back(const PendingTask &val) {
  val.sequence_num_ = ++sequence_num_;
  _val.push_back(val);
}

PendingTask queue::pick_front() {
  assert(_val.size());
  PendingTask o = _val.front();
  _val.pop_front();
  return o;
}

PendingTask queue::front() {
  assert(_val.size());
  return _val.front();
}

void queue::swap(taskqueue::queue &val) { _val.swap(val._val); }

} // namespace taskqueue

} // namespace base
