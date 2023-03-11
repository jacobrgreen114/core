// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include "delegate.hpp"

namespace muchcool {

template <typename... Args>
class Event {
 public:
  using DelegateType = Delegate<void, Args...>;

 private:
  std::vector<Shared<DelegateType>> _delegates;

 public:
  void Invoke(Args&&... args) {
    for (auto& delegate : _delegates)
      delegate->Invoke(std::forward<Args>(args)...);
  }

  Event& operator+=(const Shared<DelegateType>& handler) {
    auto pos = std::find(_delegates.begin(), _delegates.end(), handler);
    if (pos == _delegates.end()) _delegates.emplace_back(handler);

    return *this;
  }

  Event& operator-=(const Shared<DelegateType>& handler) {
    auto pos = std::remove(_delegates.begin(), _delegates.end(), handler);
    if (pos != _delegates.end()) _delegates.erase(pos, _delegates.end());

    return *this;
  }
};

}  // namespace muchcool