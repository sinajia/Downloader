#pragma once

#include "../base/FastDelegate.h"

using namespace fastdelegate;

#define BIND(c, f) MakeDelegate(&c, &f)

typedef FastDelegate0<> Delegate0;
typedef FastDelegate1<LPCTSTR> Delegate1_string;
typedef FastDelegate1<UINT> Delegate1_UINT;

class EventHandler {
public:
  EventHandler() {}
  EventHandler(Delegate0 func) : m_func_no_para(func) {}
  EventHandler(Delegate1_string func) : m_func_para_string(func) {}
  EventHandler(Delegate1_UINT func) : m_func_para_int(func) {}

  void Invoke() {
    if (m_func_no_para) {
      m_func_no_para();
    }
  }

  void Invoke(LPCTSTR strPara) {
    if (m_func_para_string) {
      m_func_para_string(strPara);
    }
  }

  void Invoke(UINT nPara) {
    if (m_func_para_int) {
      m_func_para_int(nPara);
    }
  }

public:
  Delegate0 m_func_no_para;
  Delegate1_string m_func_para_string;
  Delegate1_UINT m_func_para_int;
};
