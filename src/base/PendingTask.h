#pragma once

#include <queue>
#include <list>
#include "../time/time.h"
#include "FastDelegateImpl.h"
#include "ref_counted.h"

namespace base {
class PendingTask {
public:
  explicit PendingTask();
  explicit PendingTask(fastdelegate::Task<void> *task);
  explicit PendingTask(fastdelegate::Task<void> *task,
                       const TimeTicks delayed_run_time);
  ~PendingTask();

  bool is_null() const;

  void Reset();

  bool Equals(const PendingTask &other) const;

  void Run();
  // Used to support sorting.
  bool operator<(const PendingTask &other) const;
  // The time when the task should be run.
  base::TimeTicks delayed_run_time_;
  // Secondary sort key for run time.
  mutable unsigned int sequence_num_;

private:
  scoped_refptr<fastdelegate::Task<void> > task_;
};

namespace taskqueue {
class queue {
public:
  queue() : sequence_num_(0) { _val.clear(); }
  ~queue() { _val.clear(); }
  bool empty();
  int size();
  void push_back(const PendingTask &task);
  //_val.empty() is false
  PendingTask pick_front();
  PendingTask front();
  void swap(queue &val);

private:
  std::list<PendingTask> _val;
  unsigned int sequence_num_;
};

} // end namespace taskqueue

typedef std::priority_queue<base::PendingTask> DelayedTaskQueue;
typedef taskqueue::queue TaskQueue;

} // namespace base
