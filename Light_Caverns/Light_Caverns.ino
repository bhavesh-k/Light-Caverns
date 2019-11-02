#include <Adafruit_NeoPixel.h>

#define PIN                    6
#define NUMPIXELS              60
#define NUM_SUBSTRIPS          10
#define PIXELS_PER_SUBSTRIP    6

#define POST_GREEN         25000/*32000*/
#define PRE_GREEN          6000/*50000*/

#define DELAY_MS         50 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint16_t hue = POST_GREEN;
bool hueDirectionUp = true;

uint8_t value = 64;
bool breatheDirectionUp = true;
bool breathePlateau = false;
uint8_t plateauTime = 0;

uint8_t saturation = 255;

void setup()
{
  pixels.begin();
}

void loop()
{
  // per-pixel hue calculations
  for(int i = 0; i < NUM_SUBSTRIPS; i++)
  {
    pixels.fill(pixels.gamma32(pixels.ColorHSV(hue + i * 1000, saturation, value)), i * PIXELS_PER_SUBSTRIP, PIXELS_PER_SUBSTRIP);
  }
  
  pixels.show();

  // color change logic
  if(hueDirectionUp)
  {
    hue += 100;
    if(hue > PRE_GREEN && hue - PRE_GREEN < 200)
      hueDirectionUp = false;
  }
  else
  {
    hue -= 100;
    if(hue < POST_GREEN && POST_GREEN - hue < 200)
      hueDirectionUp = true;
  }

  // brightness breathe logic
  if(breatheDirectionUp)
  {
    value += 4;
    if(value >= 252)
    {
      breatheDirectionUp = false;
      breathePlateau = true;
    }
  }
  else
  {
    if(breathePlateau)
    {
      plateauTime++;
      if(plateauTime > 200)
      {
        breathePlateau = false;
        plateauTime = 0;
      }
    }
    else
    {
      value -= 4;
      if(value <= 152)
        breatheDirectionUp = true;
    }
  }

  // saturation logic to avoid harsh reds
  if(hue < 4000)
    saturation = 255 - (uint8_t)(127.0 * (4000.0 - hue)/4000.0);
  else if(hue > 50535)
    saturation = 255 - (uint8_t)(127.0 * (hue - 50535.0)/15000.0);
  else
    saturation = 255;
  
  delay(DELAY_MS); // Pause before next pass through loop
}
