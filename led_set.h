#ifndef led_set_h
#define led_set_h

class Led_Set{
private:
	CLEDController *m_controller;
	CRGBW *m_leds;
	int m_numLeds;
	int m_drawOffset;
	int m_viewport[2];
	bool m_wrap;

	void swap(CRGBW* a, CRGBW* b)
	{
		CRGBW temp = *a;
		*a = *b;
		*b = temp;
	}

	void reverseArray(int startIndex, int endIndex) {
		int i;
		CRGBW* start= &m_leds[startIndex];
		int size = endIndex-startIndex+1;
		for(i=0; i<size/2; i++)
			swap(&start[i], &start[size-1-i]);
	}
public:
	Led_Set(CLEDController *controller, int num_leds):
		m_controller(controller),
		m_numLeds(num_leds)
	{
		m_leds = (CRGBW*)controller->leds();
		reset();
	}

	void setViewport(int pos, int size) {
		if (pos < 0 || pos >= m_numLeds)
			return;

		size = constrain(size, -m_numLeds, m_numLeds);
		if (size == 0) {
			m_viewport[0] = m_numLeds;
			m_viewport[1] = m_numLeds+1;
		} else if (size >= 0) {
			m_viewport[0] = pos;
			m_viewport[1] = (pos + size)%m_numLeds;
		} else {
			m_viewport[0] = (pos + size)%m_numLeds;
			m_viewport[1] = pos;
		}
	}
	void setOffset(int offset) {m_drawOffset = offset;}
	void setWrap(bool wrap) {m_wrap = wrap;}

	void setLED(int pos, CRGB color)
	{
		int element = pos+m_drawOffset;

		if (m_wrap) {
			element = element%m_numLeds;
			if (element < 0)
				element += m_numLeds;
		} else {
			if( element < 0 || element >= m_numLeds)
				return;
		}

		if ((m_viewport[0] == m_viewport[1]) ||
			(m_viewport[1] > m_viewport[0] && (element >= m_viewport[0] && element <= m_viewport[1]))
			 || (m_viewport[1] < m_viewport[0] && (element >= m_viewport[0] || element <= m_viewport[1]))
			){
			m_leds[element] = color;
		}
	}

	void reset()
	{
		setViewport(0, m_numLeds);
		setOffset(0);
		setWrap(true);
		for(int j = 0; j < m_numLeds; j++)
			m_leds[j] = CRGB::Black;
	}

	void clear()
	{
		drawColor(CRGB::Black);
	}

	void drawColor(CRGB color)
	{
		for(int j = 0; j < m_numLeds; j++)
			setLED(j, color);
	}

	void drawGradient(uint16_t hue_from, uint16_t hue_to, uint8_t size) {
	
		for (uint8_t i=0; i< size; i++) {
			uint8_t hue =  lerp16by8(hue_from, hue_to, 255*i/(size-1));
			setLED(i, CHSV(hue, 255, 128));
		}
	}

	void drawRandom(uint8_t num)
	{
		for(uint8_t i=0; i<num; i++) {
			uint8_t rand = random8(m_numLeds);
			setLED(rand, CHSV(random8(), 255, 128));
		}
	}

	void drawTriangle(uint8_t size, uint8_t hue) {
		for(uint8_t i=0; i < size; i++) {
			CRGB color = CHSV(hue, 255, 255*(size-i)/size);
			setLED( i, color);
			setLED(-i, color);
		}
	}

	void drawStreak(uint8_t size, uint8_t hue, bool fowards) {
		for(uint8_t i=0; i < size; i++) {
			CRGB color = CHSV(hue, 255, 255*(size-i)/size);
			setLED(fowards?i:-i, color);
		}
	}

	void shift(int m) {
		reverseArray(0, m_numLeds-1);
		reverseArray(0, m - 1);
		reverseArray(m, m_numLeds-1);
	}

	CRGBW* leds(){ return m_leds;}
	uint8_t numLeds(){ return m_numLeds;}
	void showLeds() { m_controller->showLeds(); }
};

#endif