/** @file dma_base.h
 *  @brief DMA wrapper base class.
 *
 *	This is file provides the base for all DMA transactions we need to add to
 *	our device.
 *
 *  @author Shashank Hegde (shashankhegde@gmail.com)
 *  @bug No known bugs.
 */
#ifndef __DMA_BASE__
#define __DMA_BASE__

#include "../sig_register.h"
#include "../mbed.h"
#include "stm32f7xx_hal.h"


#include <stdlib.h>

/*
 * Ensure to include the following in main.cpp as global variable:
 *

	ADC_HandleTypeDef hadc1;
	DMA_HandleTypeDef hdma_adc1;

	I2C_HandleTypeDef hi2c1;
	I2C_HandleTypeDef hi2c2;
	I2C_HandleTypeDef hi2c3;
	DMA_HandleTypeDef hdma_i2c1_rx;
	DMA_HandleTypeDef hdma_i2c1_tx;
	DMA_HandleTypeDef hdma_i2c2_rx;
	DMA_HandleTypeDef hdma_i2c2_tx;
	DMA_HandleTypeDef hdma_i2c3_rx;
	DMA_HandleTypeDef hdma_i2c3_tx;

	QSPI_HandleTypeDef hqspi;
	DMA_HandleTypeDef hdma_quadspi;

	UART_HandleTypeDef huart4;
	UART_HandleTypeDef huart1;
	DMA_HandleTypeDef hdma_uart4_rx;
	DMA_HandleTypeDef hdma_uart4_tx;
	DMA_HandleTypeDef hdma_usart1_rx;
	DMA_HandleTypeDef hdma_usart1_tx;

	NOR_HandleTypeDef hnor1;
 */

/*
 * NOTE: please ensure that you include stm32f7xx_hal.h in order to enable all
 * necessary inits for GPIOs.
 */

 extern  sig_events::EventManager EVENT_HANDLER;

 extern  ADC_HandleTypeDef hadc1;
 extern  DMA_HandleTypeDef hdma_adc1;

 extern  I2C_HandleTypeDef hi2c1;
 extern  I2C_HandleTypeDef hi2c2;
 extern  I2C_HandleTypeDef hi2c3;
 extern  DMA_HandleTypeDef hdma_i2c1_rx;
 extern  DMA_HandleTypeDef hdma_i2c1_tx;
 extern  DMA_HandleTypeDef hdma_i2c2_rx;
 extern  DMA_HandleTypeDef hdma_i2c2_tx;
 extern  DMA_HandleTypeDef hdma_i2c3_rx;
 extern  DMA_HandleTypeDef hdma_i2c3_tx;

 extern  QSPI_HandleTypeDef hqspi;
 extern  DMA_HandleTypeDef hdma_quadspi;

 extern  UART_HandleTypeDef huart4;
 extern  UART_HandleTypeDef huart1;
 extern  DMA_HandleTypeDef hdma_uart4_rx;
 extern  DMA_HandleTypeDef hdma_uart4_tx;
 extern  DMA_HandleTypeDef hdma_usart1_rx;
 extern  DMA_HandleTypeDef hdma_usart1_tx;

 extern  NOR_HandleTypeDef hnor1;

namespace sigdma{
/*
 * i2c_a -> i2c1 -> (PB9 sda, PB8 scl)
 * i2c_b -> i2c2 -> (PH5 sda, PH4 scl)
 * i2c_c -> i2c3 -> (PH8 sda, PH7 scl)
 *
 * USART 1 -> PA_9(tx)
 * 		   -> PA_10(rx)
 * UART 4  -> PH13(tx)
 * 		   -> PH14(rx)
 *
 */

/*
 * AnalogIn telem0(PC_0); //10
 * AnalogIn telem1(PC_1); //11
 * AnalogIn telem2(PC_3); //13
 * AnalogIn telem3(PC_4); //14
 * AnalogIn telem4(PC_5); //15
 * AnalogIn telem5(PB_0); //8
 * AnalogIn telem6(PB_1); //9
 * AnalogIn telem7(PA_0); //0
 * AnalogIn telem8(PA_1); //1
 * AnalogIn telem9(PA_2); //2
 * AnalogIn telem10(PA_3); //3
 * AnalogIn telem11(PA_4); //4
 * AnalogIn telem12(PA_5); //5
 * AnalogIn telem13(PA_6); //6
 * AnalogIn telem14(PA_7); //7
 * AnalogIn telem15(PC_2); //12
 *
 */

// Indicate DMA error on LED-> Red.
static DigitalOut CS_LED0(PC_8);

typedef enum dma_comm_type{
	MEM2MEM,	// Memory to transfers needed, saved to service future needs.
	DMA_ADC,	// ADC transaction (ADC1 -> 0-15).
	DMA_I2CA,	// i2a to i2c to read data from max8688s lulz :)
	DMA_I2CB,	//
	DMA_I2CC,	//
	DMA_USART1, // uart that connects to either of error/console log
	DMA_UART4,  // uart that connects to either of error/console log
	DMA_FLASH   // for QSPI flash transaction
} dma_type;


enum trandfer_mode{
	AUTO_INCREMENT,
	SRCAUTO_DSTMANUAL,
	SRCMANUAL_DSTAUTO,
	MANUAL };

class dma_base{

public:
	// TODO(shashank):
	// dma_typedef, this should be played as needed from the inherited class.
	//static DMA_HandleTypeDef dma_handle;

	// ADC related variable decalaration
	ADC_ChannelConfTypeDef adc_config;
	//static ADC_HandleTypeDef hadc1;

	// private method that is needed to handle error.
	// prints the line the file name and line number for sole debugging
	// purpose.
	void error_handler(char *file, int line);

	// Base constructor, does nothing but initializes the HAL
	dma_base();

	// TODO(shashank): GPIOinit() and dmaInit() should happen here, use #if decs
	void init();

	// dma_legal_chack: this method should read Channels, directions and stream
	// and verify whether or not the specified DMA can be initiated.
	// This should be a light weight function.
	void dma_legal_check();

	// init function wrapper.

	//dma_base(dma_type);

	// method that configures stream and channel stuff
	// returns -1  on error.
	//
	// Ideally this function should be used only if one wishes to override the
	// default constructors.
	//
	// TODO(shashank): Ensure that this function verifies legality of the
	// change at the current status of access.
	int configure(DMA_HandleTypeDef overrider);

	// explicit mode updation method
	// possible modes:
	// 	1. Auto increment src/dst
	//	2.
	int config_mem_mode(int mode=AUTO_INCREMENT);

	// SET priority
	// Each DMA port has an arbiter for handling the priority between other
	// DMA streams. Stream priority is software-configurable (there are four
	// software levels). If two or more DMA streams have the same software
	// priority level, the hardware priority is used (stream 0 has priority
	// over stream 1, etc.).
	//
	// #define DMA_PRIORITY_LOW             ((uint32_t)0x00000000U)    /*!< Priority level: Low       */
	// #define DMA_PRIORITY_MEDIUM          ((uint32_t)DMA_SxCR_PL_0)  /*!< Priority level: Medium    */
	// #define DMA_PRIORITY_HIGH            ((uint32_t)DMA_SxCR_PL_1)  /*!< Priority level: High      */
	// #define DMA_PRIORITY_VERY_HIGH       ((uint32_t)DMA_SxCR_PL)    /*!< Priority level: Very High */
	int config_priority(int priority=DMA_PRIORITY_MEDIUM);

	//

};

}// sigdma

#endif __DMA_BASE__

