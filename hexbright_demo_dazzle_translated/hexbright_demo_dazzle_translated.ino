/*
  Hexbright demo firmware: Dazzle
  Andrew Magill  9/2012
  
  Dazzler flashes while the button is down.
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
  static unsigned long lastTime;
  hb.update();
  
  if (hb.button_held())
  {
    hb.set_light(0, (random(4)<1)*1000, NOW);
  }
  else
    hb.shutdown();
}

