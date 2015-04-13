/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.10.3
        Device            :  PIC16F1709
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X 2.26
*/

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*/

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/ccp1.h"

// Modulate the 38kHz carrier (produced by PWM2) by injecting logic in CLC1-Gate2
#define IR_ON()   LC1G2POL = 1;   // invert output of CLC1 - Gate2 (inject 1)
#define IR_OFF()  LC1G2POL = 0;   // normal output of CLC1 - Gate2 (inject 0)

/**
 * define an accurate delay based on TMR1 + CCP1 in compare mode
 */
void Delay( uint16_t duration)
{ // duration is in us: assumes INTOSC == 4MHz -> Fcy = 1MHz
    CCP1_SetCompareCount( duration-50); // compensate for lost cycles
    TMR1 = 0;
    PIR1bits.CCP1IF = 0;                // clear CCP flag
    while ( !CCP1_IsCompareComplete()); // wait
} // Delay


void Leader( void)
{
    IR_ON();
    Delay( 9000);   // 9 ms pulse
    IR_OFF();
    Delay( 4500);   // 4.5 ms pause
} // Leader


void Blast( uint8_t byte)
{
    uint8_t i;
    for( i=0; i<8; i++)
    {   // serialize data LSB first
        IR_ON();
        Delay( 560);
        IR_OFF();
        if ( byte & 1)
            Delay(1680);    // total duration 1.12 ms
        else
            Delay( 560);    // total duration 2.24 ms
        byte >>= 1;
    }
} // Blast


void Stop( void)
{
    IR_ON();
    Delay( 560);
    IR_OFF();
} // Stop


// Table of valid button codes 
// remember to shift left by 1 and add parity bit
// parity of entire transmission must be ODD
// 01 - Menu
// 02 - Center
// 03 - Right
// 04 - Left
// 05 - Up
// 06 - Down
// 2f - Play/Pause


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    while (1)
    {
        // check if button pressed
        if ( !SW1_GetValue())
        {
            // send one blast
            Leader();
            Blast( 0xEE);   // unique apple remote signature
            Blast( 0x87);   // signature
            Blast( (0x2f<<1)+1);   // example Up button: ( 0x5 << 1) + 0 parity
            Blast( 0x3E);   // pairing code 
            Stop();

            while( !SW1_GetValue()); // wait for release
            Delay( 50000);          // debounce
        }
    }
} // main
/**
 End of File
*/