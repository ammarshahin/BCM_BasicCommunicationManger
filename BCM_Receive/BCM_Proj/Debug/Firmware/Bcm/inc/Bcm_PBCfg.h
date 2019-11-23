/*
 * Bcm_PBCfg.h
 *
 * Created: 11/17/2019 7:30:10 PM
 *  Author: Ammar Shahin
 */ 


#ifndef BCM_PBCFG_H_
#define BCM_PBCFG_H_


/************************************************************************/
/*                               INCLUDES                               */
/************************************************************************/
#include "Typedefs.h"
//#include "BCM.h"

/************************************************************************/
/*                               ENUMS                                  */
/************************************************************************/
typedef void (*ptrBCMFunCallBk_t)(void);


typedef enum{
	UART  ,
	SPI   ,
	I2C   ,
}BCM_CommunicationPeripheral_t;


typedef enum{
	SEND_MODE         ,
	RECEIVE_MODE      ,
	SEND_RECEIVE_MODE ,
}BCM_ComType_t;

/************************************************************************/
/*                       STRUCTS AND UNIONS                              */
/************************************************************************/
/* This is a Cfg struct for a BCM_Config */



typedef struct 
{
	uint8			  Bcm_ID;
	BCM_ComType_t	  BCM_Rx_or_Tx;
	ptrBCMFunCallBk_t BCMPtr_Rx_FuncCallBk;
	ptrBCMFunCallBk_t BCMPtr_Tx_FuncCallBk;
}BCM_Config_t;


extern BCM_Config_t BCM_Cfg_t;
extern BCM_CommunicationPeripheral_t Communication_type;

#endif /* BCM_PBCFG_H_ */