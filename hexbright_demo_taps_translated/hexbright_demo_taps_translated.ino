/*
  Hexbright demo firmware: Tap flash!
  Andrew Magill  9/2012
  
  Hold down the button, and tap out a rhythm against
  the light.  It will then flash out the same rhythm
  until you turn it off by pressing the button again.
*/

#include <Wire.h>
#include <hexbright.h>

hexbright hb;

// Settings
#define BUFSIZE                 64
// Modes
#define MODE_OFF                0
#define MODE_RECORD             1
#define MODE_PLAY               2

// State
byte mode;
unsigned int loops;
unsigned int recording[BUFSIZE];
int nTaps, curTap;

void setup()
{
  mode = MODE_OFF;
  Serial.println("Powered up!");
  hb.init_hardware();
}

void loop()
{
  hb.update();
  switch (mode)
  {
  case MODE_OFF:
    if(hb.button_held()>200){
      // (re-)initialize variables
      loops = 0;
      curTap = 0;
      nTaps = 0;
      for (int i=0; i<BUFSIZE; i++)
        recording[i] = 0;
      // turn on light as indicator
      hb.set_light(CURRENT_LEVEL, 200, NOW);
      // and start the recording
      Serial.println("Recording!");
      mode = MODE_RECORD;
    }    
    break;
  case MODE_RECORD:
    if (hb.button_held() && nTaps<BUFSIZE-1)
    {
      if(hb.tapped() && loops>5)
      {
        Serial.println("Tap!");
        recording[nTaps++] = loops; // how many loops passed before a tap?
        loops = 0; // reset our counter
        hb.set_light(MAX_LEVEL, 200, 150);
      }
    }
    else if (hb.button_held() && nTaps==BUFSIZE-1) 
    { // if we're out of buffer space, stop responding, but still keep track of the release time
      recording[nTaps] = loops;
    }
    else
    {  // Time to transition to playback mode
      Serial.println("Playback!");
      for (int i = 0; i < nTaps; i++)
      {
        Serial.print(recording[i]);
        Serial.print(" ");
      }
      recording[nTaps] = loops;  // End time
      Serial.println(recording[nTaps]);
      loops = 0;
      curTap = 0;
      mode = MODE_PLAY;
    }
    break;
  case MODE_PLAY:    
    if (hb.button_released()) // make sure we don't switch modes until the button is released (or we're off)
    {
      mode = MODE_OFF;
    } 
    else if (hb.button_held()) 
    {
      Serial.println("Off!");
      hb.shutdown();
    }
    else
    { // No button, keep playing.
      if (loops > recording[curTap])
      { // Time to flash out this tap!
        if (curTap < nTaps)
        {
          hb.set_light(MAX_LEVEL, 200, 150);
          curTap++;
        }
        else
        {
          curTap = 0;
        }
        loops = 0;
      }
    }    
    break;
  }
  loops++;
}

