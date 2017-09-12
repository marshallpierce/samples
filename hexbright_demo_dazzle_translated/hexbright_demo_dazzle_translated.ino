/*
  Hexbright demo firmware: Dazzle
  Andrew Magill  9/2012
  
  Dazzler flashes while the button is down.
*/

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

hexbright hb;

void setup()
{
  hb.init_hardware();
}

void loop()
{
  static unsigned long lastTime;
  hb.update();
  
  if (hb.button_pressed())
  {
    hb.set_light(0, (random(4)<1)*1000, NOW);
  }
  else
    hb.set_light(0, OFF_LEVEL, NOW);
}

