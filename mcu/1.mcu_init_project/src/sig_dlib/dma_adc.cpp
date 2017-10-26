#include "dma_adc.h"


using namespace sig_events;
//namespace sigdma{
namespace adc{
dma_adc::dma_adc()
{

  this->gpio_init();
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  if (HAL_ADC_Init(&hadc1) != HAL_OK) {
    this->error_handler(__FILE__, __LINE__);
  }

  adc_config.Channel = ADC_CHANNEL_0;
  adc_config.Rank = 1;
  adc_config.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &adc_config) != HAL_OK)
  {
    this->error_handler(__FILE__, __LINE__);
  }

  this->dma_init();

}

void dma_adc::gpio_init(){
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

void dma_adc::dma_init(){
	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

void dma_adc::start_transaction(uint32_t src=ADC1_BASE, uint32_t dest=(uint32_t)adc_buff, uint32_t len=160){
	if(HAL_DMA_Start_IT(&hdma_adc1, src, (uint32_t)dest, len) != HAL_OK){
		this->error_handler(__FILE__, __LINE__);
	}
}

void dma_adc::transfer_complete(){
#ifdef EVENT_ENGINE
	EVENT_HANDLER.e_push_circBuf(adc_event);
#endif
}

}// adc
//}//sigdma
