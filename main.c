/******************************************************************************/
                /*Program by MARIMOUTOU Mourougen*/
                            /*Date:04/2022*/
                            /*Revision:0.2*/
/******************************************************************************/


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

/*
Config:
 -D0,D1:moteur arriere gauche
 -D2,D3:moteur arriere droit
 -D4,D5:moteur avant gauche
 -D6,D7:moteur avant droit
 D0,D2,D4,D6:marche avant
 D1,D3,D5,D7:marche arriere

 * Capteur a ultrasons:
 -pin Echo: RC1
 -Pin Trigger:RC0

 * Capteur sonore:
 -Pin Trigger:RE0
 */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/*                          Sous Programme                                    */
/******************************************************************************/


void clap()
{
    if(PORTEbits.RE0==1)
    {
        LATBbits.LATB7=1;
    }else{LATBbits.LATB7=0;}

}


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

void tourner_droite()
{
    LATDbits.LATD0=0;
    LATDbits.LATD1=1;
    LATDbits.LATD2=1;
    LATDbits.LATD3=0;
    LATDbits.LATD4=0;
    LATDbits.LATD5=1;
    LATDbits.LATD6=1;
    LATDbits.LATD7=0;

}

void tourner_gauche()
{

    LATDbits.LATD0=1;
    LATDbits.LATD1=0;
    LATDbits.LATD2=0;
    LATDbits.LATD3=1;
    LATDbits.LATD4=1;
    LATDbits.LATD5=0;
    LATDbits.LATD6=0;
    LATDbits.LATD7=1;
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
    //Temporisation afin de determiner la duree
    while(PORTCbits.RC1==0);
    T1CONbits.TMR1ON=1; //On debute le comptage
    while(PORTCbits.RC1==1);
    T1CONbits.TMR1ON=0; //On arrete le comptage

    int Timer=TMR1; //Temps en µs
    int Duree=Timer;
    int Distance;
    if(Duree<120) //Valeur aleatoire qui marche bien ^^
    {
        Distance=Duree/58 ;
        arret();
    }else{avance(1);}
    Duree=0;
    TMR1=0;

}

void affichage()
{
}

/******************************************************************************/
/*                             Main Program                                   */
/******************************************************************************/
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
        //clap();
        //detection_obstacle();
        //LATD0=!LATD0;
        //avance(1);
        //__delay_ms(1000);
        //recule(1);
        //__delay_ms(5000);
        //for(i=0;i<1024;i++);
    }

}

