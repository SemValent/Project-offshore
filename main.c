#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include <util/delay.h>
#include "h_bridge.h"
#include <avr/io.h>

#define DEBOUNCE_PERIOD_MS 15

volatile int Y_check = 0;

void init_knoppen(void){

 DDRF &= ~(1<<PF0);  // A0 input (button 1)
 DDRF &= ~(1<<PF1);  // A1 input (button 2)
 DDRF &= ~(1<<PF2);  // A2 input (button 3)
 DDRF &= ~(1<<PF3);  // A3 input (noodknop)
 DDRF &= ~(1<<PF4);  // A4 input (joystick up)
 DDRF &= ~(1<<PF5);  // A5 input (joystick down)
 DDRK &= ~(1<<PK1);  // A9 input (limit switch z_as)
 DDRK &= ~(1<<PK2);  // A10 input (limit switch x_as)

}
void init_leds(void){
 DDRF |= (1<< PF7); // a7 outpout (led)
 DDRK |= (1<< PK3); // a11 outpout(magneet)
 DDRK |= (1<< PK5); // a13 output (buzzer)
}

void init_timer(void){
    TCCR3A = 0;
    TCCR3B = (0<<CS32) | (1<<CS31) | (1<<CS30);
}

void init(void)
{
	init_X_as();
	init_Y_as();
	init_Z_as();
	init_knoppen();
	init_leds();
	init_timer();
}


void delay(int time){
    int timer = 0;

    while (timer < time){

            if (TIFR3 & (1<<TOV3))
            {
                TIFR3 = (1<<TOV3);
                timer++;
            }
        }
}


int main(void)
{
	init();
	 PORTF = 0b00000000;

    X_as_percentage(0);

    while(1){



    if (button1_pressed()){

		X_as_percentage(100);
		delay(5);
		X_as_percentage(0);

     }

     if (!button2_pressed()){
        X_as_percentage(-100);
        delay(5);
        X_as_percentage(0);
     }

    }

    return 0;
}



 int button1_pressed(void)
{
    if (PINF &(1<<PF0)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF0)) return 1;
        }
        return 0;
}

int button2_pressed(void)
{
    if (PINF &(1<<PF1)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF1)) return 1;
        }
        return 0;
}

int button3_pressed(void)
{
    if (PINF &(1<<PF2)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF2)) return 1;
        }
        return 0;
}

int nood_knop(void)
{
    if (PINF &(1<<PF3)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF3)) return 1;
        }
        return 0;
}
int joystick_up(void)
{
    if (PINF &(1<<PF4)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF4)) return 1;
        }
        return 0;
}
int joystick_down(void)
{
    if (PINF &(1<<PF5)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF5)) return 1;
        }
        return 0;
}

 int limit_switch_Zas(void)
{
    if (PINK &(1<<PK1)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK1)) return 1;
        }
        return 0;
}

 int limit_switch_Xas(void)
{
    if (PINK &(1<<PK2)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK2)) return 1;
        }
        return 0;


//reset
if (button1_pressed()){

    while (limit_switch_Xas()){
        X_as_percentage(100);
    }
    X_as_percentage(0);

    while(limit_switch_Zas()){
        Z_as_percentage(100);
    }
    Z_as_percentage(0);
}

//Y_as
if (button3_pressed){

    if (Y_check == 1){
        Y_as_percentage(-100);
        delay(10);
        Y_as_percentage(0);

        PORTK = (0<<PK3);
        delay(10);

        Y_as_percentage(100);
        delay(10);
        Y_as_percentage(0);

        Y_check = 0;
    }

    else if (Y_check == 0){
        Y_as_percentage(-100);
        delay(10);
        Y_as_percentage(0);

        PORTK = (1<<PK3);
        delay(10);

        Y_as_percentage(100);
        delay(10);
        Y_as_percentage(0);

        Y_check = 1;
    }

}

//x_as up
if(joystick_up){
    X_as_percentage(-100);
    delay(10);
    X_as_percentage(0);
}

//x_as down
if(joystick_down){
    X_as_percentage(100);
    delay(10);
    X_as_percentage(0);
}

//auto
if(button2_pressed()){
    //reset
    while (!limit_switch_Xas()){
        X_as_percentage(100);
    }
    X_as_percentage(0);

    while(!limit_switch_Zas()){
        Z_as_percentage(100);
    }
    Z_as_percentage(0);

    //X_as naar positie
    //...
}



	return 0;
}
