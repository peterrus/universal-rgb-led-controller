#include <math.h>

/**
 * RGBController
 * author: peterrus
 * License: Attribution-ShareAlike 3.0 Unported (http://creativecommons.org/licenses/by-sa/3.0/)
 * rgbled autofading code based on https://gist.github.com/766994 by jamesotron
 */

const int redPin = 9;
const int greenPin = 6;
const int bluePin = 10;
const int btnPin = 2;
const int ledPin = 13;

const unsigned int debounceTime = 50;

unsigned long lastInterruptTime = 0;

boolean cycle = true;

boolean doModeSwitch = false;

void setup() {
  Serial.begin(38400);
  Serial.println("ctrl:on");
  
  pinMode(btnPin, INPUT);
  digitalWrite(btnPin, HIGH);

  attachInterrupt(0, doCycleToggle, LOW);
  
  // Start off with the LED off.
  setColourRgb(0,0,0);
}

void loop() {
  if(cycle)
  {
    unsigned int rgbColour[3];
  
    // Start off with red.
    rgbColour[0] = 255;
    rgbColour[1] = 0;
    rgbColour[2] = 0;  
  
    // Choose the colours to increment and decrement.
    for (int decColour = 0; decColour < 3; decColour += 1) {
      
      if(doModeSwitch) 
      {
        doModeSwitch = false;
        setColourRgb(0,0,0);
        break;
      }
      
      int incColour = decColour == 2 ? 0 : decColour + 1;
  
      int previousVal = 0;
  
      // cross-fade the two colours.
      for(int i = 0; i < 255; i += 1) {
        if(!cycle) { doModeSwitch = true; break; }
        
        rgbColour[decColour] -= 1;
        rgbColour[incColour] += 1;
        
        setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
        
        float reading = analogRead(A0);
        int fadeSpeed = 1024*pow((reading/1024), 6);

        if(fadeSpeed - 2 != fadeSpeed && fadeSpeed + 2 != fadeSpeed && fadeSpeed != fadeSpeed && fadeSpeed - 1 != fadeSpeed && fadeSpeed + 1 != fadeSpeed)
        {
          Serial.print("ctrl:speed=");
          Serial.println(fadeSpeed);
        }
        
        previousVal = fadeSpeed;
        
        delay(fadeSpeed);
      }
    }
  }
  else
  {
    setColourFromPotmeter(analogRead(A0));
  }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
 }

void doCycleToggle()
{ 
  if(millis() - lastInterruptTime > debounceTime)
  {
    cycle = !cycle;
  }
  
  lastInterruptTime = millis();
  
//  if(cycle)
//  {
//    Serial.println("ctrl:mode=cycle");
//  }
//  else
//  {
//    Serial.println("ctrl:mode=colorwheel");
//  }
}

void setColourFromPotmeter(double potValue)
{
  // R = (sin(x * pi / 512 - (0.5 * pi)) + 1) * 127.5
  // G = (sin((x + 2 / 3 * 1024) * pi / 512 - (0.5 * pi)) + 1) * 127.5
  // B = (sin((x + 4 / 3 * 1024) * pi / 512 - (0.5 * pi)) + 1) * 127.5
  double pi = 3.1415;
  double x = potValue;
  
  // delen werkt niet: gebruik 3.0 notatie.
  int red = (((sin(x*pi/512.0-(0.5*pi)))+0.5)/1.5*255.0);
  int green = (((sin((x+2.0/3.0*1024.0)*pi/512.0-(0.5*pi)))+0.5)/1.5*255.0);
  int blue = (((sin((x+4.0/3.0*1024.0)*pi/512.0-(0.5*pi)))+0.5)/1.5*255.0);

  setColourRgb(red, green, blue);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}


