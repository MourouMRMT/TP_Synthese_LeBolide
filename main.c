/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <pic18f45k50.h>


#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"       /* User funct/params, such as InitApp */

#define _XTAL_FREQ 4000000 //Fixe la frequence de l'oscilateur sur 4MHz

#pragma config FOSC=INTOSCIO
#pragma config WDTEN=OFF
#pragma config MCLRE=OFF
#pragma config LVP=OFF
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void avance(int imp)
{
    if(imp==1)
    {
        LATDbits.LATD0=1;
        LATDbits.LATD1=0;
        LATDbits.LATD2=1;
        LATDbits.LATD3=0;

    }else{LATD=0;}
}
void recule(int imp)
{
    if(imp==1)
    {
        LATDbits.LATD0=0;
        LATDbits.LATD1=1;
        LATDbits.LATD2=0;
        LATDbits.LATD3=1;

    }else{LATD=0;}
}

void arret()
{
    LATD=0;
}

void detection_obstacle()
{
    //Generarion MLI
    LATCbits.LATC0=1;
    __delay_us(10);
    LATCbits.LATC0=0;
    
    while(PORTCbits.RC1==0);               
    T1CONbits.TMR1ON=1;               
    while(PORTCbits.RC1==1);               
    T1CONbits.TMR1ON=0; 
    
    int Timer=TMR1;                     
    int Duree=Timer/2;
    int Distance;
    if(Duree<60)               
    {
        Distance=Duree/58 ;
        arret();
        recule(1);
        __delay_ms(100);
    }else{avance(1);LATDbits.LATD0=1;}
    Duree=0;                     
    TMR1=0;

}

void affichage()
{
}


void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    /* TODO <INSERT USER APPLICATION CODE HERE> */
    TRISD=0;
    ANSELD=0;
    TRISCbits.TRISC1=1;
    TRISCbits.TRISC0=0;
    ANSELC=0;
    int i;
    while(1)
    {
        detection_obstacle();
        //LATD0=!LATD0;
        //avance(1);
        //__delay_ms(1000);
        //recule(1);
        //__delay_ms(5000);
        //for(i=0;i<1024;i++);
    }

}

