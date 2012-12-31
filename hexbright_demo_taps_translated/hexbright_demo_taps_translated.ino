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
// Constants
#define ACC_ADDRESS             0x4C
#define ACC_REG_XOUT            0
#define ACC_REG_YOUT            1
#define ACC_REG_ZOUT            2
#define ACC_REG_TILT            3
#define ACC_REG_INTS            6
#define ACC_REG_MODE            7
// Pin assignments
#define DPIN_RLED_SW            2
#define DPIN_GLED               5
#define DPIN_PWR                8
#define DPIN_DRV_MODE           9
#define DPIN_DRV_EN             10
// Modes
#define MODE_OFF                0
#define MODE_RECORD             1
#define MODE_PLAY               2

// State
byte mode;
unsigned long btnTime, tapTime;
unsigned long recording[BUFSIZE];
int nTaps, curTap;

void setup()
{
/*
  // We just powered on!  That means either we got plugged 
  // into USB, or the user is pressing the power button.
  pinMode(DPIN_PWR,      INPUT);
  digitalWrite(DPIN_PWR, LOW);

  // Initialize GPIO
  pinMode(DPIN_RLED_SW,  INPUT);
  pinMode(DPIN_GLED,     OUTPUT);
  pinMode(DPIN_DRV_MODE, OUTPUT);
  pinMode(DPIN_DRV_EN,   OUTPUT);
  digitalWrite(DPIN_DRV_MODE, LOW);
  digitalWrite(DPIN_DRV_EN,   LOW);
  
  // Initialize serial busses
  Serial.begin(9600);
  Wire.begin();

  // Configure accelerometer
  byte config[] = {
    ACC_REG_INTS,  // First register (see next line)
    0xE4,  // Interrupts: shakes, taps
    0x00,  // Mode: not enabled yet
    0x00,  // Sample rate: 120 Hz
    0x0F,  // Tap threshold
    0x05   // Tap debounce samples
  };
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(config, sizeof(config));
  Wire.endTransmission();

  // Enable accelerometer
  byte enable[] = {ACC_REG_MODE, 0x01};  // Mode: active!
  Wire.beginTransmission(ACC_ADDRESS);
  Wire.write(enable, sizeof(enable));
  Wire.endTransmission();

*/
  mode = MODE_OFF;
  btnTime = 0;
  Serial.println("Powered up!");
  hb.init_hardware();
}

void loop()
{
  hb.update();
  switch (mode)
  {
  case MODE_OFF:
  /*
    if (digitalRead(DPIN_RLED_SW))
    {  // Button is down..
      if (btnTime==0)
      {  // ..and just went down
        btnTime = millis();
      }
      else if (millis()-btnTime > 200)
      {  // ..and has been down for a little while
        Serial.println("Recording!");
        nTaps = 0;
        tapTime = 0;
        for (int i=0; i<BUFSIZE; i++)
          recording[i] = 0;
        digitalWrite(DPIN_PWR, HIGH);
        pinMode(DPIN_PWR, OUTPUT);
        analogWrite(DPIN_DRV_EN, 32);
        mode = MODE_RECORD;
      }
    }*/
    if(hb.button_held()>200){
      // re-initialize variables
      btnTime = millis();
      nTaps = 0;
      tapTime = 0;
      for (int i=0; i<BUFSIZE; i++)
        recording[i] = 0;
      // turn on light as indicator
      hb.set_light(CURRENT_LEVEL, 300, NOW);
      // and start the recording
      Serial.println("Recording!");
      mode = MODE_RECORD;
    }    
    break;
  case MODE_RECORD:
    if (hb.button_held() && nTaps<BUFSIZE)
    {/*
      // Poll the accelerometer
      Wire.beginTransmission(ACC_ADDRESS);
      Wire.write(ACC_REG_TILT);
      Wire.endTransmission(false);       // End, but do not stop!
      Wire.requestFrom(ACC_ADDRESS, 1);  // This one stops.
      delay(5);
      if (Wire.read() & 0x20 && millis()-tapTime>200)
      {  // Tap detected!
        Serial.println("Tap!");
        tapTime = millis();
        recording[nTaps++] = tapTime-btnTime;
        analogWrite(DPIN_DRV_EN, 255);
        delay(100);
        analogWrite(DPIN_DRV_EN, 32);
      }*/
      
      if(hb.tapped() && millis()-tapTime>200)
      {
        Serial.println("Tap!");
        tapTime = millis();
        recording[nTaps++] = tapTime-btnTime;
        hb.set_light(MAX_LEVEL, CURRENT_LEVEL, 150);
      }
    }
    else
    {  // Time to transition to playback mode
      Serial.println("Playback!");
      for (int i = 0; i < nTaps; i++)
      {
        Serial.print(recording[i]);
        Serial.print(" ");
      }
      recording[nTaps] = millis()-btnTime;  // End time
      Serial.println(recording[nTaps]);
      curTap = 0;
      btnTime = millis();
      mode = MODE_PLAY;
    }
    break;
  case MODE_PLAY:    
/*    if (digitalRead(DPIN_RLED_SW))
    { // Buton pressed again.  Turn off.
      Serial.println("Off!");
      digitalWrite(DPIN_DRV_EN, LOW);
      digitalWrite(DPIN_PWR, LOW);
      btnTime = 0;
      mode = MODE_OFF;
      while (digitalRead(DPIN_RLED_SW));
      delay(200);
    } 
    else
    { // No button, keep playing.
      if (millis()-btnTime > recording[curTap])
      { // Time to flash out this tap!
        if (curTap < nTaps)
        {
          analogWrite(DPIN_DRV_EN, 255);
          delay(100);
          analogWrite(DPIN_DRV_EN, 32);
          curTap++;
        }
        else
        {
          curTap = 0;
          btnTime = millis();
        }
      }
    }    
*/
    if (hb.button_released()) // make sure we don't switch modes until the button is released (or we're off)
    {
      btnTime = 0;
      mode = MODE_OFF;
    } 
    else if (hb.button_held()) 
    {
      Serial.println("Off!");
      hb.shutdown();
    }
    else
    { // No button, keep playing.
      if (millis()-btnTime > recording[curTap])
      { // Time to flash out this tap!
        if (curTap < nTaps)
        {
          hb.set_light(MAX_LEVEL, CURRENT_LEVEL, 150);
          curTap++;
        }
        else
        {
          curTap = 0;
          btnTime = millis();
        }
      }
    }    
    break;
  }
}

