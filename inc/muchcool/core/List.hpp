
#pragma once

#include <vector>

#include "Collections.hpp"

template <typename T> class List : public virtual IArrayList<T> {
  std::vector<T> _vec;

public:
  explicit List() : _vec() {}

  uintn Size() const override { return _vec.size(); }

  void Add(const T &item) override { return _vec.push_back(item); }

  T *Data() const override { return const_cast<T *>(_vec.data()); }

  T &At(intn index) const override { return Data()[index]; }


  static Pointer<List<T>> New() { return new List<T>(); }

  static Pointer<List<T>> From(const Pointer<IEnumerable<T>> &enumerable);
};

template <typename T>
Pointer<List<T>> List<T>::From(const Pointer<IEnumerable<T>> &enumerable) {
  auto list = std::make_shared<List<T>>();

  for (auto enumerator = enumerable->GetEnumerator(); enumerator->MoveNext();) {
    list.Add(enumerator->Current());
  }

  return list;
}