/*
  Hexbright demo firmware: Momentary
  Andrew Magill  9/2012
  
  Light turns on when the button is down.  That's it.
*/

#include <hexbright.h>
#include <Wire.h>
hexbright hb;

void setup()
{
  hb.init_hardware();
}

void loop()
{
  hb.update();
  if (hb.button_pressed())
    hb.set_light(MAX_LEVEL, MAX_LEVEL, NOW);
  else
  // We could set_light(CURRENT_LEVEL, 0, NOW), but this will
  // actually turn us off if we're not plugged in.
    hb.shutdown();
}

