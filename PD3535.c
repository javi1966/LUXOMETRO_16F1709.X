#include <xc.h>
#include "PD3535.h"
/*Libreria para el PD3535,display led Alf. en forma serie
  cod 2XCD4094
Funciones:
            void snd4094(BYTE* dato);
            void initPD3535(byte modo);
            void  disChar(char ch); //para printf  */



#define _XTAL_FREQ  16000000

//**********************************************************

void snd4094(WORD_VAL dato) {
    BYTE i = 16;


    //output_low(clk);     //pone clk a 0
    clk = 0;

    for (i = 16; i > 0; i--) {
        if ((dato.Val & 0x8000) == 0) //envia MSB a los 4094

            //output_low(data);
            data = 0;
        else
            //output_high(data);
            data = 1;

        //shift_left(dato,2,0); //lo desplaza 16 bits 2xIC4094
        dato.Val <<= 1; //(BYTE)(dato<<16);
        //output_high(clk);
        clk = 1;
        __delay_us(4); //Aplica señal de reloj para desp.
        //output_low(clk);
        clk = 0;

    } //16 bits


    // output_low(wr);
    wr = 0;
    __delay_us(8);
    // output_high(wr);
    wr = 1;

}
//*******************************************************************

void initPD3535(BYTE modo) {
    WORD_VAL dato;

    tris_data = tris_clk = tris_wr = 0;

    dato.v[0] = modo;
    dato.v[1] = 0;
    snd4094(dato);
}
//*******************************************************************
//Visualiza caracter en display PD3535,con shift serie-paralelo 4094

void disChar(char ch) {
    static int pos = 7; //empieza posicion MSB
    WORD_VAL buff;
    buff.v[0] = ch;
    buff.v[1] = pos;
    snd4094(buff);
    if (--pos < 4)
        pos = 7; //acaba posicion LSB
}


//********************************************************************
//Visualiza caracter en pos ,digito MSB 3 ,digito LSB 0

void disCharPos(char ch, int pos) {
    WORD_VAL Buff;

    if (pos > 3)
        pos = 3;

    Buff.v[0] = ch;
    Buff.v[1] = pos + 4;
    snd4094(Buff);

}
//**************************************************

void printVal(char *str) {

    int i;
    int length();

    i = 0;
    while (*str) {

        disChar(*str++);

        i++;

    }

   

}
