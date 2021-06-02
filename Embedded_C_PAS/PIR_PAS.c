/*************************************************************************************
 *
 *      File Name:      PIR_PAS.c
 *
 *      Owner:          Ayush
 *
 *      Project Name:   Embedded_C_PAS
 *
 *      @description:   File consists of function definitions declared in PIR_PAS.h
 *                      useful for interfacing with main program
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
#include <msp430.h>
#include <PIR_PAS.h>


/*************************************************************************************
 *
 *      Owner:          Ayush
 *
 *      Function Name:  PIR_init()
 *
 *      @description:   Function is used initialize directions and outputs
 *
 *      @param:         void
 *
 *      Return Type:    Returns NULL
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
void PIR_init (void)
{
    BIT_SET(P1DIR, PIR_OUTPUT);  //  setting GREEN on-board LED as output
    BIT_CLR(P1DIR, PIR_INPUT);   //  setting input direction for the data pin connection from PIR
    BIT_CLR(P1OUT, PIR_OUTPUT);  //  initializing LED to logic LOW
}

/*************************************************************************************
 *
 *      Owner:          Ayush
 *
 *      Function Name:  check_motion()
 *
 *      @description:   Function is used to check for motion/body
 *                      emitting IR Radiation
 *
 *      @param:         void
 *
 *      Return Type:    Returns 1 for success in detecting body else 0 for
 *                      failure
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
unsigned char check_motion (void)
{
    if ((P1IN & PIR_INPUT) == PIR_INPUT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



