/* 
 * File:   i2c_hal.h
 * Author: Javi
 *
 * Created on 10 de noviembre de 2014, 17:55
 */

#ifndef I2C_HAL_H
#define	I2C_HAL_H

#define SDA_TRIS		TRISBbits.TRISB4
#define SCK_TRIS		TRISBbits.TRISB6

void initI2C();
void I2CStart();
void I2CStop();
void I2CRestart();
void I2CAck();
void I2CNak();
void I2CWait();
void I2CSend(unsigned char dat);
unsigned char I2CRead(void);

#endif	/* I2C_HAL_H */

