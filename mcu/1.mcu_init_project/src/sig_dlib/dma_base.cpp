#include "dma_base.h"

namespace sigdma{
/*
dma_base::dma_base(){

	__HAL_RCC_DMA2_CLK_ENABLE();
	// Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0
	dma_handle.Instance = DMA2_Stream0;
	dma_handle.Init.Channel = DMA_CHANNEL_0;
	dma_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	dma_handle.Init.PeriphInc = DMA_PINC_ENABLE;
	dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dma_handle.Init.Mode = DMA_NORMAL;
	dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
	dma_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
	dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK) {
		this->error_handler(__FILE__, __LINE__);
	}
	// DMA interrupt init
	// DMA2_Stream0_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}
*/

dma_base::dma_base(){
	// This one function takes care of all the necessary function calls needed
	// Please refrain from calling any HAL_Inits later in the code.
	//HAL_Init();
}


// TODO(shashank): finish it :)
int dma_base::configure(DMA_HandleTypeDef overrider){
	return -1;
}

void dma_base::error_handler(char *file, int line){
	CS_LED0 = CS_LED0?0:1;
	wait(0.2);
}

}//sigdma
