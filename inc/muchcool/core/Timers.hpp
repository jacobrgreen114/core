
// Copyright (c) 2022. Jacob R. Green
// All Rights Reserved.

#pragma once

#include "Datatypes.hpp"

#include <chrono>

using Clock = std::chrono::high_resolution_clock;

class Timer final {
  Clock::time_point _startTime;

public:
  Timer();

  void Reset();

  double ElapsedTime() const;
};

using ScopedTimerCallback = void (*)(const char*, double);

class ScopedTimer {
  Timer _timer;
  const char* _name;
  ScopedTimerCallback _callback;

public:
  ScopedTimer(const char* name, ScopedTimerCallback callback);
  ~ScopedTimer();
};

enum class Month : uint16 {
  January = 1,
  February,
  March,
  April,
  May,
  June,
  July,
  August,
  September,
  October,
  November,
  December
};

enum class DayOfWeek : uint16 {
  Sunday = 0,
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday
};

class DateTime {
  uint16 _year;
  Month _month;
  DayOfWeek _dayOfWeek;
  uint16 _day;
  uint16 _hour;
  uint16 _minute;
  uint16 _second;
  uint16 _millisecond;
  uint64 _ticks;

public:
  DateTime(uint16 year = {}, Month month = Month::January,
           DayOfWeek dayOfWeek = DayOfWeek::Sunday, uint16 day = {},
           uint16 hour = {}, uint16 minute = {}, uint16 second = {},
           uint16 milli = {}, uint64 ticks = {});

  static DateTime Now();
};