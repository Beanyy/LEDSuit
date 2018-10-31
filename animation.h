#ifndef animation_h
#define animation_h

long mapWrap(long x, long in_min, long in_max, long out_min, long out_max)
{
	return map(x%in_max, 0, in_max-1, out_min, out_max);
}
class Animation {
private:
	long m_progress;
	unsigned long m_last_time;
public:
	Animation():
		m_duration(0),
		m_speed(1.0f)
	{
		reset();
	}

	long m_duration; //Try to make this a number with many factors.
	float m_speed;
	int m_progessOffset;

	virtual void draw(Led_Set &leds) = 0;
	void update(unsigned long time) {
		m_progress += (int)((time - m_last_time)*m_speed);
		m_progress = m_progress%m_duration;
		if (m_progress < 0) m_progress+=m_duration;
		m_last_time = time;
	}

	void reset() {
		m_progress = 0;
		m_progessOffset = 0;
		m_last_time = millis();
	}

	long progress() {
		long p = m_progress;
		if (m_progessOffset) {
			p = (p+m_progessOffset)%m_duration;
			if (p < 0) p+=m_duration;
		}
		return p;
	}
};
class AniMeter : public Animation {
public:
	AniMeter():
		Animation()
	{m_duration = 1000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size*2-1);
		if (offset <= size) {
			leds.setViewport(0, offset);
		} else {
			int blank_offset = offset%size;
			leds.setViewport(blank_offset, size-blank_offset-1);
		}
		leds.drawGradient(0, 254, size);
	}
};

class AniMeter2 : public Animation {
public:
	AniMeter2():
		Animation()
	{m_duration = 8000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size*8-1);
		uint8_t hue = (offset/size)*32;
		leds.drawColor(CHSV(hue, 255, 128));
		leds.setViewport(0, offset%size);
		leds.drawColor(CHSV(hue+32, 255, 128));
	}
};

class AniGradient : public Animation {
public:
	AniGradient():
		Animation()
	{m_duration = 1000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size-1);
		leds.setOffset(offset);
		leds.drawGradient(HUE_BLUE, HUE_PINK, 8);
		leds.setOffset(offset + size/2);
		leds.drawGradient(HUE_BLUE, HUE_PINK, 8);
	}
};

class AniStandingWave : public Animation {
public:
	AniStandingWave():
		Animation(),
		m_waveNumber(2)
	{m_duration = 1500;}

	uint8_t m_waveNumber;
	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, 255);
		uint8_t wavelength = size/m_waveNumber;
		for (uint8_t i=0; i<wavelength; i++) {
			int8_t amplitude = 127 - cos8(255*i/wavelength);
			uint8_t wavePosition = sin8(offset);
			int8_t phase = 127 - wavePosition;
			for (uint8_t j=0; j<m_waveNumber; j++)
				leds.setLED(i + wavelength*j, CHSV(map8(wavePosition, HUE_AQUA, HUE_RED-1), 255, amplitude*phase/128 + 128));
		}
	}
};

class AniRainbowRain : public Animation {
public:
	AniRainbowRain():
		Animation()
	{m_duration = 3000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size*3);
		int offset2 = map(progress(), 0, m_duration, 0, size*4);
		uint8_t hue = map(progress(), 0, m_duration, 0, 255);
		uint8_t hue2 = map(progress(), 0, m_duration, 0, 511);
		leds.setOffset(offset);
		leds.drawTriangle(4, hue);
		leds.setOffset(offset2);
		leds.drawTriangle(4, hue2);
	}
};

class AniDualParticle : public Animation {
public:
	AniDualParticle():
		Animation()
	{m_duration = 3000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size*3);
		int offset2 = map(progress(), 0, m_duration, 0, size*4);
		uint8_t hue = map(progress(), 0, m_duration, HUE_RED, HUE_GREEN);
		uint8_t hue2 = mapWrap(progress(), 0, m_duration/2, HUE_RED, HUE_GREEN);
		leds.setOffset(offset);
		leds.drawTriangle(4, hue);
		leds.setOffset(-offset2);
		leds.drawTriangle(4, hue2);
	}
};

class AniMotionParticle : public Animation {
public:
	AniMotionParticle():
		Animation()
	{m_duration = 1000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		int offset = map(progress(), 0, m_duration, 0, size);
		leds.setOffset(offset);
		leds.drawGradient(HUE_YELLOW, HUE_BLUE, 8);
		leds.setOffset(offset + size/2);
		leds.drawGradient(HUE_YELLOW, HUE_BLUE, 8);
	}
};

class AniSineParticle : public Animation {
public:
	AniSineParticle():
		Animation()
	{m_duration = 6000;}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		uint8_t hue = mapWrap(progress(), 0, m_duration/2, 0, 255);
		int sinArg = mapWrap(progress(), 0, m_duration/3, 0, 255);
		int offset = map(sin8(sinArg), 0, 255, 0, size);

		leds.setOffset(offset);
		leds.drawTriangle(5, hue);
		leds.setOffset(offset + size/2);
		leds.drawTriangle(5, hue);
	}
};


class AniCharge : public Animation {
private:
	//Need longs here to prevent overflow
	const long m_maxTicks = 70;
	const long m_tickDuration = 500; //Make sure m_tickDuration is a multiple of m_maxTicks
	const long m_fullChargeDuration = 5000;
public:
	AniCharge():
		Animation()
	{
		m_duration = m_fullChargeDuration + m_tickDuration*m_maxTicks/2;
	}

	void draw(Led_Set &leds) override {
		int size = leds.numLeds();
		leds.reset();

		long p = progress();
		int offset;

		if(p < m_tickDuration*m_maxTicks/2){
			int ticks=0;
			while(ticks<m_maxTicks) {
				int tmp = p - m_tickDuration*(m_maxTicks-ticks)/m_maxTicks;
				if (tmp <= 0) break;
				p = tmp;
				ticks++;
			}

			int viewport_size = map(ticks, 0, m_maxTicks, 0, size);
			leds.setViewport(0, viewport_size);
			leds.drawGradient(0, 255, size);

			int offset = map(p, 0, m_tickDuration, size-1, 0);
			uint8_t hue = map(offset, 0, size-1, 0, 255);
			leds.setViewport(0, size);
			leds.setOffset(offset);
			leds.drawTriangle(3, hue);
		} else {
			p -= m_tickDuration*m_maxTicks/2;
			offset = map(p, 0, m_fullChargeDuration, 0, size*20);
			leds.setOffset(-offset);
			leds.drawGradient(0, 255, size);
		}
	}
};
#endif