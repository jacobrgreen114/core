

// Copyright (c) 2022-2023 Jacob R. Green
// All Rights Reserved.

#include "muchcool/core/Timers.hpp"

Timer::Timer() : _startTime(Clock::now()) {}

void Timer::Reset() { _startTime = Clock::now(); }

using DurationSeconds =
    std::chrono::duration<double, std::chrono::seconds::period>;

double Timer::ElapsedTime() const {
  auto currentTime = Clock::now();
  auto duration = DurationSeconds(currentTime - _startTime);
  return duration.count();
}

ScopedTimer::ScopedTimer(const char *name, ScopedTimerCallback callback)
    : _name(name), _callback(callback) {}

ScopedTimer::~ScopedTimer() {
  if (_callback)
    _callback(_name, _timer.ElapsedTime());
}

DateTime::DateTime(uint16 year, Month month, DayOfWeek dayOfWeek, uint16 day,
                   uint16 hour, uint16 minute, uint16 second, uint16 milli,
                   uint64 ticks)
    : _year(year), _month(month), _dayOfWeek(dayOfWeek), _day(day), _hour(hour),
      _minute(minute), _second(second), _millisecond(milli), _ticks(ticks) {}

DateTime DateTime::Now() {
  FILETIME fileTime;
  SYSTEMTIME systemTime;

  GetSystemTimeAsFileTime(&fileTime);
  FileTimeToSystemTime(&fileTime, &systemTime);

  return DateTime(systemTime.wYear, (Month)systemTime.wMonth,
                  (DayOfWeek)systemTime.wDayOfWeek, systemTime.wDay,
                  systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
                  systemTime.wMilliseconds,
                  *reinterpret_cast<uint64 *>(&fileTime));
}
