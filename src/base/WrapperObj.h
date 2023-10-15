#pragma once

// weakprt
// ref count
// Unretained
namespace base {

template <typename T> class __declspec(novtable) ObjWrapper {
public:
  virtual T *get() const = 0;
  virtual void AddRef() const = 0;
  virtual void Release() const = 0;
  virtual bool IsWeakPtr() const = 0;
};

template <typename T> class NormalWrapper : public ObjWrapper<T> {
public:
  explicit NormalWrapper(T *o) : ptr_(o) {}
  virtual T *get() const override { return ptr_; }
  virtual void AddRef() const override { ptr_->AddRef(); }
  virtual void Release() const override { ptr_->Release(); }
  virtual bool IsWeakPtr() const override { return false; }

private:
  T *ptr_;
};

template <typename T> class UnretainedWrapper : public ObjWrapper<T> {
public:
  explicit UnretainedWrapper(T *o) : ptr_(o) {}
  T *get() const override { return ptr_; }
  void AddRef() const override {}
  void Release() const override {}
  bool IsWeakPtr() const override { return false; }

private:
  T *ptr_;
};

template <typename T> static inline UnretainedWrapper<T> *Unretained(T *o) {
  return new UnretainedWrapper<T>(o);
}

} // namespace base
