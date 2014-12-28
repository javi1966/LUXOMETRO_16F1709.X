/* 
 * File:   PD3535.h
 * Author: t133643
 *
 * Created on 21 de diciembre de 2014, 8:51
 */

#ifndef PD3535_H
#define	PD3535_H


#include <pic16f1709.h>
#include "GenericTypeDefs.h"



#define data       LATBbits.LATB4
#define tris_data  TRISBbits.TRISB4
#define clk        LATBbits.LATB5
#define tris_clk   TRISBbits.TRISB5
#define wr         LATBbits.LATB6
#define tris_wr    TRISBbits.TRISB6



//********Atributos Display PD3535

#define CLEARDISPLAY   0x80
#define LAMPTEST       0x40
#define BLINK          0x20
#define ATRIBUTOS      0x10
#define CURSORON       0x00
#define BLINKCARACTER  0x01
#define CURSORPARPA    0x02
#define CARACURSOR     0x03
#define BLANCO         0x00
#define BRI25          0x01
#define BRI50          0x02
#define BRI100         0x03



void snd4094(WORD_VAL dato);
void initPD3535(BYTE modo);
void disChar(char ch);
void disCharPos(char ch,int pos);
void printVal(char *str);
#endif	/* PD3535_H */

