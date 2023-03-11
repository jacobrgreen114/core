// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#include <atomic>
#include <unordered_set>
#include <typeinfo>

#include "datatypes.hpp"

#define MUCHCOOL_POINTER_IMPLICIT_CASTING true

namespace muchcool {

/**
 * @brief Base class for all "Objects"
 * @todo Implement weak references
 */
class Object {
  class WeakControlBlock {
    std::atomic<Object*> _ptr;
    std::atomic_size_t _weak;
    friend Object;
  };

  std::atomic_size_t _strong;
  std::atomic<WeakControlBlock*> _weak;

 public:
  constexpr Object() : _strong{0}, _weak{null} {}

  constexpr virtual ~Object() = default;

 private:
  auto _object_ref() const -> void { ++(const_cast<Object*>(this)->_strong); }

  auto _object_unref() const -> bool {
    return --(const_cast<Object*>(this)->_strong) == 0;
  }

  template <typename T>
  friend class Shared;
};

template <typename T>
class Shared;

template <typename T>
class Shared final {
  T* _ptr;

 public:
  auto* get() const { return _ptr; }

  /*
   * Ctors / Dtors
   */

  constexpr Shared() : _ptr{null} {}

  constexpr Shared(Shared&& move) noexcept : _ptr(move._ptr) {
    move._ptr = null;
  }

  constexpr Shared(const Shared& copy) : _ptr(copy._ptr) {
    if (_ptr) {
      _ptr->_object_ref();
    }
  }

  constexpr explicit Shared(T* ptr) : _ptr(ptr) {
    if (_ptr) {
      _ptr->_object_ref();
    }
  }

  constexpr ~Shared() {
    if (_ptr && _ptr->_object_unref()) {
      delete _ptr;
    }
  }

  /*
   * Assignment
   */

  constexpr auto operator=(Shared&& move) -> Shared& {
    if (_ptr && _ptr->_object_unref()) {
      delete _ptr;
    }
    _ptr = move._ptr;
    move._ptr = null;
    return *this;
  }

  constexpr auto operator=(const Shared& copy) -> Shared& {
    if (_ptr && _ptr->_object_unref()) {
      delete _ptr;
    }

    _ptr = copy._ptr;

    return *this;
  }

  constexpr auto operator=(T* ptr) -> Shared& {
    if (_ptr && _ptr->_object_unref()) {
      delete _ptr;
    }

    _ptr = ptr;
    if (_ptr) {
      _ptr->_object_ref();
    }

    return *this;
  }

#if MUCHCOOL_POINTER_IMPLICIT_CASTING

  template <typename U>
  constexpr Shared(Shared<U>&& move) noexcept
      : _ptr(static_cast<T*>(move._ptr)) {
    move._ptr = null;
  }

  template <typename U>
  constexpr Shared(const Shared<U>& copy) noexcept
      : _ptr(static_cast<T*>(copy._ptr)) {
    if (_ptr) {
      _ptr->_object_ref();
    }
  }

  template <typename U>
  constexpr Shared(U* ptr) noexcept : _ptr(static_cast<T*>(ptr)) {
    if (_ptr) {
      _ptr->_object_ref();
    }
  }

  template <typename U>
  constexpr auto operator=(Shared<U>&& move) -> Shared& {
    if (_ptr && _ptr->_object_unref()) {
      delete _ptr;
    }
    _ptr = static_cast<T*>(move._ptr);
    move._ptr = null;

    return *this;
  }

#endif

  constexpr auto operator*() const noexcept -> T& { return *_ptr; }
  constexpr auto operator->() const noexcept -> T* { return _ptr; }

  constexpr operator bool() const noexcept { return _ptr; }

  constexpr auto operator==(const Shared& copy) const -> bool {
    return _ptr == copy._ptr;
  }

  constexpr auto operator==(const T* const ptr) const -> bool { return _ptr == ptr; }

  template <typename... Args>
  constexpr static Shared create(Args&&... args) {
    return Shared{new T(std::forward<Args>(args)...)};
  }

  template <typename U>
  constexpr static auto cast_static(Shared&& move) -> Shared<U> {
    auto ptr = Shared<U>{};
    ptr._ptr = static_cast<U*>(move._ptr);
    move._ptr = null;
    return ptr;
  }

  template <typename U>
  constexpr static auto cast_reinterpret(Shared&& move) -> Shared<U> {
    auto ptr = Shared<U>{};
    ptr._ptr = reinterpret_cast<U*>(move._ptr);
    move._ptr = null;
    return ptr;
  }

  template <typename U>
  constexpr static auto cast_dynamic(Shared&& move) -> Shared<U> {
    auto ptr = Shared<U>{};
    ptr._ptr = dynamic_cast<U*>(move._ptr);
    move._ptr = null;
    return ptr;
  }
};

template <typename B, typename A>
[[deprecated("As will be removed in the future")]] B* As(A* a) {
  auto ptr = dynamic_cast<B*>(a);
  [[unlikely]] if (ptr == nullptr)
    throw std::exception();
  return ptr;
}

template <typename B, typename A>
[[deprecated("Is will be removed in the future")]] bool Is(A* a) {
  auto ptr = dynamic_cast<B*>(a);
  return ptr != nullptr;
}

}  // namespace muchcool
