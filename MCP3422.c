#include "MCP3422.h"

void initMCP3422() {
    unsigned char ACK_bit;


    initI2C();
    I2CStart();
    I2CSend(0b11010000);
    I2CSend(0b00001000); //canal 0,16 bits
    ACK_bit = SSP1CON2bits.ACKSTAT;
    I2CStop();

}

//**********************************************************

WORD readMCP3422() {

    UINT32_VAL resul;

    //Un Shot,16 bits canal 0
    
    I2CStart();
    I2CSend(0b11010000);
    I2CSend(0b10001000); //canal 0,16 bits,1 shot
    I2CStop();           // 62.5uv

    __delay_ms(70);   //Convierte
  


    I2CStart();
    I2CSend(0b11010001);  //read

    resul.v[1] = I2CRead();
    I2CAck();
    resul.v[0] = I2CRead();
    I2CAck();
    resul.v[2] = I2CRead();
   
    I2CNak();
    I2CStop();

    return resul.w[0];
}


