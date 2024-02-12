// Slow fade
#include "np_slow_fade.h"




void SlowFade(slowfade_state &state, int min, int max, int time)
{
  float delta = ((max - min) / time);

  if (state.interval == 0)
  {
    state.brightness = min;
  }
  if (state.interval < (time / 2))
  {
    state.brightness += delta;
    state.strip.fill(state.strip.Color(state.brightness, state.brightness, state.brightness));
  }
  if (state.interval >= (time / 2))
  {
    state.brightness -= delta;
    state.strip.fill(state.strip.Color(state.brightness, state.brightness, state.brightness));
  }
  state.strip.show();

  if (state.interval < time)
  {
    state.interval++;
  }
  else
  {
    state.interval = 0;
  }
}
