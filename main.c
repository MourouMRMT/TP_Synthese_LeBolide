/******************************************************************************/
                /*Program by MARIMOUTOU Mourougen*/
                            /*Date:04/2022*/
                            /*Revision:0.5*/
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

#include "C:\Users\mouro\Desktop\CoursGE\TP_de_Synthese\Code\I2C_LCD.h"
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
 -pin Echo: RC4
 -Pin Trigger:RC0
 *Boite:
 -pin Echo: RC7
 -Pin Trigger:RC6

 * Capteur sonore:
 -Pin Trigger:RE0
 *
 * Sharp Sensor:
 * Analog read :RAO
 */
/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/
int imp=0,obs=0,distance=0;
int cpt=0;
/* i.e. uint8_t <variable_name>; */
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void ADC_init()
{
    ADCON0=0;
    ADCON1=0;
    ADCON2=0;
}

int ADC_Read(int channel)
{
    int digit;
    ADCON0=(ADCON0 & 0b11000011)|((channel<<2) & 0b00111100);
    ADCON0|=((1<<ADON)|(1<<GO));

    while(ADCON0bits.GO_nDONE==1);
    digit=(ADRESH*256)|(ADRESL);

    return digit;

}

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

void avance_PWM(int var)
{
    LATDbits.LATD4=var;
    LATDbits.LATD5=var;
}
void recule(int imp)
{
    if(imp==1)
    {
        LATDbits.LATD0=1;
        LATDbits.LATD1=0;
        LATDbits.LATD2=1;
        LATDbits.LATD3=0;

    }else{LATD=0;}
}
void tourner_droite()
{
    if(imp==1)
    {
    LATDbits.LATD0=1;
    LATDbits.LATD1=0;
    LATDbits.LATD2=0;
    LATDbits.LATD3=1;
    }

}

void tourner_gauche()
{
    if(imp==1)
    {

    LATDbits.LATD0=0;
    LATDbits.LATD1=1;
    LATDbits.LATD2=1;
    LATDbits.LATD3=0;
    }
}


void arret()
{
    LATD=0;
}

void Sharp_detector()
{
  float range=0; // value from sensor * (5/1024)
  int digit = ADC_Read(0); // worked out from datasheet graph
  range = digit*((float)5.0/(float)1024.0);
  if (range> 30){
      LATEbits.LATE1=1;

  }else
  {
      LATEbits.LATE1=0;
      imp=0;// stop the bolide
  }


}
void detection_obstacle()
{
    //Generarion MLI
    LATCbits.LATC0=1;
    __delay_us(10);
    LATCbits.LATC0=0;

    //Temporisation afin de determiner la duree
    while(PORTCbits.RC4==0);
    T1CONbits.TMR1ON=1;     //On debute le comptage
    while(PORTCbits.RC4==1);
    T1CONbits.TMR1ON=0;     //On arrete le comptage


    int Timer=TMR1;          //Temps en µs
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


    int Timer=TMR1;          //Temps en µs
    int Duree=Timer;
    int Distance;
    if(Duree<180)
    {
        imp=0;
        LATEbits.LATE1=0;
        tourner_gauche();
        //Distance=Duree2/58 ;

    }else
    {
        avance(imp);
        LATEbits.LATE1=1;

    }
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
    ANSELC=0;
    ANSELB=0;

    ANSELE=0;
    TRISCbits.TRISC4=1;
    TRISEbits.TRISE0=1;

    TRISCbits.TRISC0=0;
    TRISBbits.TRISB7=0;
    TRISCbits.TRISC7=0;
    TRISCbits.TRISC6=1;

    ANSELDbits.ANSD0=0;
    ANSELDbits.ANSD1=0;
    ANSELDbits.ANSD2=0;
    ANSELDbits.ANSD3=0;

    ANSELDbits.ANSD4=1;
    ANSELDbits.ANSD5=1;
    //Sharp Sensor
    TRISAbits.RA0=1;
    ANSELAbits.ANSA0=1;
    TRISEbits.TRISE1=0;
    //Afficheur
    //PWM
    ANSELCbits.ANSC2=1;
    TRISCbits.TRISC2=0;
    while(1)
    {
        clap();
        avance(imp);
        avance_PWM(255);
        Sharp_detector();
        //tour_boite();
        //detection_obstacle();
        //tourner_gauche();
        //affichage();
        //avance_PWM(120);

    }

}
