#pragma once
#include <stdint.h>

// Pure timing/state logic, separated from Arduino I/O for host tests.
class FeederController {
 public:
  FeederController(uint32_t intervalMs, uint32_t openMs, uint32_t cooldownMs)
      : interval(intervalMs), duration(openMs), cooldown(cooldownMs) {}

  void begin(uint32_t now) {
    lastFeed = now;
    openedAt = now;
    active = false;
    started = true;
    fedBefore = false;
    count = 0;
  }

  bool requestFeed(uint32_t now) {
    if (!started || active) return false;
    if (fedBefore && uint32_t(now - lastFeed) < cooldown) return false;
    active = true;
    openedAt = now;
    lastFeed = now;
    fedBefore = true;
    ++count;
    return true;
  }

  void update(uint32_t now) {
    if (!started) return;
    if (active) {
      if (uint32_t(now - openedAt) >= duration) active = false;
      return;
    }
    if (uint32_t(now - lastFeed) >= interval) requestFeed(now);
  }

  bool isOpen() const { return active; }
  uint32_t feedCount() const { return count; }

 private:
  uint32_t interval, duration, cooldown;
  uint32_t lastFeed = 0, openedAt = 0, count = 0;
  bool active = false, started = false, fedBefore = false;
};
