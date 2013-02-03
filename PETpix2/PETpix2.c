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
    uart0_init(25);
    unsigned char byte;
    unsigned char buffer[1000];
    int i;
    while(1)
    {
        for (i = 0; i < 1000; i++)
        {
            buffer[i] = receiveByte();
        }
        
        unsigned char calib_start = 0;
        for (i = 0; i < 1000; i++)
        {
            if (calib_start == 0)
            {
                if (buffer[i] == 0xff)
                {
                    calib_start = 1;
                }
            }
            else if (calib_start == 1)
            {
                // wait for 0 value
                if (buffer[i] == 0x00)
                {
                    calib_start = 2;
                }
            }
            
            if (calib_start == 2)
            {
                //if (i < 100)
                //{
                    transmitHex(CHAR, buffer[i]);
                    transmitString(" ");
                    transmitHex(INT, i);
                    
                    if (i > 0)
                    {
                        if (buffer[i] != buffer[i-1]+1)
                        {
                            transmitString(" **");
                        }
                    }
                    
                    transmitString("\r\n");
                    _delay_loop_2(65535);
                //}
            }
            
            
        }
        
        /*
        int res;
        int start_offset = -1;
        for (i = 0; i < 1000; i++)
        {
            if (start_offset == -1)
            {
                if (buffer[i] == 0x00)
                {
                    start_offset = i;
                    transmitString("offset ");
                    transmitHex(INT, start_offset);
                    transmitString("\r\n");
                }
            }
            
            if (start_offset >= 0)
            {
                res = (i-start_offset) % 256;

                if (res != buffer[i])
                {
                    transmitString("error:");
                    transmitHex(INT, i);
                    transmitByte(' ');
                    transmitHex(CHAR, buffer[i]);
                    transmitByte(' ');
                    transmitHex(INT, res);
                    transmitString("\r\n");
                }
            }
            
        }
        */
        
        /*
        for (i = 0; i < 100; i++)
        {
            transmitHex(CHAR, buffer[i]);
            transmitString(" ");
            transmitHex(INT, i);
            
            if (i > 0)
            {
                if (buffer[i] != buffer[i-1]+1)
                {
                    transmitString(" **");
                }
            }
            
            transmitString("\r\n");
            _delay_loop_2(65535);
        }
        */
    }
}