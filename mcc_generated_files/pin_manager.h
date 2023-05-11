/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1936
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set L1 aliases
#define L1_TRIS                 TRISAbits.TRISA0
#define L1_LAT                  LATAbits.LATA0
#define L1_PORT                 PORTAbits.RA0
#define L1_ANS                  ANSELAbits.ANSA0
#define L1_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define L1_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define L1_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define L1_GetValue()           PORTAbits.RA0
#define L1_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define L1_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define L1_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define L1_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set L2 aliases
#define L2_TRIS                 TRISAbits.TRISA1
#define L2_LAT                  LATAbits.LATA1
#define L2_PORT                 PORTAbits.RA1
#define L2_ANS                  ANSELAbits.ANSA1
#define L2_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define L2_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define L2_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define L2_GetValue()           PORTAbits.RA1
#define L2_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define L2_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define L2_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define L2_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set L4 aliases
#define L4_TRIS                 TRISAbits.TRISA6
#define L4_LAT                  LATAbits.LATA6
#define L4_PORT                 PORTAbits.RA6
#define L4_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define L4_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define L4_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define L4_GetValue()           PORTAbits.RA6
#define L4_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define L4_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set L3 aliases
#define L3_TRIS                 TRISAbits.TRISA7
#define L3_LAT                  LATAbits.LATA7
#define L3_PORT                 PORTAbits.RA7
#define L3_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define L3_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define L3_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define L3_GetValue()           PORTAbits.RA7
#define L3_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define L3_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set DEC_A2 aliases
#define DEC_A2_TRIS                 TRISBbits.TRISB1
#define DEC_A2_LAT                  LATBbits.LATB1
#define DEC_A2_PORT                 PORTBbits.RB1
#define DEC_A2_WPU                  WPUBbits.WPUB1
#define DEC_A2_ANS                  ANSELBbits.ANSB1
#define DEC_A2_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define DEC_A2_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define DEC_A2_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define DEC_A2_GetValue()           PORTBbits.RB1
#define DEC_A2_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define DEC_A2_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define DEC_A2_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define DEC_A2_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define DEC_A2_SetAnalogMode()      do { ANSELBbits.ANSB1 = 1; } while(0)
#define DEC_A2_SetDigitalMode()     do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set DEC_A1 aliases
#define DEC_A1_TRIS                 TRISBbits.TRISB2
#define DEC_A1_LAT                  LATBbits.LATB2
#define DEC_A1_PORT                 PORTBbits.RB2
#define DEC_A1_WPU                  WPUBbits.WPUB2
#define DEC_A1_ANS                  ANSELBbits.ANSB2
#define DEC_A1_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define DEC_A1_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define DEC_A1_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define DEC_A1_GetValue()           PORTBbits.RB2
#define DEC_A1_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define DEC_A1_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define DEC_A1_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define DEC_A1_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define DEC_A1_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define DEC_A1_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set DEC_A3 aliases
#define DEC_A3_TRIS                 TRISBbits.TRISB3
#define DEC_A3_LAT                  LATBbits.LATB3
#define DEC_A3_PORT                 PORTBbits.RB3
#define DEC_A3_WPU                  WPUBbits.WPUB3
#define DEC_A3_ANS                  ANSELBbits.ANSB3
#define DEC_A3_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define DEC_A3_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define DEC_A3_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define DEC_A3_GetValue()           PORTBbits.RB3
#define DEC_A3_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define DEC_A3_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define DEC_A3_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define DEC_A3_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define DEC_A3_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define DEC_A3_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set DEC_A0 aliases
#define DEC_A0_TRIS                 TRISBbits.TRISB4
#define DEC_A0_LAT                  LATBbits.LATB4
#define DEC_A0_PORT                 PORTBbits.RB4
#define DEC_A0_WPU                  WPUBbits.WPUB4
#define DEC_A0_ANS                  ANSELBbits.ANSB4
#define DEC_A0_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define DEC_A0_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define DEC_A0_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define DEC_A0_GetValue()           PORTBbits.RB4
#define DEC_A0_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define DEC_A0_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define DEC_A0_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define DEC_A0_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define DEC_A0_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define DEC_A0_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set BTN_3 aliases
#define BTN_3_TRIS                 TRISBbits.TRISB5
#define BTN_3_LAT                  LATBbits.LATB5
#define BTN_3_PORT                 PORTBbits.RB5
#define BTN_3_WPU                  WPUBbits.WPUB5
#define BTN_3_ANS                  ANSELBbits.ANSB5
#define BTN_3_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define BTN_3_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define BTN_3_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define BTN_3_GetValue()           PORTBbits.RB5
#define BTN_3_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define BTN_3_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define BTN_3_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define BTN_3_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define BTN_3_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define BTN_3_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set BTN_2 aliases
#define BTN_2_TRIS                 TRISBbits.TRISB6
#define BTN_2_LAT                  LATBbits.LATB6
#define BTN_2_PORT                 PORTBbits.RB6
#define BTN_2_WPU                  WPUBbits.WPUB6
#define BTN_2_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define BTN_2_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define BTN_2_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define BTN_2_GetValue()           PORTBbits.RB6
#define BTN_2_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define BTN_2_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define BTN_2_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define BTN_2_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)

// get/set BTN_1 aliases
#define BTN_1_TRIS                 TRISBbits.TRISB7
#define BTN_1_LAT                  LATBbits.LATB7
#define BTN_1_PORT                 PORTBbits.RB7
#define BTN_1_WPU                  WPUBbits.WPUB7
#define BTN_1_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define BTN_1_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define BTN_1_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define BTN_1_GetValue()           PORTBbits.RB7
#define BTN_1_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define BTN_1_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define BTN_1_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define BTN_1_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()              PORTCbits.RC2
#define RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)

// get/set SDA aliases
#define SDA_TRIS                 TRISCbits.TRISC4
#define SDA_LAT                  LATCbits.LATC4
#define SDA_PORT                 PORTCbits.RC4
#define SDA_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SDA_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SDA_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SDA_GetValue()           PORTCbits.RC4
#define SDA_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)

// get/set SCL aliases
#define SCL_TRIS                 TRISCbits.TRISC5
#define SCL_LAT                  LATCbits.LATC5
#define SCL_PORT                 PORTCbits.RC5
#define SCL_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SCL_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SCL_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SCL_GetValue()           PORTCbits.RC5
#define SCL_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/