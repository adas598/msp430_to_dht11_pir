/*************************************************************************************
 *
 *      File Name:      DHT11_PAS.c
 *
 *      Owner:          Ayush
 *
 *      Project Name:   Embedded_C_PAS
 *
 *      @description:   File consists of function definitions declared in DHT11_PAS.h
 *                      useful for interfacing with main program
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/

#include <msp430.h>
#include <DHT11_PAS.h>

/*************************************************************************************
 *
 *      Owner:          Ayush
 *
 *      Function Name:  start_Signal()
 *
 *      @description:   Function is used to generate start signal to
 *                      bring DHT11 to active mode
 *
 *      @param:         void
 *
 *      Return Type:    Returns NULL
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
static void start_Signal()
{
    BIT_SET(P2DIR, DATA_PIN);           // Set Data pin to output direction
    BIT_CLR(P2OUT,DATA_PIN);            // Set output to low

    __delay_cycles(25000);              // Low for at least 18ms

    BIT_SET(P2OUT,DATA_PIN);

    __delay_cycles(30);                 // High for at 20us-40us

    BIT_CLR(P2DIR,DATA_PIN);            // Set data pin to input direction
}

/*************************************************************************************
 *
 *      File Name:      DHT11_PAS.c
 *
 *      Owner:          Ayush 
 *
 *      Function Name:  check_Response
 *
 *      @description:   To determine whether DHT11 is responding to start signal
 *                      from MCU
 *
 *      @param:         void
 *
 *      Return Type:    Returns 1 for success in response and 0 for failure
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
static unsigned char check_Response()
{
    TIMER_OUT = 0;

    BIT_SET(TACTL,TACLR);                                                   //Resets TAR, clock divider and count direction

    TACCR0 = 25;                                                            //BIT_SET timer to overflow in 100uS. 100 @ 1x divider

    BIT_SET(TACCTL0,CCIE);                                                  //And enable timer interrupt

    while ( !BIT_TEST(P2IN,DATA_PIN) && !TIMER_OUT );          //Waits for response from sensor

    if (TIMER_OUT)
    {
        return 0;
    }
    else
    {
        BIT_SET(TACTL,TACLR);                                               //Halt timer
        BIT_SET(TACCTL0,CCIE);

        while( BIT_TEST(P2IN,DATA_PIN) && (!TIMER_OUT) );     //Time for DHT to send data via Single Data Bus

        if(TIMER_OUT)
            return 0;
        else
        {
            BIT_CLR(TACCTL0,CCIE);                                          // Disable timer interrupt
            return 1;
        }
    }
}

/*************************************************************************************
 *
 *      Owner:              Ayush
 *
 *      Function Name:      read_Byte
 *
 *      @description:       Function used to read 8 bit of data from the DHT11
 *
 *      @param:     -       void
 *
 *      Return Type:        static unsigned char
 *
 *      Date Modified:      28-11-2019
 *
 **************************************************************************************/
static unsigned char read_Byte(void)
{
    TIMER_OUT = 0;

    unsigned char byteData = 0;
    unsigned char i;

    BIT_CLR(TACCTL0,CCIE);

    for (i=8; i>0; i--)
    {
        while(!(BIT_TEST(P2IN,DATA_PIN)));                           //Wait for signal to go high

        BIT_SET(TA0CTL, TACLR);                                       //Resets TAR, clock divider and count direction
        BIT_SET(TA0CTL, 0x10);                                        //Re-enable timer
        BIT_SET(TACCTL0, CCIE);                                      //Enable timer interrupt

        while(BIT_TEST(P2IN,DATA_PIN));                              //Wait for signal to go low

        BIT_CLR(TA0CTL,0x30);                                        //Halt Timer (by making MC_0: halt timer operation)

        if (TAR > 13)                                                //40 @ 1x divider
        {
            byteData |= 1 << (i-1);
        }
    }
    return byteData;
}

/*************************************************************************************
 *
 *      Owner:          Ayush
 *
 *      Function Name:  read_Packet
 *
 *      @description:   File consists of function definitions declared in DHT11_PAS.h
 *
 *      @param:         Takes a pointer of unsigned char type
 *
 *      Return Type:    Returns 1 for successful data transfer and 0 for failure
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
unsigned char read_Packet(unsigned char * data)
{
    start_Signal();

    if (check_Response())
    {
        //Cannot be done with a for loop!
        data[0] = read_Byte();
        data[1] = read_Byte();
        data[2] = read_Byte();
        data[3] = read_Byte();
        data[4] = read_Byte();
        return 1;
    }
    else
    {
        return 0;
    }
}

/*************************************************************************************
 *
 *      Owner:          Ayush
 *
 *      Function Name:  validate_checksum
 *
 *      @description:   File consists of function definitions declared in DHT11_PAS.h
 *
 *      @param:         Takes a pointer to unsigned char
 *
 *      Return Type:    Returns 0 for invalid data transfer and 1 for valid data
 *
 *      Date Modified:  28-11-2019
 *
 **************************************************************************************/
unsigned char validate_checksum(unsigned char *data){
    if (data[4] != (data[0] + data[1] + data[2] + data[3])){
        return 0;
    }
    else return 1;
}
