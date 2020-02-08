#include "g/engine.h"
#include "config_engine.h"

#define TYPING_LAYER 1

C_SIZE    process_engine_pre(C_SIZE cur_chord, uint16_t keycode, keyrecord_t *record) { 
  // Check if we're on Typing Layer, if so do engine-y processing
  if ((layer_state & (1 << TYPING_LAYER)) != 0)
    return true; 

  return false;
}
