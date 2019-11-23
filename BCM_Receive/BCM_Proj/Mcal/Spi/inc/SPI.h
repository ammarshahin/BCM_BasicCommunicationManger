/*
 * SPI.h
 *
 * Created: 11/12/2019 3:33:13 PM
 *  Author: Ammar Shahin 
 */ 


#ifndef SPI_H_
#define SPI_H_

/************************************************************************/
/*                            Files Includes                            */
/************************************************************************/
#include "Typedefs.h"
#include "SPI_Lcfg.h"

/************************************************************************/
/*                               Macros                                 */
/************************************************************************/
#define  DISABLED       0
#define  ENABLED        1

/* Mode values */
#define SLAVE 0
#define MASTER 1

/* clk_Polarity options */
#define RISING  0
#define FALLING 1

/* Interrupt_Enable options */
#define POLLING     0
#define INTERRUPT  1

/* Clock Phase options */
#define SAMPLE  0
#define SETUP   1

/* Data Order options */
#define MSB_ORDER  0
#define LSB_ORDER   1

/* SPI Prescaler options */
#define SPI_PRESCALER_4    0
#define SPI_PRESCALER_16   1
#define SPI_PRESCALER_64   2
#define SPI_PRESCALER_128  3


/* SPI GPIO Port and Pins */
#define SPI_PORT MYPORTB
#define SPI_SS   BIT4
#define SPI_MOSI BIT5
#define SPI_MISO BIT6
#define SPI_SCK  BIT7


extern volatile uint32 Speed;
extern volatile int Count_gBcm_Rx_Flag;

/************************************************************************/
/*                   SPI Functions Prototypes                          */
/************************************************************************/
/**
 * Function : SPI_Init
 * Description: this function is to initialize the SPI 
 * @param cfg_s the Configuration Structure which include:
 * 								1- the mode of the SPI {MASTER or SLAVE}
 *								2- the clk_Polarity {RISING or FALLING}
 * 								3- the Interrupt_Enable  {POLLING or INTERRUPTS}
 * 								4- the DataOrder  {MSB_ORDER or LSB_ORDER}
 * 								5- the ClockPhase  {SAMPLE or SETUP}
 * 								6- the SPI_Prescaler  {4,16,64,128}
 * @return the Status of the initialization [OK Or NOT_OK] 
 */
uint8 SPI_Init(SPI_CFG_S* cfg_s);

/**
 * Function : SPI_Send
 * Description: this function is to send a data(8 bits) by the SPI 
 * @param data the symbol to be sent
 * @return the Status of the function [OK Or NOT_OK] 
 */
uint8 SPI_Send(uint8 data);

/**
 * Function : SPI_Receive
 * Description: this function is to Receive a data by the SPI 
 * @param data the symbol to be sent
 * @return the Received data
 */
uint8 SPI_Receive();

/**
 * Function : SPI_Read
 * Description: this function is to Receive a data by the SPI 
 * @param data the symbol to be sent
 * @return the Received data
 */
uint8 SPI_Read();

#endif /* SPI_H_ */
