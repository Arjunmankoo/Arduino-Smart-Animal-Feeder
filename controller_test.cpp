#include <cassert>
#include <cstdint>
#include <iostream>
#include "../firmware/SmartAnimalFeeder/FeederController.h"
int main() {
  FeederController f(6000, 700, 1000);
  f.begin(100);
  assert(!f.isOpen() && f.feedCount() == 0); // no startup dispense
  f.update(6099); assert(!f.isOpen());
  f.update(6100); assert(f.isOpen() && f.feedCount() == 1);
  assert(!f.requestFeed(6200)); // no overlapping feed
  f.update(6799); assert(f.isOpen());
  f.update(6800); assert(!f.isOpen());
  assert(!f.requestFeed(6900)); // cooldown enforced
  assert(f.requestFeed(7100)); // manual feed after cooldown
  f.update(7800); assert(!f.isOpen());
  f.update(13099); assert(!f.isOpen()); // manual feed reset interval
  f.update(13100); assert(f.isOpen() && f.feedCount() == 3);

  FeederController wrap(6000, 700, 1000);
  const uint32_t start = UINT32_MAX - 100;
  wrap.begin(start);
  wrap.update(uint32_t(start + 5999)); assert(!wrap.isOpen());
  wrap.update(uint32_t(start + 6000)); assert(wrap.isOpen());
  wrap.update(uint32_t(start + 6700)); assert(!wrap.isOpen());
  assert(wrap.feedCount() == 1);
  FeederController opening(6000, 700, 1000);
  opening.begin(start); assert(opening.requestFeed(start));
  opening.update(uint32_t(start + 699)); assert(opening.isOpen());
  opening.update(uint32_t(start + 700)); assert(!opening.isOpen());
  std::cout << "Controller timing, manual feed, cooldown and rollover checks passed.\n";
}
