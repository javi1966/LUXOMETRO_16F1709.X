
#include <pic16f1709.h>
#include <xc.h>
#include <stdlib.h>
#include "GenericTypeDefs.h"
#include "MCP3422.h"
#include "PD3535.h"


#pragma config FOSC=INTOSC , WDTE=OFF, PWRTE=OFF , MCLRE=ON ,\
        CP = OFF, BOREN = OFF, CLKOUTEN = OFF,\
        WRT = OFF,  STVREN = ON, LVP = OFF


#define _XTAL_FREQ  16000000

#define LED                     LATBbits.LATB7
#define TRIS_LED                TRISBbits.TRISB7
#define	ADCON0_INIT 		0b10000001
#define	AD_SET_CHAN(x)		{ ADCON0 = ADCON0_INIT | (x << 2); }
#define	AD_CONVERT()		{ ADCON0bits.GO_nDONE = 1; while(ADCON0bits.GO_nDONE); }
#define K_VDD                    4.9

unsigned long ad_res;
volatile unsigned int res;
volatile BOOL bMedida = FALSE;
LONG resul;
volatile LONG wlux;
unsigned long lux;
float ip;
unsigned long vr;
float fvr;
int i;
char Resul[5];

void main(void) {


    unsigned char temp;
    int i;


    OSCCONbits.SCS = 0b10;
    OSCCONbits.IRCF = 0b1111; //16Mhz


    TRISA = 0xFFFF;
    ANSELB = 0x00;

    TRIS_LED = 0;
    LED = 1;

    //temporizacion indicacion alarma
    OPTION_REGbits.PSA = 0; //prescaler a TMR0
    OPTION_REGbits.PS = 0b111; //FOSC/4/256
    OPTION_REGbits.TMR0CS = 0; //FOSC/4   instruccion cicle
    INTCONbits.TMR0IE = 1; //interrupt 255 * 255 * 0.25us aprox 33ms
    INTCONbits.TMR0IF = 0;
    TMR0 = 0;


    //OPAMP2 UP
    OPA2CONbits.OPA2EN = 1;

    //conv A/D
    ANSELA = 0b00000100; //RA2 analog
    TRISA = 0b00000100; //RA2 input
    ADCON1bits.ADFM = 1; //Right justified
    ADCON0bits.CHS = 0b00010; //AN2
    ADCON1bits.ADPREF = 0; //VDD
    ADCON1bits.ADCS = 0b110; //FOSC/32 4us@8Mhz
    ADCON0bits.ADON = 1; //AD on*/

    //initMCP3422();


    for (i = 0; i < 5; i++) {
        initPD3535(LAMPTEST);
        __delay_ms(300);
        initPD3535(BLANCO);
        __delay_ms(300);
    }
    initPD3535(BRI100);

    printVal("LUXM");



    //GIE y PEIE
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    __delay_ms(500);

    while (TRUE) {


        if (bMedida == TRUE) {

            /*
             //sensibilidad 10k * 10e-8 A/lux= 0.1mv/lux
             //sensibilidad 100k * 10e-8 A/lux= 1mv/lux
             //sensibilidad 1M * 10e-8 A/lux = 10mv/lux
             resul = readMCP3422() * (LONG) 62.5; // result*62.5uV
             wlux = (resul + 612000) / 2000; //1000 para uV * 2 para ajuste
             */
            ad_res = 0;

            for (i = 0; i < 32; i++) {

                AD_CONVERT();

                ad_res = ad_res + (ADRESL + 256 * ADRESH);
            }

            ad_res /= 32;
            vr = (ad_res * 1000 / 1024)*4900; //para eliminar float se x1000
           
            ////sensibilidad 10k * 10e-8 A/lux= 0.1mv/lux
            //sensibilidad 100k * 10e-8 A/lux= 1mv/lux
            //sensibilidad 1M * 10e-8 A/lux = 10mv/lux
            //vr/1000=xxxx mV ,para 10mV xxxx/10 -> 1e4
            lux = (vr / 1e4) * 2;
            ltoa(Resul, lux, 10);
            printVal(Resul);

            bMedida = FALSE;
        }

    }//fin while


}


//***********************************************************************

void interrupt isr(void) {


    static int cnt = 0;


    if (TMR0IF) {

        if (++cnt > 126) { //aprox 1 seg
            LED = ~LED;

            cnt = 0;
            bMedida = TRUE;
        }

        TMR0IF = 0;
    }

}

