/******************************************************************************
 *
 *      File Name:      main.c
 *
 *      Owner:          Suraj, Ayush and Prathamesh
 *
 *      Project Name:   DHT11
 *
 *      @description:   This file is an example of how you can use the functions
 *                      designed in the library DHT11_LIB_PAS
 *
 *      Date Modified:  28-11-2019
 *
 ******************************************************************************/
#include <msp430.h>
#include <DHT11_PAS.h>
#include <PIR_PAS.h>

unsigned char RH_byte1;
unsigned char RH_byte2;
unsigned char T_byte1;
unsigned char T_byte2;
unsigned char checksum;

unsigned char dht_dataPacket[5];

unsigned char volatile TIMER_OUT;                        //REQUIRED for library
unsigned char volatile TIMER_WAIT=0;

void init()
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop watchdog timer

    P1OUT  = 0x00;                              // Initializing output
    P1DIR  = 0x41;                              // Setting direction for GREEN and RED LED

    BCSCTL1 = CALBC1_1MHZ;                      // Set oscillator to 1MHz
    DCOCTL = CALDCO_1MHZ;                       // Set oscillator to 1MHz

    TACCR0 = 50000;                             // Initialize the timer to count at 5Hz
    TACCTL0 = CCIE;                             // Enable interrupt
    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;    // SMCLK, div 4, up mode,
                                                // clear timer
    _enable_interrupt();                        //Enable global interrupt
}

void main(void)
{
    unsigned char check = 0;

    init();

    PIR_init();

    while(1)
    {
        //Must wait 1 second initially and between all reads
        if(TIMER_WAIT >= 5)
        {
            //0.2 second @ CCR0 = 50000 & div 4
            read_Packet(dht_dataPacket);
            RH_byte1 =  dht_dataPacket[0];
            RH_byte2 =  dht_dataPacket[1];
            T_byte1 =   dht_dataPacket[2];
            T_byte2 =   dht_dataPacket[3];
            checksum =  dht_dataPacket[4];

            if (validate_checksum(dht_dataPacket) == 1)
                BIT_SET (P1OUT, 0x40);

            BIT_SET (TACTL, TACLR);
            BIT_SET (TA0CTL, 0x10);
            TACCR0 = 50000;                   //Initialize the timer to count at 5Hz
            TIMER_WAIT = 0;                   //Clear counter
        }

        while ( (check = check_motion()) == 1)
        {
            BIT_SET (P1OUT, PIR_OUTPUT);
        }

        BIT_CLR (P1OUT, PIR_OUTPUT);
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void)
{
        TIMER_WAIT++;
        TIMER_OUT=1;
        //BIT_TOG (P1OUT,0x01);                           //Toggles GREEN led when interrupt is serviced
        BIT_CLR (TACCTL0, CCIFG);
}
