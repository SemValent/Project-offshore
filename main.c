#ifndef F_CPU
#define F_CPU 16000000ul
#endif
#include <util/delay.h>
#include "h_bridge.h"
#include <avr/io.h>

#define DEBOUNCE_PERIOD_MS 15

volatile int Z_check = 0;
volatile int X_cordinaat = 0;
volatile int Y_cordinaat = 0;

void init_knoppen(void){

 DDRF &= ~(1<<PF0);  // A0 input (button 1)
 DDRF &= ~(1<<PF1);  // A1 input (joystick up)
 DDRF &= ~(1<<PF2);  // A2 input (joystick left)
 DDRF &= ~(1<<PF3);  // A3 input (noodknop)
 DDRF &= ~(1<<PF4);  // A4 input (joystick right)
 DDRF &= ~(1<<PF5);  // A5 input (joystick down)
 DDRK &= ~(1<<PK1);  // A9 input (limit switch z_as)
 DDRK &= ~(1<<PK2);  // A10 input (limit switch x_as)
 DDRK &= ~(1<<PK6);  // A14 input (button 2)
 DDRK &= ~(1<<PK7);   //A15 input (button 3)
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

    TCNT3 = 0;
    TIFR3 = (1<<TOV3);

    while (timer < time){

            if (TIFR3 & (1<<TOV3))
            {
                TIFR3 = (1<<TOV3);
                timer++;
            }
        }
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
    if (PINK &(1<<PK6)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK6)) return 1;
        }
        return 0;
}


int joystick_up(void)
{
    if (PINF &(1<<PF1)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINF &(1<<PF1)) return 1;
        }
        return 0;
}

int joystick_left(void)
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
int joystick_right(void)
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


 int limit_switch_Xas(void)
{
    if (PINK &(1<<PK1)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK1)) return 1;
        }
        return 0;
}

 int limit_switch_Yas(void)
{
    if (PINK &(1<<PK2)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK2)) return 1;
        }
        return 0;
}



int button3_pressed(void)
{
    if (PINK &(1<<PK7)){
    _delay_ms (DEBOUNCE_PERIOD_MS);
        if (PINK &(1<<PK7)) return 1;
        }
        return 0;
}


void home_Xas(void)
{
    while (limit_switch_Xas()){

		X_as_percentage(-100);

     }

    if (!limit_switch_Xas()){
        X_as_percentage(0);
    }
    X_cordinaat = 0;

}

void home_Yas(void)
{
    while (limit_switch_Yas()){

		Y_as_percentage(100);

     }

    if (!limit_switch_Yas()){
        Y_as_percentage(0);
    }
}

void pak_ton(void){

    if (Z_check == 0){

    Z_as_percentage(-100);
    delay(19);
    Z_as_percentage(0);

    PORTK |= (1<<PK3);
    delay(2);

    Z_as_percentage(100);
    delay(19);
    Z_as_percentage(0);

    Z_check = 1;
    }

    else if (Z_check == 1){

    Z_as_percentage(-100);
    delay(19);
    Z_as_percentage(0);

    PORTK &= ~(1<<PK3);
    delay(2);

    Z_as_percentage(100);
    delay(19);
    Z_as_percentage(0);

    Z_check = 0;
    }

}





int main(void)
{
	init();


    while(1){

        if (!nood_knop()){


        if (button1_pressed()){
            home_Xas();
            home_Yas();
        }

        if (!button2_pressed()){
            pak_ton();
        }

        if (!button3_pressed()){
            Z_as_percentage(100);
        }

        if (!joystick_right()){

            if (X_cordinaat > 0)
            X_as_percentage(-100);
            delay(1);
            X_cordinaat --;

        }

        if (!joystick_left()){

            if (X_cordinaat < 22){
            X_as_percentage(100);
            delay(1);
            X_cordinaat ++;
            }
        }

        if (!joystick_down()){
            Y_as_percentage(100);
            delay(1);
        }

        if (!joystick_up()){
            Y_as_percentage(-100);
            delay(1);
        }

        else{
            X_as_percentage(0);
            Y_as_percentage(0);
            Z_as_percentage(0);
        }

        }




    }
    return 0;
}


