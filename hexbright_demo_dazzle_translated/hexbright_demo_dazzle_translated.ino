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
    if (millis() - lastTime > 10)
    {
      if(random(3)<1)
        hb.set_light(1000, 0, 10);
      lastTime = millis();
    }
  }
  else
    hb.shutdown();
}

