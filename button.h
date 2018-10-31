#ifndef button_h
#define button_h

#define BTN_DOWN 1
#define BTN_UP -1

class Button{
private:
	uint8_t m_btn_pin;
	uint8_t m_led_pin;
	uint8_t m_state;
	int8_t m_edge;
public:
	Button(uint8_t button_pin):
		m_btn_pin(button_pin),
		m_state(0),
		m_edge(0)
	{
		pinMode(m_btn_pin, INPUT_PULLUP);
	}

	void update()
	{
		bool value = !digitalRead(m_btn_pin);

		if(m_state && !value)
			m_edge = BTN_UP;
		else if(!m_state && value)
			m_edge = BTN_DOWN;
		else
			m_edge = 0;

		m_state = value;
	}

	uint8_t state(){return m_state;}
	int8_t edge(){return m_edge;}
};

#endif