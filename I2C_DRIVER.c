/*
 * File:   i2c_gateopen.c
 * Author: gsvis
 *
 * Created on 13 January, 2021, 2:24 PM
 */
#include "mcc_generated_files/mcc.h"
#include <xc.h>

void I2C2_RepeatStart(void);
void I2C2_Start(void);
void I2C2_Write(uint8_t data);
void I2C2_IDLE(void);
void I2C2_Stop(void);
uint8_t I2C2_Read(void);

#define SCL BIT6
#define SDA BIT4   //
uint8_t  EEPROM_ADD =0xA2;// 0xA0;
uint8_t WRITE=0x00;
uint8_t READ =0x01;





//I2C functions /*

void init_I2C(void)
{
    
    //SSP1ADD = 0X40;
    SSP1STAT = 0x00;//0x08
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1ADD  = 0x13;//0x03
    SSP1CON1bits.SSPEN = 1;
} 
void I2C_Store(uint8_t add, uint8_t *data)
{
    uint8_t index=0;
//    SSP1CON1bits.SSPEN = 1;
    I2C2_Start();
    I2C2_Write(EEPROM_ADD | WRITE);
    __delay_ms(100);
//    I2C2_Write(0x00);
    I2C2_Write(add);
    __delay_us(20);
    for(index=0; index<8; index ++)
    I2C2_Write(data[index]);
    I2C2_IDLE(); 
    I2C2_Stop();
   __delay_ms(100);
//   SSP1CON1bits.SSPEN = 0;
} 
  
 uint8_t I2C_Retrive(uint8_t add)
{
    uint8_t I2Cread;
//    SSP1CON1bits.SSPEN = 1;
    I2C2_Start();
    I2C2_Write(EEPROM_ADD |WRITE);
    __delay_ms(100);
//    I2C2_Write(0x00);
    I2C2_Write(add);
//    __delay_ms(100);
    I2C2_RepeatStart();
    I2C2_Write(EEPROM_ADD |READ);
    __delay_us(100);
//    I2C2_Write(add);
    I2Cread =I2C2_Read();
    I2C2_Stop();
  __delay_us(25);
//  SSP1CON1bits.SSPEN = 0;
  return I2Cread;
}
void I2C2_Write(uint8_t data)
{
  I2C2_IDLE();
  SSP1BUF = data;
 
  if (SSP1CON1bits.WCOL)     // Check for write collision
    return;
  while(SSP1STATbits.BF);    // Check for Transmission  Complete
  __delay_us(100);
}
uint8_t I2C2_Read(void)
{
  I2C2_IDLE();
  SSP1CON2bits.RCEN=1;      //Enable reception in Master device 
  while(!SSP1STATbits.BF);  //Wait till buffer is full
 __delay_us(110);
  SSP1CON2bits.RCEN=0;       //Disable reception in Master device 
  return SSP1BUF;           //return received data
}

void I2C2_IDLE(void)
{
  while ((SSP1STAT & 0x04) || (SSP1CON2 & 0x1F));   //Transmit is in progress
}
void I2C2_Stop(void)
{
  I2C2_IDLE();
  SSP1CON2bits.PEN = 1;
  while(SSP1CON2bits.PEN);
}

void I2C2_Start(void)
{
    I2C2_IDLE(); 
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void I2C2_RepeatStart(void)
{
  I2C2_IDLE();
  SSP1CON2bits.RSEN = 1; 
  while(SSP1CON2bits.RSEN);
}
uint8_t I2C2_Slave_ACK(void)
{
  // Return: 1 = Acknowledge was not received from slave
  // 0 = Acknowledge was received from slave
  return(SSP1CON2bits.ACKSTAT);
}
  