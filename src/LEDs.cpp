#include <LEDs.h>

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BGR); // (confirm with tests as DotStars differ in color order)
bool displayOff = true;
bool animated = false;
bool spiral = false;
bool displayE = false;
bool displayEwithRPM = false;
bool displayMNS = false;
bool displayMovingHI = false;
bool customTextDisplay = false;
int movingHIpos = 0;
int spiralLEDPos = 0;
int spiralAnimationSpeed = 200; // Speed of animation in milliseconds
int tspiraltimer = 0;           // Timer for spiral animation
int tHiscrollingtimer = 0;      // Timer for HI scrolling
int currentPosition = 0;
int currentPattern = 0;

void displayImage(int ID, unsigned long colour, int pos)
{

  // Reset display
  strip.clear();
  strip.show();
  displayOff = false;
  spiral = false;
  displayE = false;
  displayEwithRPM = false;
  displayMNS = false;
  displayMovingHI = false;
  customTextDisplay = false;

  switch (ID)
  {
  case 0:
  { // Display image Circle
    strip.setPixelColor(0, 0x000F00);
    strip.show();
    break;
  }
  case 1:
  { // Display image Spiral
    strip.setPixelColor(0, 0x00000F);
    strip.show();
    spiral = true;
    spiralLEDPos = 0;
    break;
  }
  case 2:
  {
    strip.setPixelColor(pos, colour);
    strip.setPixelColor(35 - pos, colour); // Mirror on opposite side
    strip.show();
    break;
  }
  case 3:
  {
    displayE = true;
    break;
  }
  case 4:
  {
    displayEwithRPM = true;
    break;
  }
  case 5:
  {
    displayMNS = true;
    break;
  }
  case 6:
  {
    displayMovingHI = true;
    break;
  }
  case 7:
  {
    customTextDisplay = true;
    break;
  }
  default:
  { // Turn off display [-1]
    displayOff = true;
    strip.show();
    break;
  }
  }
}

void updateLEDs(int colour, int movingPos, String customText[])
{
  if (displayOff)
    return; // Do nothing if display is off

  if (spiral && (tspiraltimer - millis() > spiralAnimationSpeed))
  {
    tspiraltimer = millis();
    strip.setPixelColor(spiralLEDPos, 0x000000); // Turn current LED off
    spiralLEDPos += 1;
    if (spiralLEDPos >= NUMPIXELS)
    {
      spiralLEDPos = 0; // Loop back to start
    }
    strip.setPixelColor(spiralLEDPos, 0x00000F); // Turn next LED on
    strip.show();
  }
  else if (displayE)
  {
    // Load imageE into RAM as red
    for (int col = 0; col < 36; col++)
    {
      for (int row = 0; row < NUMPIXELS; row++)
      {
        if (letterE[col][row] == 1)
        {
          strip.setPixelColor(row, 0x0F0000); // Red ON
        }
        else
        {
          strip.setPixelColor(row, 0x000000); // OFF
        }
      }
      strip.show();            // Display strip
      delayMicroseconds(1326); // wait for the next column in microseconds
    }
  }
  else if (displayEwithRPM)
  {
    // Load imageE into RAM as red
    for (int col = 0; col < 150; col++)
    {
      for (int row = 0; row < 150; row++)
      {
        if (letterE[col][row] == 1)
        {
          strip.setPixelColor(row, 0x0F0000); // Red ON
        }
        else
        {
          strip.setPixelColor(row, 0x000000); // OFF
        }
      }
      strip.show();                        // Display strip
      delayMicroseconds(avgRPM_delay*delta_t[0]); // wait for the next column in microseconds
    }
  }
  else if (displayMovingHI)
  {
    // Scroll the display horizontally using movingPos
    for (int col = 0; col < 36; col++)
    {
      int scrollCol = (col + movingPos) % 36; // Wrap around the 36-column array

      for (int row = 0; row < NUMPIXELS; row++)
      {
        if (letterHI[scrollCol][row] == 1)
        {
          strip.setPixelColor(row, 0x00000F); // Green ON
        }
        else
        {
          strip.setPixelColor(row, 0x000000); // OFF
        }
      }

      strip.show();            // Update LED strip
      delayMicroseconds(1326); // Wait for next column
    }
  }
  else if (displayMNS)
  {
    // Load imageMNS into RAM as red
    for (int col = 0; col < 36; col++)
    {
      for (int row = 0; row < NUMPIXELS; row++)
      {
        if (letterMNS[col][row] == 1)
        {
          strip.setPixelColor(row, 0x000F00); // Green ON
        }
        else
        {
          strip.setPixelColor(row, 0x000000); // OFF
        }
      }
      strip.show();            // Display strip
      delayMicroseconds(1326); // wait for the next column in microseconds
    }
  }
  else if (customTextDisplay)
  {
    // Custom text display logic can be added here
    for (int i = 0; i < MAX_TEXT_LINES - 1; i += 2)
    {
      String text = customText[i];
      for (int j = 0; j < text.length(); j++)
      {
        textToMatrix(String(text[j]).c_str());
        printMatrix();
        delayMicroseconds(avgRPM_delay); // Adjust delay between letters as needed
      }
    }
  }
}

void initDisplayOff()
{
  strip.begin();
  strip.clear();
  strip.show();
}