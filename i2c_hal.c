#include <xc.h>
#include "i2c_hal.h"

#define _XTAL_FREQ  16000000

void initI2C() {

    // SDA_TRIS = 1;
    //SCK_TRIS = 1;
    RB4PPS = 0x11; //SDA
    RB6PPS = 0x10; //SCL
    WPUBbits.WPUB4 = 1;
    WPUBbits.WPUB6 = 1;
    SSP1CONbits.SSPM = 0x8; // I2C master mode
    SSP1CONbits.SSPEN = 1; //enable modulo i2c

    // For this example we want the the standard 100Khz I2C clock rate and our
    // internal Fosc is 16Mhz so we get:  100000 = 16000000/[(SSPADD+1)*4]
    // or solving for SSPADD = [(16000000/100000)-4]/4
    // and we get SSPADD = 39

    SSP1ADD = 39; // set Baud rate clock divider
    SSP1STAT = 0x80;
    PIE1bits.SSP1IE = 1;
    __delay_ms(10);
}

/**************************************************************/
void I2CStart() {
    PIR1bits.SSP1IF = 0;
    SSP1CON2bits.SEN = 1; /* Start condition enabled */
    while (!PIR1bits.SSP1IF); /* automatically cleared by hardware */
    /* wait for start condition to finish */
}
//*************************************************************

void I2CStop() {
    PIR1bits.SSP1IF = 0;
    //SSPCON2bits.
    PEN = 1; /* Stop condition enabled */
    while (!PIR1bits.SSP1IF); /* Wait for stop condition to finish */
    /* PEN automatically cleared by hardware */
}
//**************************************************************

void I2CRestart() {
    PIR1bits.SSP1IF = 0;
    SSPCON2bits.RSEN = 1; /* Repeated start enabled */
    while (!PIR1bits.SSP1IF); /* wait for condition to finish */
}
//*************************************************************

void I2CAck() {
    PIR1bits.SSP1IF = 0;
    SSPCON2bits.ACKDT = 0; /* Acknowledge data bit, 0 = ACK */
    SSPCON2bits.ACKEN = 1; /* Ack data enabled */
    while (!PIR1bits.SSP1IF); /* wait for ack data to send on bus */
}
//****************************************************************

void I2CNak() {
    PIR1bits.SSP1IF = 0;
    SSPCON2bits.ACKDT = 1; /* Acknowledge data bit, 1 = NAK */
    SSPCON2bits.ACKEN = 1; /* Ack data enabled */
    while (!PIR1bits.SSP1IF); /* wait for ack data to send on bus */
}
//****************************************************************

void I2CWait() {
    while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04));
    /* wait for any pending transfer */
}
//***************************************************************

void I2CSend(unsigned char dat) {
    PIR1bits.SSP1IF = 0;
    SSPBUF = dat; /* Move data to SSPBUF */
    while (!PIR1bits.SSP1IF); /* wait till complete data is sent from buffer */
    /* wait for any pending transfer */
}
//***************************************************************

unsigned char I2CRead(void) {

    /* Reception works if transfer is initiated in read mode */
    PIR1bits.SSP1IF = 0;
    RCEN = 1; /* Enable data reception */
    while (!PIR1bits.SSP1IF); /* wait for buffer full */
    return ( SSPBUF); /* Read serial buffer and store in temp register */
    /* Return the read data from bus */
}