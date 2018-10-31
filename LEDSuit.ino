#include <FastLED.h>
#include "FastLED_RGBW.h"
#include "led_set.h"
#include "animation.h"
#include "button.h"
#include <FaBo9Axis_MPU9250.h>

#define LED_PIN_0 6
#define LED_PIN_1 5
#define LED_PIN_2 4
#define LED_PIN_3 3
#define LED_PIN_4 2

#define BUTTON_PIN 7
#define NUM_LEDS_SHORT 36
#define NUM_LEDS_LONG 45

enum body_part {
  RIGHT_ARM = 0,
  LEFT_ARM,
  RIGHT_LEG,
  LEFT_LEG,
  HEAD,
  BODY_ALL
};
CRGBW leds[NUM_LEDS_SHORT];
// CRGBW la_leds[NUM_LEDS_SHORT];
// CRGBW h_leds[NUM_LEDS_SHORT];
// CRGBW rl_leds[NUM_LEDS_SHORT];
// CRGBW ll_leds[NUM_LEDS_SHORT];
Led_Set ledSets[BODY_ALL] =
{
  Led_Set(&FastLED.addLeds<WS2812B, LED_PIN_1, RGB>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS_SHORT)), NUM_LEDS_SHORT),
  Led_Set(&FastLED.addLeds<WS2812B, LED_PIN_2, RGB>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS_SHORT)), NUM_LEDS_SHORT),
  Led_Set(&FastLED.addLeds<WS2812B, LED_PIN_3, RGB>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS_SHORT)), NUM_LEDS_SHORT),
  Led_Set(&FastLED.addLeds<WS2812B, LED_PIN_4, RGB>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS_SHORT)), NUM_LEDS_SHORT),
  Led_Set(&FastLED.addLeds<WS2812B, LED_PIN_0, RGB>((CRGB *)&leds[0], getRGBWsize(NUM_LEDS_SHORT)), NUM_LEDS_SHORT)
};

Button btn = Button(BUTTON_PIN);
FaBo9Axis fabo_9axis;

void setup() {
  fabo_9axis.begin();
}

int mode=0;
int tmp = 0;
AniMeter2 aniMeter2 = AniMeter2();
AniMeter aniMeter = AniMeter();
AniGradient aniGradient = AniGradient();
AniStandingWave aniStandingWave = AniStandingWave();
AniRainbowRain aniRainbowRain = AniRainbowRain();
AniDualParticle aniDualParticle = AniDualParticle();
AniMotionParticle aniMotionParticle = AniMotionParticle();
AniSineParticle aniSineParticle = AniSineParticle();
AniCharge aniCharge = AniCharge();
// the loop function runs over and over again forever
void loop() {
  float ax,ay,az;

  fabo_9axis.readAccelXYZ(&ax,&ay,&az);

  unsigned long cur_time = millis();
  btn.update();
  if (btn.edge() == 1)
    mode = (mode+1)%14;

  switch(mode) {
    case 0:
      for(int i=0; i<BODY_ALL; i++) {
        ledSets[i].reset();
        ledSets[i].showLeds();
      }
      delay(100);
      break;
    case 1:
      aniCharge.m_speed = -ax*2;
      aniCharge.update(cur_time);
      aniCharge.draw(ledSets[0]);
      for(int i=0; i<BODY_ALL; i++) {
        ledSets[i].showLeds();
      }
      break;
    case 2:
      aniSineParticle.update(cur_time);
      for(int i=0; i<BODY_ALL; i++) {
        aniSineParticle.m_progessOffset = i*200;
        aniSineParticle.draw(ledSets[i]);
        ledSets[i].showLeds();
      }
      break;
    case 3:
      aniGradient.update(cur_time);
      for(int i=0; i<BODY_ALL; i++) {
        aniGradient.m_progessOffset = i*200;
        aniGradient.draw(ledSets[i]);
        ledSets[i].showLeds();
      }
      break;
    case 4:
      aniMeter.update(cur_time);
      aniMeter.draw(ledSets[0]);
      for(int i=0; i<HEAD; i++) {
        ledSets[i].showLeds();
      }
      aniStandingWave.update(cur_time);
      aniStandingWave.draw(ledSets[0]);
      ledSets[HEAD].showLeds();
      break;
    case 5:
      ledSets[0].drawRandom(3);
      ledSets[0].showLeds();
      ledSets[1].showLeds();
      ledSets[0].shift(12);
      ledSets[2].showLeds();
      ledSets[3].showLeds();
      ledSets[0].shift(12);
      ledSets[4].showLeds();
      ledSets[0].shift(12);
      break;
    case 6:
      {
        CRGB color;
        if (ax > -0.1  && ax < 0.1) ax = 0;
        else if (ax >= 0.1) ax -= 0.05;
        else if (ax <= -0.1) ax += 0.05;
        float ax1 = constrain(-ax, 0.0, 1.0);
        float ax2 = constrain(ax, 0.0, 1.0);
        color.r = (uint8_t)(ax1*100);
        color.g = (uint8_t)(ax2*100);
        color.b = 0;
        for(int i=0; i<BODY_ALL; i++) {
          ledSets[i].drawColor(color);
          ledSets[i].showLeds();
        }
      }
      break;
    case 7:
      {
        CRGB colorSet[5] =     {CRGB::Yellow, CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Purple};
        body_part bodySet[5] = {RIGHT_ARM,    LEFT_ARM,  RIGHT_LEG,  HEAD,        LEFT_LEG};
        if (tmp >= BODY_ALL) tmp = 0;
        for(int i=0; i<BODY_ALL; i++) {
          if (tmp == i)
            ledSets[bodySet[i]].drawColor(colorSet[i]);
          else
            ledSets[bodySet[i]].clear();
          ledSets[bodySet[i]].showLeds();
        }
        tmp++;
        delay(100);
        break;
      }
    case 8:
      aniStandingWave.update(cur_time);
      aniStandingWave.draw(ledSets[0]);
      for(int i=0; i<BODY_ALL; i++) {
        ledSets[i].showLeds();
      }
      break;
    case 9:
      aniMotionParticle.m_speed = -ax*2;
      aniMotionParticle.update(cur_time);
      aniMotionParticle.draw(ledSets[0]);
      for(int i=0; i<BODY_ALL; i++) {
        ledSets[i].showLeds();
      }
      break;
    case 10:
      aniMeter2.update(cur_time);
      for(int i=0; i<BODY_ALL; i++) {
        aniMeter2.m_progessOffset = i*1351;
        aniMeter2.draw(ledSets[i]);
        ledSets[i].showLeds();
      }
      break;
    case 11:
      aniRainbowRain.update(cur_time);
      for(int i=0; i<BODY_ALL; i++) {
        aniRainbowRain.m_progessOffset = i*600;
        aniRainbowRain.draw(ledSets[i]);
        ledSets[i].showLeds();
      }
      break;
    case 12:
      aniDualParticle.update(cur_time);
      for(int i=0; i<BODY_ALL; i++) {
        aniDualParticle.m_progessOffset = i*600;
        aniDualParticle.draw(ledSets[i]);
        ledSets[i].showLeds();
      }
      break;
    case 13:
      {
        Animation* anis[5] = {&aniStandingWave, &aniStandingWave, &aniRainbowRain, &aniRainbowRain, &aniDualParticle};
        for(int i=0; i<BODY_ALL; i++) {
          anis[i]->update(cur_time);
          anis[i]->draw(ledSets[i]);
          ledSets[i].showLeds();
        }
        break;
      }
  }
}
