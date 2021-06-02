/*************************************************************************************
 *
 *      File Name:      DHT11_PAS.h
 *
 *      Owner:          Suraj, Ayush and Prathamesh
 *
 *      Project Name:   Embedded_C_PAS
 *
 *      @description:   Header file consisting of functions and
 *                      macros at the disposal for the user to interface
 *                      MSP430 with DHT11
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
#ifndef DHT11_PAS_H
#define DHT11_PAS_H

#define BIT_TEST(x,y)       (x & (y))
#define BIT_SET(x,y)        (x|= (y))
#define BIT_CLR(x,y)        (x &= ~(y))
#define BIT_TOG(x,y)        (x ^= (y))

#define DATA_PIN            BIT4                               //User can change pin value to his/her requirement

extern unsigned char volatile TIMER_OUT;

unsigned char read_Packet(unsigned char *);                    //Pass an array with 5 elements (5x8 = 40 bits)
unsigned char validate_checksum(unsigned char *);              //Pass an array with 5 elements (5x8 = 40 bits)

#endif /* DHT11_PAS_H */
