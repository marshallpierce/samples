/*
  Hexbright demo firmware: Fades
  Andrew Magill  9/2012
  
  1. Hold button down.  Light fades up and down.
  2. Let go of button.  Light holds current brightness.
  3. Press button.  Light turns off.
*/

#include <hexbright.h>
#include <Wire.h>

hexbright hb;

// Modes
#define MODE_OFF                0
#define MODE_FADE               1
#define MODE_ON                 2

byte mode;

void setup()
{
  hb.init_hardware();
  
  mode = MODE_OFF;
}

void loop()
{
  hb.update();
  switch (mode)
  {
  case MODE_OFF:
    if(hb.button_held()) {
      mode = MODE_FADE;
    }
    break;
  case MODE_FADE:
    if(hb.button_held())
    {
      if(!hb.light_change_remaining())
        if(hb.get_light_level()<500) // the light is low, go up from here
          hb.set_light(CURRENT_LEVEL, 1000, 1000);  // go from low to high over 2 seconds
        else
          hb.set_light(CURRENT_LEVEL, 1, 1000); // go from high to low over 2 seconds
    }
    else // released!
    { // use the current light level for our brightness
      hb.set_light(CURRENT_LEVEL, CURRENT_LEVEL, NOW);
      mode = MODE_ON;
    }
    break;
  case MODE_ON:
    if (hb.button_held()<200 && hb.button_released())
    {
      hb.shutdown();
      mode = MODE_OFF;
    }
    else if (hb.button_held()>200) 
    {
      mode = MODE_FADE; 
    }
    break;    
  }
}

