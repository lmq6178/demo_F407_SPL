#include "Hardware_RCC.h"

void RCC_Init(void)
{
	ErrorStatus		HSEStartUpStatus;
	
	/*把RCC外设初始化为复位状态*/
//	RCC_DeInit();
		
	RCC_ClockSecuritySystemCmd(ENABLE);
	
	/*使能HSE，开启外部晶振，这里是8MHz*/
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/*配置FLASH预取和等待状态*/
		FLASH_PrefetchBufferCmd(ENABLE);
		FLASH_InstructionCacheCmd(ENABLE);
        FLASH_DataCacheCmd(ENABLE);
		FLASH_SetLatency(FLASH_Latency_1);		//对于48MHz需要一个等待状态,查STM32F4系列等待状态判断表
		
		/*配置总线分频*/
		RCC_HCLKConfig(RCC_SYSCLK_Div1);      // AHB = 48MHz                              
		RCC_PCLK1Config(RCC_HCLK_Div1);		  // APB1 = 48MHz
		RCC_PCLK2Config(RCC_HCLK_Div1);       // APB2 = 48MHz
		
		/*配置PLL
		*M:输入分频;N倍频系数;P:系统时钟分频;N:USB/SDIO时钟分频
		*HSE = 8MHz, PLL_M = 8, PLL_N = 96, PLL_P = 2, PLL_Q = 4
		*VCO = (HSE / PLL_M) * PLL_N = (8MHz / 8) * 96 = 96MHz
		*System Clock = VCO / PLL_P = 96MHz / 2 = 48MHz
		*USB OTG等 = VCO / PLL_Q = 96MHz / 4 = 24MHz
		*/
		RCC_PLLConfig(RCC_PLLSource_HSE,RCC_PLLCFGR_PLLM_3,96,2,4);
		RCC_PLLCmd(ENABLE);
                    
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08)                  
		{
		}
	}
}
