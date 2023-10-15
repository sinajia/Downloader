#pragma once

class ShowState {
public:
  explicit ShowState();
  enum PngType {
    PNG_BEGIN = 0,
    APP1,
    APP2,
    APP3,
    APP4,
    APP5,
    APP6,
  };

  enum TransDegree {
    DETEN = 20,
    DEFIF = 50,
    DESIX = 100,
    DESEVN = 150,
    DEEIGHT = 180,
    DENIGHT = 220,
    DEALL = 255,
  };

  bool bPending_;
  PngType step_;
  DWORD degree_;
};
