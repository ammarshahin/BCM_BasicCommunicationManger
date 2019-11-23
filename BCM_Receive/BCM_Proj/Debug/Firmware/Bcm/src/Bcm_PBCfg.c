/*
 * Bcm_PBCfg.c
 *
 * Created: 11/17/2019 7:30:34 PM
 *  Author: Ammar Shahin
 */ 

/************************************************************************/
/*				              Files Includes                            */
/************************************************************************/
#include "Bcm_PBCfg.h"
#include "BCM.h"

/************************************************************************/
/*				              Enums                                     */
/************************************************************************/

/************************************************************************/
/*				       GLOBAL Variable Definitions                      */
/************************************************************************/

/* Create an object of the BCM_Rx_Config_t Struct */
BCM_Config_t BCM_Cfg_t = {
	BCM_ID      ,
	SEND_MODE   ,
	NULL		,
	NULL		,
};
	
BCM_CommunicationPeripheral_t Communication_type = UART;