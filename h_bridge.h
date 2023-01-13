/*
 * h_bridge.h - XvR 2020
 */

#ifndef _H_BRIDGE_H_
#define _H_BRIDGE_H_


    //define voor x-as
#define PORT_X_RPWM PORTH
#define PIN_X_RPWM PH3//Digital pin 6
#define DDR_X_RPWM DDRH

#define PORT_X_LPWM PORTH
#define PIN_X_LPWM PH4//Digital pin 7
#define DDR_X_LPWM DDRH

    //define voor de y-as
#define PORT_Y_RPWM PORTH
#define PIN_Y_RPWM PH5//Digital pin 8
#define DDR_Y_RPWM DDRH

#define PORT_Y_LPWM PORTH
#define PIN_Y_LPWM PH6//Digital pin 9
#define DDR_Y_LPWM DDRH

    //define voor de z-as
#define PORT_Z_RPWM PORTB
#define PIN_Z_RPWM PB4//Digital pin 10
#define DDR_Z_RPWM DDRB

#define PORT_Z_LPWM PORTB
#define PIN_Z_LPWM PB5//Digital pin 11
#define DDR_Z_LPWM DDRB

void X_as(void);
void X_as_percentage(signed char percentage);

void Y_as(void);
void Y_as_percentage(signed char percentage);

void Z_as(void);
void Z_as_percentage(signed char percentage);

#endif /* _H_BRIDGE_H_ */
