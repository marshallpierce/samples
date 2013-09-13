/*
  Hexbright demo firmware: Morse
  Andrew Magill  9/2012
  
  On each button press, flashes out a message in morse code
  and then turns off.  Message and speed are configurable 
  just below.
*/

#include <hexbright.h>
#include <twi.h>

hexbright hb;

char message[] = "SOS HEXBRIGHT SOS";
int millisPerBeat = 75;

// High byte = length
// Low byte  = morse code, LSB first, 0=dot 1=dash
word morse[] = {
  0x0202, // A .-
  0x0401, // B -...
  0x0405, // C -.-.
  0x0301, // D -..
  0x0100, // E .
  0x0404, // F ..-.
  0x0303, // G --.
  0x0400, // H ....
  0x0200, // I ..
  0x040E, // J .---
  0x0305, // K -.-
  0x0402, // L .-..
  0x0203, // M --
  0x0201, // N -.
  0x0307, // O ---
  0x0406, // P .--.
  0x040B, // Q --.-
  0x0302, // R .-.
  0x0300, // S ...
  0x0101, // T -
  0x0304, // U ..-
  0x0408, // V ...-
  0x0306, // W .--
  0x0409, // X -..-
  0x040D, // Y -.--
  0x0403, // Z --..
  0x051F, // 0 -----
  0x051E, // 1 .----
  0x051C, // 2 ..---
  0x0518, // 3 ...--
  0x0510, // 4 ....-
  0x0500, // 5 .....
  0x0501, // 6 -....
  0x0503, // 7 --...
  0x0507, // 8 ---..
  0x050F, // 9 ----.
};

void setup()
{
  hb.init_hardware();
}

#define OFF_MODE 0
#define MORSE_MODE 1
int mode = 0;

void loop()
{
  hb.update();
  if(hb.button_pressed_time()>500)
  {
    mode = OFF_MODE;
  }
  else if (hb.button_pressed_time()<500 && hb.button_just_released())
  {
    mode = MORSE_MODE; 
  }
  
  if(mode == OFF_MODE)
  {
    hb.set_light(0, OFF_LEVEL, NOW);
  }
  if(mode == MORSE_MODE)
  {
    if(!hb.light_change_remaining())
    { // we're not currently doing anything, start the next step
      static int current_character = 0;
      static char symbols_remaining = 0;
      static byte pattern = 0;
      
      if(current_character>=sizeof(message))
      { // we've hit the end of message, turn off.
        mode = OFF_MODE;
        // reset the current_character, so if we're connected to USB, next printing will still work.
        current_character = 0;
        
        // Flash three times the easy way (but a different color).
        //  Printing 30 would flash green 3 times, with a long red.
        hb.print_number(3);
        // return now to skip the following code.
        return;
      }
            
      if(symbols_remaining <= 0) // we're done printing our last character, get the next!
      {
        char ch = message[current_character];
        // Remap ASCII to the morse table
        if      (ch >= 'A' && ch <= 'Z') ch -= 'A';
        else if (ch >= 'a' && ch <= 'z') ch -= 'a';
        else if (ch >= '0' && ch <= '9') ch -= '0' - 26;
        else ch = -1; // character not in table
      
        if(ch>=0)
        {
          // Extract the symbols and length
          pattern = morse[ch] & 0x00FF;
          symbols_remaining = morse[ch] >> 8;
          // we count space (between dots/dashes) as a symbol to be printed;
          symbols_remaining *= 2;
        }
        current_character++;
      }
      
      if (symbols_remaining<=0)
      { // character was unrecognized, treat it as a space
        // 7 beats between words, but 3 have already passed
        // at the end of the last character
        hb.set_light(0,0, millisPerBeat * 4);
      }
      else if (symbols_remaining==1) 
      { // last symbol in character, long pause
        hb.set_light(0, 0, millisPerBeat * 3);
      }
      else if(symbols_remaining%2==1) 
      { // even symbol, print space!
        hb.set_light(0,0, millisPerBeat);
      }
      else if (pattern & 1)
      { // dash, 3 beats
        hb.set_light(MAX_LEVEL, MAX_LEVEL, millisPerBeat * 3);
        pattern >>= 1;
      }
      else 
      { // dot, by elimination
        hb.set_light(MAX_LEVEL, MAX_LEVEL, millisPerBeat);
        pattern >>= 1;
      }
      symbols_remaining--;
    }
  }
}
