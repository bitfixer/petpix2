/*
 * PETpix2.c
 *
 * Created: 2/2/2013 9:14:06 PM
 *  Author: Michael Hill
 */ 

//#include "huffman.h"
#include "lz.h"
//#include "rle.h"
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
    unsigned int numBytes;
    unsigned char compressedBuffer[1100];
    unsigned char buffer[1000];
    int i;
    while(1)
    {
        // set CA1 line low
        PORTA = 0x00;
        byte = 0x00;
        //transmitString("waiting for buffer bytes:");
        
        
        // skip over buffer bytes
        while (byte != 0xff)
        {
            byte = receiveByte();
        }
        
        while (byte != 0x00)
        {
            byte = receiveByte();
        }
        
        buffer[0] = receiveByte();
        buffer[1] = receiveByte();
        
        numBytes = buffer[0];
        numBytes = numBytes << 8;
        numBytes += buffer[1];
        
        //transmitHex(INT, numBytes);

        for (i = 0; i < numBytes; i++)
        {
            compressedBuffer[i] = receiveByte();
        }
        
        //transmitString("got bytes\r\n");
        transmitHex(INT, numBytes);
        //Huffman_Uncompress(compressedBuffer, buffer, numBytes, 1000);
        //RLE_Uncompress(compressedBuffer, buffer, numBytes);
        LZ_Uncompress(compressedBuffer, buffer, numBytes);
        /*
        int cc = 0;
        for (int i = 0; i < 25; i++)
        {
            for (int j = 0; j < 40; j++)
            {
                buffer[cc] = '0'+j;
                cc++;
            }
        }
        */
        
        //transmitString("uncompressed buffer\r\n");
        
        
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
        
        //transmitString("done sending image.");
    }
}