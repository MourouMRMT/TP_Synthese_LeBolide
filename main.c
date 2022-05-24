/******************************************************************************/
                /*Program by MARIMOUTOU Mourougen*/
                            /*Date:04/2022*/
                            /*Revision:0.4*/
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
//#include"C:\Users\mouro\Desktop\CoursGE\TP_de_Synthese\Code\SSD1306OLED.c"

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"       /* User funct/params, such as InitApp */

#define _XTAL_FREQ 4000000

#pragma config FOSC=INTOSCIO
#pragma config WDTEN=OFF
#pragma config MCLRE=OFF
#pragma config LVP=OFF



/*
Config:
 -D4,D5:moteur avant gauche,moteur arriere gauche
 -D6,D7:moteur avant droit,moteur arriere droit
 D0,D2,D4,D6:marche avant
 D1,D3,D5,D7:marche arriere

 * Capteur a ultrasons:
 * Mur:
 -pin Echo: RC1
 -Pin Trigger:RC0
 *Boite:
 -pin Echo: RC7
 -Pin Trigger:RC6

 * Capteur sonore:
 -Pin Trigger:RE0
 */
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
int imp=0,obs=0;
int cpt=0;
/* i.e. uint8_t <variable_name>; */
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void clap()
{
    if(PORTEbits.RE0==1)
    {
        imp=1;
    }
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
    if(imp==1)
    {
    LATDbits.LATD0=0;
    LATDbits.LATD1=1;
    LATDbits.LATD2=1;
    LATDbits.LATD3=0;
    }

}

void tourner_gauche()
{
    if(imp==1)
    {

    LATDbits.LATD0=1;
    LATDbits.LATD1=0;
    LATDbits.LATD2=0;
    LATDbits.LATD3=1;
    }
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
    T1CONbits.TMR1ON=1;     //On debute le comptage
    while(PORTCbits.RC1==1);
    T1CONbits.TMR1ON=0;     //On arrete le comptage


    int Timer=TMR1;          //Temps en �s
    int Duree=Timer;
    int Distance;
    if(Duree<180)
    {
        Distance=Duree/58 ;
        imp=0;
    }else
    {//tourner_gauche();
        avance(imp);

    }
    Duree=0;
    TMR1=0;

}

void tour_boite()
{
    //Generarion MLI
    LATCbits.LATC7=1;
     __delay_us(10);
    LATCbits.LATC7=0;

    //Temporisation afin de determiner la duree
    while(PORTCbits.RC6==0);
    T1CONbits.TMR1ON=1;     //On debute le comptage
    while(PORTCbits.RC6==1);
    T1CONbits.TMR1ON=0;     //On arrete le comptage


    int Timer=TMR1;          //Temps en �s
    int Duree=Timer;
    int Distance;
    if(Duree<180)
    {
        imp=0;
        LATEbits.LATE1=0;
        //Distance=Duree2/58 ;

    }else
    {tourner_gauche();
        //avance(imp);
        LATEbits.LATE1=1;

    }
    Duree=0;
    TMR1=0;
}


void affichage()
{
  /*SSD1306_Begin(SSD1306_SWITCHCAPVCC, 0X7A);
  SSD1306_ClearDisplay();
  SSD1306_SetTextWrap(false);
  SSD1306_GotoXY(25, 0);
  SSD1306_Print("Hello world");
  SSD1306_Display();*/
}


void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    TRISD=0;
    ANSELC=0;
    ANSELB=0;
    ANSELD=0;
    ANSELE=0;
    TRISCbits.TRISC1=1;
    TRISEbits.TRISE0=1;
    TRISEbits.TRISE1=0;
    TRISCbits.TRISC0=0;
    TRISBbits.TRISB7=0;
    TRISCbits.TRISC7=0;
    TRISCbits.TRISC6=1;

    while(1)
    {
        clap();
        //tour_boite();
        //detection_obstacle();
        //tourner_gauche();
        //affichage();

    }

}
