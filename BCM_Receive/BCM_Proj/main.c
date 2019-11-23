/*
 * main.c
 *
 * Created: 11/19/2019 4:37:50 PM
 *  Author: AVE-LAP-44
 */ 

#include "Typedefs.h"
#include "BCM.h"
#include "Bcm_PBCfg.h"
#include "Interrupts.h"
#include "Led.h"


#define ARRAY_SIZE 100

void led1(void)
{
	Led_Tog(LED0);
}


uint8 arr[ARRAY_SIZE] = {0};

int main(void)
{
	Led_Init(LED0);
	BCM_Init(led1);
	Interrupts_On();
	BCM_Rx_SetupBuffer(arr,ARRAY_SIZE);
	while (1)
	{
		BCM_RxDispatcher();
	}
	return 0;
}

