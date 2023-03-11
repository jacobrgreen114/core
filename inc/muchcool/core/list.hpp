
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include <vector>

#include "collections.hpp"

namespace muchcool {

template <typename T>
class List : public virtual IArrayList<T> {
  std::vector<T> _vec;

 public:
  explicit List() : _vec() {}

  uword Size() const override { return _vec.size(); }

  void Add(const T& item) override { return _vec.push_back(item); }

  T* Data() const override { return const_cast<T*>(_vec.data()); }

  T& At(iword index) const override { return Data()[index]; }

  static Shared<List<T>> New() { return new List<T>(); }

  static Shared<List<T>> From(const Shared<IEnumerable<T>>& enumerable);
};

template <typename T>
Shared<List<T>> List<T>::From(const Shared<IEnumerable<T>>& enumerable) {
  auto list = std::make_shared<List<T>>();

  for (auto enumerator = enumerable->GetEnumerator(); enumerator->MoveNext();) {
    list.Add(enumerator->Current());
  }

  return list;
}

}  // namespace muchcool