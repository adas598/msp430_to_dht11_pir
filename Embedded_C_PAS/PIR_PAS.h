/*************************************************************************************
 *
 *      File Name:      PIR_PAS.h
 *
 *      Owner:          Suraj, Ayush and Prathamesh
 *
 *      Project Name:   Embedded_C_PAS
 *
 *      @description:   Header file consisting of functions and
 *                      macros at the disposal for the user to interface
 *                      MSP430 with PIR motion sensor
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/

#ifndef PIR_PAS_H_
#define PIR_PAS_H_

#define PIR_INPUT 0x08       //  (BIT3) User can change according to his/her requirements
#define PIR_OUTPUT 0x01      //  (BIT0) User can change according to his/her requirements

#define BIT_TEST(x,y)       (x & (y))
#define BIT_SET(x,y)        (x|= (y))
#define BIT_CLR(x,y)        (x &= ~(y))
#define BIT_TOG(x,y)        (x ^= (y))

void PIR_init(void);
unsigned char check_motion(void);

#endif /* PIR_PAS_H_ */
