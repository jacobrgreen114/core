
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include <vector>

#include "collections.hpp"

namespace muchcool {

template <typename T>
class Array final : public virtual IReadonlyArrayList<T> {
 private:
  class Enumerator;
  friend Enumerator;

  std::vector<T> _vec;

 public:
  explicit Array(uword size) : _vec(size) {}

  uword Size() const override { return _vec.size(); }

  T* Data() const override { return const_cast<T*>(_vec.data()); }

  T& At(iword index) const override { return Data()[index]; }

  static Shared<Array<T>> New(uword size) { return new Array(size); }

  static Shared<Array<T>> From(const Shared<IEnumerable<T>>& enumerable) {
    auto size = enumerable->Size();

    auto array = Array<T>::New(size);

    auto enumerator = enumerable->GetEnumerator();
    for (iword i = 0; i < size && enumerator->MoveNext(); ++i) {
      array.At(i) = enumerator->Current();
    }

    return array;
  }

  Shared<IEnumerator<T>> GetEnumerator() const override {
    return new Enumerator(const_cast<Array<T>*>(this));
  }
};

template <typename T>
class Array<T>::Enumerator final : public virtual IEnumerator<T> {
  Shared<Array<T>> _array;
  typename std::vector<T>::iterator _it;

 public:
  Enumerator(Array<T>* array) : _array(array), _it(array->_vec.begin()) {}

  ~Enumerator() override = default;

  bool MoveNext() override { return ++_it < _array->_vec.end(); }
  T& Current() const override { return *_it; }
  void Reset() override { _it = _array->_vec.begin(); }
};

}  // namespace muchcool