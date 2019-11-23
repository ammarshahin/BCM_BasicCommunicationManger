/*
 * BCM.c
 *
 * Created: 11/17/2019 7:27:02 PM
 *  Author: Ammar Shahin
 */ 


/************************************************************************/
/*				              Files Includes                            */
/************************************************************************/
#include "BCM.h"
#include "Bcm_PBCfg.h"
#include "UART.h"


/************************************************************************/
/*                               LOCAL MACROS                           */
/************************************************************************/
#define SHIFT_FACTOR 8
#define ID_BYTE      0
#define ZERO         0
#define ONE          1

/************************************************************************/
/*                         LOCAL Structures                            */
/************************************************************************/



typedef struct{
	uint8 *BcmPtrBuffer  ;
	uint16 BcmBufferSize ;
	uint8 BcmCheckSum    ;
}BCM_Buffer_t;


/************************************************************************/
/*							Local Enums                                 */
/************************************************************************/
typedef enum{
	ID_RECEIVE_STATE		 ,
	SIZE_RECEIVE_STATE		 ,
	DATA_RECEIVE_STATE		 ,
	CHECKSUM_RECEIVE_STATE   ,
}BCM_Rx_State_t;

typedef enum{
	ID_SEND_STATE		 ,
	SIZE_SEND_STATE		 ,
	DATA_SEND_STATE		 ,
	CHECKSUM_SEND_STATE   ,
}BCM_Tx_State_t;

typedef enum{
	RECEIVED	 ,
	NOT_RECEIVED ,
}BCM_Rx_Flag_t;

typedef enum{
	SENT_DONE	 ,
	NOT_SENT	 ,
}BCM_Tx_Flag_t;

typedef enum{
	FIRST_BYTE	  = 1,
	SECOND_BYTE   = 2,
}BCM_Rx_SizeFlag_t;

typedef enum{
	UNLOCKED ,
	LOCKED   ,
}BCM_Lock_t;

/************************************************************************/
/*                     LOCAL FUNCTIONS PROTOTYPES                       */
/************************************************************************/



/************************************************************************/
/*                       GLOBAL STATIC VARIABLES                        */
/************************************************************************/
static uint16 gBufferRxIndex = ZERO;
static uint16 gBufferTxIndex = ZERO;

static uint16 LengthSizeRx = ZERO;

static BCM_Lock_t gBcm_Rx_Lock;
static BCM_Lock_t gBcm_Tx_Lock;

static BCM_Buffer_t gBcmBuffer;

static BCM_Rx_State_t gBcm_Rx_state = ID_RECEIVE_STATE;
static BCM_Tx_State_t gBcm_Tx_state = ID_SEND_STATE;

static BCM_Rx_Flag_t gBcm_Rx_Flag = NOT_RECEIVED;
static BCM_Tx_Flag_t gBcm_Tx_Flag = NOT_SENT;

static BCM_Rx_SizeFlag_t gBcmSizeFlag = FIRST_BYTE;
/************************************************************************/
/*                       GLOBAL EXTERN VARIABLES                        */
/************************************************************************/


/************************************************************************/
/*                           APIs IMPLEMENTATION                        */
/************************************************************************/
/**
 * Function : BCM_Init
 * Description: this function is to initialize the BCM
 * @param CallBkFun: A pointer to a function to be called when receiving/Sending Finish
 * @return Status: of the initialization according to the Error handling macros 
 **/

EnmBCMError_t BCM_Init(ptrBCMFunCallBk_t CallBkFun)
{
	EnmBCMError_t API_State = OK_t;
	BCM_Cfg_t.Bcm_ID = BCM_ID;
	gBcm_Rx_Lock = UNLOCKED;
	gBcm_Tx_Lock = UNLOCKED;

	/* The Call Back Setup */
	if( NULL != CallBkFun)
	{
		switch(BCM_Cfg_t.BCM_Rx_or_Tx)
		{
			case RECEIVE_MODE:
			BCM_Cfg_t.BCMPtr_Rx_FuncCallBk = CallBkFun;
			break;
			case SEND_MODE:
			BCM_Cfg_t.BCMPtr_Tx_FuncCallBk = CallBkFun;
			break;
			case SEND_RECEIVE_MODE:
			BCM_Cfg_t.BCMPtr_Rx_FuncCallBk = CallBkFun;
			BCM_Cfg_t.BCMPtr_Tx_FuncCallBk = CallBkFun;
			break;
			default:
			break;
		}
	}
	else
	{
		API_State = INVALID_POINTER_TO_FUNCTION;
	}
	
	switch(Communication_type)
	{
		case UART:
			/* UART Setup */
			UART_Init();
			UART_SetCallBack(BCM_Tx_Callback);
			break;
		case SPI:
			// N
			break;
		case I2C:
			// N
			break;
		default:
			break;
	}
	
	return API_State;
}


/**
 * Function : BCM_Rx_SetupRxBuffer
 * Description: This Function is used to setup the Rx buffer
 * @param Buffer : A pointer to the Buffer in the App Layer
 * @param Size : the Size of the Buffer in the App Layer
 * @return Status: of the Setup according to the Error handling
 **/

EnmBCMError_t BCM_Rx_SetupBuffer(uint8 *Buffer,uint16 Size)
{
	EnmBCMError_t API_State = OK_t;
	
	if( UNLOCKED == gBcm_Rx_Lock)
	{
		if(NULL != Buffer)
		{
			gBcmBuffer.BcmPtrBuffer = Buffer;
			gBcmBuffer.BcmBufferSize = Size;
			gBcmBuffer.BcmCheckSum = ZERO;
			gBufferRxIndex = ZERO;
			gBcm_Rx_state = ID_RECEIVE_STATE;
			gBcmSizeFlag = FIRST_BYTE;
		}
		else
		{
			API_State = INVALID_POINTER_TO_BUFFER;
		}
	}
	else
	{
		API_State = BUFFER_LOCKED;
	}
	
	return API_State;
}


/**
 * Function : BCM_RxDispatcher
 * Description: This Function is used to handling the receiving in run time
 * @return Status: of the Setup according to the Error handling
 **/
EnmBCMError_t BCM_RxDispatcher(void)
{
	EnmBCMError_t API_State = OK_t;
	if( gBcm_Rx_Lock == LOCKED)
	{
		if(gBcm_Rx_Flag == RECEIVED)
		{
			gBcm_Rx_Flag = NOT_RECEIVED;
			switch( gBcm_Rx_state )
			{
			case ID_RECEIVE_STATE:
					if( gBcmBuffer.BcmPtrBuffer[ID_BYTE] == BCM_Cfg_t.Bcm_ID )
					{
						gBufferRxIndex++;
						gBcm_Rx_state = SIZE_RECEIVE_STATE;
					}
					else
					{
						API_State = INVALID_ID;
						BCM_DeInit();
					}
					break;
			case SIZE_RECEIVE_STATE:
					if( gBcmSizeFlag == FIRST_BYTE)
					{
						LengthSizeRx = gBcmBuffer.BcmPtrBuffer[FIRST_BYTE];
						gBufferRxIndex++;
						gBcmSizeFlag = SECOND_BYTE;	
					}
					else if(gBcmSizeFlag == SECOND_BYTE)
					{
						LengthSizeRx |= ( gBcmBuffer.BcmPtrBuffer[SECOND_BYTE] << SHIFT_FACTOR );
						gBufferRxIndex++;
						
						if( LengthSizeRx <= gBcmBuffer.BcmBufferSize )
						{
							gBcm_Rx_state = DATA_RECEIVE_STATE;
						}
						else
						{
							API_State = INVALID_DATA_LENGTH;
							BCM_DeInit();	
						}
					}
					else
					{
						// Do Nothing
					}
				break;
			case DATA_RECEIVE_STATE:				
					gBcmBuffer.BcmCheckSum += gBcmBuffer.BcmPtrBuffer[gBufferRxIndex];
					gBufferRxIndex++;
					
					if( (gBufferRxIndex + BCM_OVERHEAD) == gBcmBuffer.BcmBufferSize)
					{
						gBcm_Rx_state = CHECKSUM_RECEIVE_STATE;
					}
					else
					{
						// Do Nothing
					}
				break;
			case CHECKSUM_RECEIVE_STATE :
					if( gBcmBuffer.BcmCheckSum == gBcmBuffer.BcmPtrBuffer[gBufferRxIndex] )
					{
						BCM_Cfg_t.BCMPtr_Rx_FuncCallBk();
						gBcm_Rx_Lock = UNLOCKED;
					}
					else
					{
						API_State = INVALID_CHECK_SUM;
						BCM_DeInit();	
					}
				break;
			}
		}
		else
		{
			// Do Nothing
		}
	}
	else
	{
		// Do Nothing
	}
	return API_State;
}

/**
 * Function : BCM_DeInit
 * Description: This Function will called to return the BCM to it's Default state
 * @return Status: of the Fu/nction according to the Error handling
 **/
EnmBCMError_t BCM_DeInit(void)
{
	EnmBCMError_t API_State = OK;
	gBufferRxIndex = ZERO;
	LengthSizeRx = ZERO;
	gBcm_Rx_Lock = UNLOCKED;
	gBcm_Tx_Lock = UNLOCKED;
	gBcmBuffer.BcmBufferSize = ZERO;
	gBcmBuffer.BcmPtrBuffer = NULL;
	gBcmBuffer.BcmCheckSum = ZERO;
	gBcm_Rx_state = ID_RECEIVE_STATE;
	gBcm_Rx_Flag = NOT_RECEIVED;
	gBcm_Tx_Flag = NOT_SENT;
	gBcmSizeFlag = FIRST_BYTE;

	switch(Communication_type)
	{
		case UART:
		//UART_DeInit();
		break;
		case SPI:
		// N
		break;
		case I2C:
		// N
		break;
		default:
		break;
	}
	return API_State;
}

/**
 * Function : BCM_Tx_Callback
 * Description: This Function will called in when ISR fires By the Call back Mechanism
 * @return Void
 **/
void BCM_Tx_Callback(void)
{
	gBcm_Tx_Flag = SENT_DONE;
}

/**
 * Function : BCM_Rx_Callback
 * Description: This Function will called in when ISR fires By the Call back Mechanism
 * @return Void
 **/
void BCM_Rx_Callback(void)
{
	gBcmBuffer.BcmPtrBuffer[gBufferRxIndex] = UART_Read();
	gBcm_Rx_Flag = RECEIVED;
	gBcm_Rx_Lock = LOCKED;
}


/**
 * Function : BCM_RxDispatch
 * Description: This Function is used to construct the frame by initializing every element in the structure used
 * Then we lock buffer to make sure that no one corrupts the data which will be sent
 * @return Status: of the Function according to the Error handling
 **/
EnmBCMError_t BCM_Tx_SetupBuffer(uint8 *Buffer,uint16 Size)
{
	EnmBCMError_t API_State = OK_t;
	
	if( UNLOCKED == gBcm_Tx_Lock)
	{
		if ( NULL != Buffer )
		{
			if(Size <= MAX_DATA_SEND)
			{
				gBcmBuffer.BcmCheckSum   = ZERO;
				gBcmBuffer.BcmBufferSize = Size;
				gBcmBuffer.BcmPtrBuffer  = Buffer;
				gBcm_Tx_Lock			 = LOCKED;
				gBcm_Tx_state			 = ID_SEND_STATE;
				gBufferTxIndex			 = ZERO;
				gBcm_Tx_Flag			 = SENT_DONE;
				gBcmSizeFlag			 = FIRST_BYTE;
			}
			else
			{
				API_State = INVALID_DATA_LENGTH;
			}
		}
		else
		{
			API_State = INVALID_POINTER_TO_BUFFER;
		}
	}
	else
	{
		API_State = BUFFER_LOCKED;
	}
	return API_State;
}


/**
 * Function : BCM_TxDispatcher
 * Description: This Function is used to handle the Sending in the run time
 * @return Void
 **/
void BCM_TxDispatcher(void)
{
	if ( LOCKED == gBcm_Tx_Lock )
	{
		if( SENT_DONE == gBcm_Tx_Flag)
		{
			gBcm_Tx_Flag = NOT_SENT;
			switch(gBcm_Tx_state)
			{
				case ID_SEND_STATE:
					BCM_TxSend(BCM_ID);
					gBcm_Tx_state = SIZE_SEND_STATE;
					break;
				case SIZE_SEND_STATE:
					if( gBcmSizeFlag == FIRST_BYTE )
					{
						BCM_TxSend( (uint8) gBcmBuffer.BcmBufferSize);
						gBcmSizeFlag = SECOND_BYTE;
					}
					else if( gBufferTxIndex == SECOND_BYTE )
					{
						BCM_TxSend( (uint8)(gBcmBuffer.BcmBufferSize >> SHIFT_FACTOR) );
						gBcmSizeFlag = FIRST_BYTE;
						gBcm_Tx_state = DATA_SEND_STATE;
					}
					else
					{
						// Do Nothing
					}
					break;
				case DATA_SEND_STATE:
					if ( gBufferTxIndex < gBcmBuffer.BcmBufferSize)
					{
						BCM_TxSend(gBcmBuffer.BcmPtrBuffer[gBufferTxIndex]);
						gBufferTxIndex++;
						gBcmBuffer.BcmCheckSum += gBcmBuffer.BcmPtrBuffer[gBufferTxIndex];
					}
					else if( gBufferTxIndex == gBcmBuffer.BcmBufferSize )
					{
						gBcm_Tx_state = CHECKSUM_SEND_STATE;
					}
					break;
				case CHECKSUM_SEND_STATE:
					BCM_TxSend(gBcmBuffer.BcmCheckSum);
					gBcm_Tx_Lock = UNLOCKED;
					BCM_DeInit();
					break;
				default:
					break;
			}
		}
		else
		{
			// Don Nothing
		}
	}
	else
	{
		// Do Nothing
	}
}


/**
 * Function : BCM_TxSend
 * Description: This Function is used to Send a Byte of data By the Communication Peripheral
 * @return Status: of the Function according to the Error handling
 **/
EnmBCMError_t BCM_TxSend(uint8 data)
{
	EnmBCMError_t API_State = OK_t;
	
	switch(Communication_type)
	{
		case UART:
		/* UART Send */
		UART_Send(data);
		break;
		case SPI:
		// N
		break;
		case I2C:
		// N
		break;
		default:
		API_State = INVALID_ARGUMENT;
		break;
	}
	return API_State;
}