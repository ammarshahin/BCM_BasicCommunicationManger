/*
 * BCM_Proj.c
 *
 * Created: 11/17/2019 4:54:45 PM
 * Author : Ammar Shahin
 */ 

#include "Typedefs.h"
#include "BCM.h"
#include "Bcm_PBCfg.h"
#include "Interrupts.h"
#include "Led.h"


void led1(void)
{
	Led_Tog(LED0);
}

#define ARRAY_SIZE 10
uint8 arr[ARRAY_SIZE] = {2,2,2,2,2,2,2,2,2,2};

int main(void)
{
	Led_Init(LED0);
	BCM_Init(led1);
	Interrupts_On();
	BCM_Tx_SetupBuffer(arr,ARRAY_SIZE);
    while (1) 
    {
		BCM_TxDispatcher();
    }
	return 0;
}
