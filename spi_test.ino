/**
  ******************************************************************************
  * @file    main.c
  * @author  MMY Application Team
  * @version $Revision: 1633 $
  * @date    $Date: 2016-02-10 16:31:14 +0100 (Wed, 10 Feb 2016) $
  * @brief   This file describe the main program.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/** @addtogroup X_NUCLEO_NFC02A1_Applications
  * @{
  */

/** @addtogroup WriteURI_Application
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t NDEF_BUFFER1[NDEF_MAX_SIZE];

uint8_t espCommand = 0;

uint8_t NEED_WIFI = 0x2;
uint8_t WIFI_DATA = 0x3;
uint8_t* Wifissid;
uint8_t* WifiPw;
uint8_t NEED = 1;
uint8_t XNEED = 0;
uint8_t INVALID = -1;

SPI_HandleTypeDef hspi1;

int espComState = 0;
int androidComState = 0;


SsidPassword ssidPass;
uint8_t androidFlag;
uint8_t ssidPassStatus = SSID_STATUS_IDLE;

/* Global variables ----------------------------------------------------------*/
sURI_Info URI;
sURI_Info URI1;
/* I2C handler declaration */
I2C_HandleTypeDef hNFC02A1_i2c;



/* Private function prototypes -----------------------------------------------*/
extern void SystemClock_Config( void );
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void Enable_EnergyHarvesting( void );
void Disable_EnergyHarvesting( void );

void needWifiSPI();
void xneedWifiSPI();
void unknownCommandSPI();
uint8_t* receiveWifiSSID();
uint8_t* receiveWifiPw();
void writeAndroidFlag (uint8_t flag);
uint8_t readAndroidFlag();
void getCommandFromEsp(uint8_t *tempBuffer);
void requestSsidPassword(SsidPassword *ssidPass);
int isRequestSsidPassword();
void writeSsidPassword(SsidPassword ssidpass);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @retval None
  */
int main( void )
{
  /* STM32L0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Low Level Initialization
     */

  /******************************************************************************/
  /* Initialisation of Main STM32 board and X-NUCLEO-NFC02A1 board              */
  /******************************************************************************/
  /* Reset of all peripherals, Initializes the Flash interface and the systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  BSP_LED_Init(LED2);
  /* Init M24LR driver */
  while( BSP_NFCTAG_Init( ) != NFCTAG_OK );

  //init gipo pin as output
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* Init of the Nucleo Board led */


  /******************************************************************************/
  /* Configuration of X-NUCLEO-NFC02A1                                          */
  /******************************************************************************/
  /* Init of the Leds on X-NUCLEO-NFC02A1 board */
  NFC02A1_LED_Init( );

  /* Enable Energy Harvesting */
  Enable_EnergyHarvesting();
  //Disable_EnergyHarvesting();
  /* Check if no NDEF detected, init mem in Tag Type 5 */
  /*if( NfcType5_NDEFDetection( ) != NDEF_OK )
    {
      CCFileStruct.MagicNumber = NFCT5_MAGICNUMBER_E1_CCFILE;
      CCFileStruct.Version = NFCT5_VERSION_V1_0;
      CCFileStruct.MemorySize = ( M24LR_MAX_SIZE / 8 ) & 0xFF;
      CCFileStruct.TT5Tag = 0x05;
       Init of the Type Tag 5 component (M24LR)
      while( NfcType5_TT5Init( ) != NFCTAG_OK );
    }*/
  /* Set the LED2 on to indicate Init done */
  NFC02A1_LED_ON( BLUE_LED );

  NDEF_BUFFER1[0] = 0;
  BSP_NFCTAG_WriteData(NDEF_BUFFER1, 0, 2 );
  while( 1 )
  {
	 espComm();
	 androidComm();
  }
}
//

/******************************************************************************/
/* PRIVATE FUNCTION                                                           */
/******************************************************************************/

/**
  * @brief  This function activate Energy Harvesting mode
  */


void espComm(){
	switch(espComState){
		case IDLE:
			getCommandFromEsp(&espCommand);
			volatile int h;
			h++;
			if(espCommand == NEED_WIFI && ssidPassStatus == REQUEST_SSID_PASS){
				ssidPassStatus = SSID_STATUS_IDLE;
				needWifiSPI();
				espComState = READ_WIFI_SPI;
			}
			else if(espCommand == NEED_WIFI && ssidPassStatus == SSID_STATUS_IDLE){
				xneedWifiSPI();
			}
			else{
				unknownCommandSPI();
			}
			break;
		case READ_WIFI_SPI:
			requestSsidPassword(&ssidPass);
			ssidPassStatus = SSID_PASS_AVAILABLE;	//SsidPassStatus = SSID_PASS_AVAILABLE
			espComState = IDLE;
			volatile int i = 0;
			i++;
			break;
	}
}



void androidComm(){
	switch(androidComState){
	case CHECK_ANDROID_THERE:
		if(isRequestSsidPassword() == 1){
			ssidPassStatus = REQUEST_SSID_PASS;	//SsidPassStatus = REQUEST_SSID_PASS
			androidComState = WRITE_WIFI_EEPROM;
		}
		else{
			ssidPassStatus = SSID_STATUS_IDLE;	//SsidPassStatus = IDLE
		}
		break;
	case WRITE_WIFI_EEPROM:
		if(ssidPassStatus == SSID_PASS_AVAILABLE){	//SsidPassStatus == SSID_PASS_AVAILABLE
			ssidPassStatus = SSID_STATUS_IDLE;	//SsidPassStatus = IDLE
			writeSsidPassword(ssidPass);
			writeAndroidFlag(SSIDPASS_PRESENT);
			androidComState = CHECK_ANDROID_THERE;
		}
		break;
	}

}

void getCommandFromEsp(uint8_t *tempBuffer){
	while((*tempBuffer== 0xff ) || (*tempBuffer == 0x0)){
		while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET);
		HAL_SPI_Receive(&hspi1, tempBuffer, 1, 5000);
	}
}
void requestSsidPassword(SsidPassword *ssidPass){
	ssidPass->Wifissid = receiveWifiSSID();	//requestSsidPassword
	ssidPass->WifiPw = receiveWifiPw();		//requestSsidPassword

	volatile int l;
	l++;
}

int isRequestSsidPassword(){
	/*using push button to stimulate the response*/
	//return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));


	//BSP_NFCTAG_ReadData(NDEF_BUFFER1, 0, 1 );
	HAL_Delay(100);
	if(readAndroidFlag() == ANDROID_REQUESTING){
		writeAndroidFlag(SSIDPASS_NOT_PRESENT);
		//NDEF_BUFFER[0] = SSIDPASS_NOT_PRESENT;
		//BSP_NFCTAG_WriteData(NDEF_BUFFER1, 0, 1 );
		return 1;
	}
	return 0;

}

void writeSsidPassword(SsidPassword ssidpass){
	BSP_NFCTAG_WriteData(ssidpass.Wifissid, 4, 8);
	BSP_NFCTAG_WriteData(ssidpass.WifiPw, 12, 8);

}
void needWifiSPI(){
	HAL_SPI_Transmit(&hspi1, &NEED, 1, 500);
}
void xneedWifiSPI(){
	HAL_SPI_Transmit(&hspi1, &XNEED, 1, 500);
}
void unknownCommandSPI(){
	HAL_SPI_Transmit(&hspi1, &INVALID, 1, 500);
}
uint8_t* receiveWifiSSID(){
	static uint8_t Wifissid [8] = {0};
	int i;
	for(i = 0;i<8;i++){
		while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET);
		HAL_SPI_Receive(&hspi1, Wifissid+i, 1, 5000);
	}
	return Wifissid;
}
uint8_t* receiveWifiPw(){
	static uint8_t WifiPw[8] = {0};
	int i;
	for(i = 0;i<8;i++){
		while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET);
		HAL_SPI_Receive(&hspi1, WifiPw+i, 1, 5000);
	}
	volatile int j =0;
	j++;
	return WifiPw;
}
void writeAndroidFlag (uint8_t flag){
	BSP_NFCTAG_WriteData(&flag, 0, 1 );
}
uint8_t readAndroidFlag(){
	BSP_NFCTAG_ReadData(NDEF_BUFFER1, 0, 1 );
	return NDEF_BUFFER1[0];
}

static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}
void Enable_EnergyHarvesting( void )
{
  /* Initialise M24LR Board */
  if( BSP_NFCTAG_GetExtended_Drv() != NULL )
  {
    /* Enable Energy Harvesting */
    BSP_NFCTAG_GetExtended_Drv()->SetEH( );

    /* Store configuration in non Volatile Memory */
    BSP_NFCTAG_GetExtended_Drv()->Enable_EH_mode();
    BSP_NFCTAG_GetExtended_Drv()->WriteEH_Cfg( M24LR_EH_Cfg_6MA );
  }
}

void Disable_EnergyHarvesting( void )
{
  /* Initialise M24LR Board */
  if( BSP_NFCTAG_GetExtended_Drv() != NULL )
  {
    /* Enable Energy Harvesting */
    //BSP_NFCTAG_GetExtended_Drv()->SetEH( );
    BSP_NFCTAG_GetExtended_Drv()->ResetEH( );
    /* Store configuration in non Volatile Memory */
    BSP_NFCTAG_GetExtended_Drv()->Enable_EH_mode();
    //BSP_NFCTAG_GetExtended_Drv()->Disable_EH_mode();
    BSP_NFCTAG_GetExtended_Drv()->WriteEH_Cfg( M24LR_EH_Cfg_6MA );
  }
}

 /*static void MX_GPIO_Init(void)
{

 // GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable

  __HAL_RCC_GPIOA_CLK_ENABLE();


}*/

 static void MX_GPIO_Init(void)
 {

   GPIO_InitTypeDef GPIO_InitStruct;

    /*GPIO Ports Clock Enable*/
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();

   /*Configure GPIO pin : PushButton_Pin*/
   GPIO_InitStruct.Pin = PushButton_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(PushButton_GPIO_Port, &GPIO_InitStruct);

 }

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  */
void assert_failed( uint8_t* file, uint32_t line )
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while( 1 )
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
