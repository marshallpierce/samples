Translated version of hexbright_demo_morse (translated to the hexbright library: https://github.com/dhiltonp/hexbright).

This program required significant alterations.

The library is designed to be run periodically, managing light changes, overheating, the accelerometer, etc.

The original version of this program was proceedural, never returning from loop until shutting down.  As such, the program's flow had to be re-written.


I've made 3 changes from an external perspective:
The message won't start unless the button has been released in less than .5 seconds.
The message can be stopped by holding down the button for longer than .5 seconds.
The message is followed by 3 red flashes (not green), because it's absurdly easy to do (number printing library: print 3.  30 would print 3 green flashes, followed by a long red (zero)).
