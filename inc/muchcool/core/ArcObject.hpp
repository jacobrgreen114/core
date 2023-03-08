// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

//
// Created by jacob on 6/6/2022.
//

#pragma once

#include <atomic>
#include <unordered_set>
#include <typeinfo>

#include "Datatypes.hpp"

template <class T, class U>
concept Derived = std::is_base_of_v<U, T>;

class Object {
  using ref_t = int_fast16_t;
  using atomic_t = std::atomic<ref_t>;

  atomic_t _strong;
  // std::atomic_int16_t _weak;

public:
  Object() : _strong(0) /*, _weak(0) */ {
    // std::cout << typeid(*this).name() << " allocated. " << std::endl;
  }

  virtual ~Object() {
      // std::cout << typeid(*this).name() << " deleted. " << std::endl;
  };

private:
  void Deconstruct() { this->~Object(); }
  void DeleteThis() { operator delete(this); }

  // TODO : Ref and Unref functions are probably not thread safe

  void AddRef() {
    bool swapped = false;
    do {
      auto expected = _strong.load();
      swapped = _strong.compare_exchange_strong(expected, expected + 1);
    } while (!swapped);
  }

  void UnRef() {
    bool swapped = false;
    ref_t desired;

    do {
      ref_t expected = _strong.load();
      if (expected < 1)
        break;
      desired = expected - 1;
      swapped = _strong.compare_exchange_strong(expected, desired);
    } while (!swapped);

    if (swapped && desired < 1) {
      delete this;
    }
  }

  template <typename T> friend class Pointer;
};

template <typename T> class Pointer final {
private:
  T* _ptr;

public:
  Pointer() : _ptr(null) {}

  Pointer(T* ptr) : _ptr(ptr) {
    if (_ptr != null)
      _ptr->AddRef();
  }

  Pointer(Pointer&& ptr) : _ptr(ptr._ptr) { ptr._ptr = null; }

  Pointer(const Pointer& ptr) : _ptr(ptr._ptr) {
    if (_ptr != null)
      _ptr->AddRef();
  }

  ~Pointer() {
    if (_ptr != null)
      _ptr->UnRef();
  }

  void operator=(T* ptr) {
    if (_ptr != null)
      _ptr->UnRef();

    _ptr = ptr;
    if (_ptr != null)
      _ptr->AddRef();
  }

  void operator=(const Pointer& ptr) { *this = ptr._ptr; }

  void operator=(Pointer&& ptr) {
    if (_ptr != null)
      _ptr->UnRef();

    _ptr = ptr;
    ptr._ptr = null;
  }

  T& operator*() const noexcept { return *_ptr; }
  T* operator->() const noexcept { return _ptr; }

  operator T*() const noexcept { return _ptr; }
  operator T* const*() const noexcept { return &_ptr; }
  operator bool() const noexcept { return _ptr != null; }

  template <typename... Args> static Pointer New(Args&&... args) {
    auto t = new T(std::forward<Args>(args)...);
    return t;
  }
};

template <typename B, typename A> B* As(A* a) {
  auto ptr = dynamic_cast<B*>(a);
  [[unlikely]] if (ptr == nullptr)
    throw std::exception();
  return ptr;
}

template <typename B, typename A> bool Is(A* a) {
  auto ptr = dynamic_cast<B*>(a);
  return ptr != nullptr;
}
