/*
 * SPI_Lcfg.c
 * Description : This File is used to control the SPI Peripheral in the MCAL layer
 * Created: 11/12/2019 3:40:16 PM
 *  Author: Ammar Shahin
 */ 

/************************************************************************/
/*                            Files Includes                            */
/************************************************************************/
#include "SPI_Lcfg.h"
#include "SPI.h"

/************************************************************************/
/*                     Linking Structures Definitions                    */
/************************************************************************/

/* SPI Structure */
SPI_CFG_S SPI_Cfg_s = {
    SLAVE,           /* Mode */
    FALLING,          /* clk_Polarity */
    INTERRUPT,       /* Interrupt_Enable */
	LSB_ORDER,        /* DataOrder */
    SAMPLE,           /* ClockPhase */
	SPI_PRESCALER_4,   /* SPI_Prescaler */
};