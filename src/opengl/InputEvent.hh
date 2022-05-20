#pragma once

class InputEvent {
 public:
  const uint16_t userid;  // id of the user who created this input
  const uint16_t
      input : 12;  // 12 bits reserved to button or key that created the event
  const uint16_t mods : 4;  // 4 modifier buttons (SHIFT, ALT, CTRL, WTF)
  const uint16_t x,
      y;  // coordinates are 16 bit for quite a bit of headroom! 64k pixels
 public:
  InputEvent(uint16_t userid, uint16_t input, uint16_t mods, uint16_t x,
             uint16_t y)
      : userid(userid), input(input), mods(mods), x(x), y(y) {}
};