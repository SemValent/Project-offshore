
#include <avr/io.h>
#include <avr/interrupt.h>
#include "h_bridge.h"

//X as
ISR(TIMER0_OVF_vect)
{
	if (OCR0A == 0 && OCR0B == 0)
	{
		PORT_X_RPWM &= ~(1<<PIN_X_RPWM);
		PORT_X_LPWM &= ~(1<<PIN_X_LPWM);
	}
	else if (OCR0A != 0)
	{
		PORT_X_LPWM &= ~(1<<PIN_X_LPWM);
		PORT_X_RPWM |= (1<<PIN_X_RPWM);
	}
	else if (OCR0B != 0)
	{
		PORT_X_RPWM &= ~(1<<PIN_X_RPWM);
		PORT_X_LPWM |= (1<<PIN_X_LPWM);
	}
}

ISR(TIMER0_COMPA_vect)
{
	if (OCR0A != 255)
	{
		PORT_X_RPWM &= ~(1<<PIN_X_RPWM);
	}
}

ISR(TIMER0_COMPB_vect)
{
	if (OCR0B != 255)
	{
		PORT_X_LPWM &= ~(1<<PIN_X_LPWM);
	}
}
//Y as
ISR(TIMER2_OVF_vect)
{
    if (OCR2A == 0 && OCR2B == 0)
    {
        PORT_Y_RPWM &= ~(1<<PIN_Y_RPWM);
        PORT_Y_LPWM &= ~(1<<PIN_Y_LPWM);
    }
    else if (OCR2A != 0)
    {
        PORT_Y_LPWM &= ~(1<<PIN_Y_LPWM);
        PORT_Y_RPWM |= (1<<PIN_Y_RPWM);
    }
    else if (OCR2B != 0)
    {
        PORT_Y_RPWM &= ~(1<<PIN_Y_RPWM);
        PORT_Y_LPWM |= (1<<PIN_Y_LPWM);
    }
}

ISR(TIMER2_COMPA_vect)
{
    if (OCR2A != 255)
    {
        PORT_Y_RPWM &= ~(1<<PIN_Y_RPWM);
    }
}

ISR(TIMER2_COMPB_vect)
{
    if (OCR2B != 255)
    {
        PORT_Y_LPWM &= ~(1<<PIN_Y_LPWM);
    }
}

//Z as
ISR(TIMER1_OVF_vect)
{
    if (OCR1A == 0 && OCR1B == 0)
    {
        PORT_Z_RPWM &= ~(1<<PIN_Z_RPWM);
        PORT_Z_LPWM &= ~(1<<PIN_Z_LPWM);
    }
    else if (OCR1A != 0)
    {
        PORT_Z_LPWM &= ~(1<<PIN_Z_LPWM);
        PORT_Z_RPWM |= (1<<PIN_Z_RPWM);
    }
    else if (OCR1B != 0)
    {
        PORT_Z_RPWM &= ~(1<<PIN_Z_RPWM);
        PORT_Z_LPWM |= (1<<PIN_Z_LPWM);
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (OCR1A != 65535)
    {
        PORT_Z_RPWM &= ~(1<<PIN_Z_RPWM);
    }
}

ISR(TIMER1_COMPB_vect)
{
    if (OCR1B != 65535)
    {
        PORT_Z_LPWM &= ~(1<<PIN_Z_LPWM);
    }
}


void init_X_as(void)
{
	// Config pins as output
    DDR_X_RPWM |= (1<<PIN_X_RPWM);
	DDR_X_LPWM |= (1<<PIN_X_LPWM);

	// Output low
	PORT_X_RPWM &= ~(1<<PIN_X_RPWM);
	PORT_X_LPWM &= ~(1<<PIN_X_LPWM);

	// Use mode 0, clkdiv = 64
	TCCR0A = 0;
	TCCR0B = (0<<CS02) | (1<<CS01) | (1<<CS00);

	// Disable PWM output
	OCR0A = 0;
	OCR0B = 0;

	// Interrupts on OCA, OCB and OVF
	TIMSK0 = (1<<OCIE0B) | (1<<OCIE0A) | (1<<TOIE0);

	sei();
}

void init_Y_as(void)
{
    // Config pins as output
    DDR_Y_RPWM |= (1<<PIN_Y_RPWM);
    DDR_Y_LPWM |= (1<<PIN_Y_LPWM);

    // Output low
    PORT_Y_RPWM &= ~(1<<PIN_Y_RPWM);
    PORT_Y_LPWM &= ~(1<<PIN_Y_LPWM);

    // Use mode 0, clkdiv = 64
    TCCR2A = 0;
    TCCR2B = (0<<CS22) | (1<<CS21) | (1<<CS20);

    // Disable PWM output
    OCR2A = 0;
    OCR2B = 0;

    // Interrupts on OCA, OCB and OVF
    TIMSK2 = (1<<OCIE2B) | (1<<OCIE2A) | (1<<TOIE2);

    sei();
}

void init_Z_as(void)
{
    // Config pins as output
    DDR_Z_RPWM |= (1<<PIN_Z_RPWM);
    DDR_Z_LPWM |= (1<<PIN_Z_LPWM);

    // Output low
    PORT_Z_RPWM &= ~(1<<PIN_Z_RPWM);
    PORT_Z_LPWM &= ~(1<<PIN_Z_LPWM);

    // Use mode 0, clkdiv = 8
    TCCR1A = 0;
    TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10);

    // Disable PWM output
    OCR1A = 0;
    OCR1B = 0;

    // Interrupts on OCA, OCB and OVF
    TIMSK1 = (1<<OCIE1B) | (1<<OCIE1A) | (1<<TOIE1);

    sei();
}

void X_as_percentage(signed char percentage)
{
	if (percentage >= -100 && percentage <= 100)
	{
		if (percentage >= 0)
		{
			// Disable LPWM, calculate RPWM
			OCR0B = 0;
			OCR0A = (255*percentage)/100;
		}
		else // percentage < 0
		{
			// Disable RPWM, calculate LPWM
			OCR0A = 0;
			OCR0B = (255*percentage)/-100;
		}
	}
}

void Y_as_percentage(signed char percentage)
{
    if (percentage >= -100 && percentage <= 100)
    {
        if (percentage >= 0)
        {
            // Disable LPWM, calculate RPWM
            OCR2B = 0;
            OCR2A = (255*percentage)/100;
        }
        else // percentage < 0
        {
            // Disable RPWM, calculate LPWM
            OCR2A = 0;
            OCR2B = (255*percentage)/-100;
        }
    }
}

void Z_as_percentage(signed char percentage)
{
    if (percentage >= -100 && percentage <= 100)
    {
        if (percentage >= 0)
        {
            // Disable LPWM, calculate RPWM
            OCR1B = 0;
            OCR1A = (65535*percentage)/100;
        }
        else // percentage < 0
        {
            // Disable RPWM, calculate LPWM
            OCR1A = 0;
            OCR1B = (65535*percentage)/-100;
        }
    }
}


