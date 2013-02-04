/*
 * PETpix2.c
 *
 * Created: 2/2/2013 9:14:06 PM
 *  Author: Michael Hill
 */ 


#include "UART_routines.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // set CA1 output
    DDRA = 0x01;
    // set low
    PORTA = 0x00;
    uart0_init(25);
    unsigned char byte;
    unsigned char buffer[1000];
    int i;
    while(1)
    {
        // set CA1 line low
        PORTA = 0x00;
        byte = 0x00;
        transmitString("waiting for buffer bytes:");
        
        
        // skip over buffer bytes
        while (byte != 0xff)
        {
            byte = receiveByte();
        }
        
        while (byte != 0x00)
        {
            byte = receiveByte();
        }

        for (i = 0; i < 1000; i++)
        {
            buffer[i] = receiveByte();
        }
        
        transmitString("got 1000 bytes\r\n");
        
        int j = 1000;
        unsigned char cv, currentVal;
        
        // done getting data
        DDRC = 0xFF;
        PORTA = 0x01;
        
        // now wait for CB2 to go low
        cv = PINA;
        cv = cv & 0x02;
        while (cv != 0x00)
        {
            cv = PINA;
            cv = cv & 0x02;
        }
        
        //transmitString("got CB2 low");
        
        // CB2 is now low
        currentVal = cv;
        for (i = 0; i < j; i++)
        {
            // output the current byte
            PORTC = buffer[i];
            
            // wait for output to toggle
            cv = PINA;
            cv = cv & 0x02;
            while (cv == 0x00)
            {
                cv = PINA;
                cv = cv & 0x02;
            }
            
            i++;
            PORTC = buffer[i];
            
            //transmitHex(INT, i);
            
            if (i < j-1)
            {
                cv = PINA;
                cv = cv & 0x02;
                while (cv != 0x00)
                {
                    cv = PINA;
                    cv = cv & 0x02;
                }
            }
            
            // the value has toggled, send next byte
        }
        
        transmitString("done sending image.");
    }
}