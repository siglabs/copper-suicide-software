#ifndef __DMA_ADC__
#define __DMA_ADC__


#include "mbed.h"
#include "dma_base.h"
#include "../events.h"
//#include "../sig_register.h"

#include <stdlib.h>
//using namespace sigdma;

// TODO(shashank): Move this inside the class but you'd need to figure out if it
// if it will work in dma_init() in stm_hal.h
static uint32_t adc_buff[16*10];

namespace adc{
/*
 * Pinning information for DMA.
 *
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
// TODO(shashank): Add specific methods to infer current and voltage.
class dma_adc : public sigdma::dma_base{
private:
	// ADC related variable decalaration
	void gpio_init();
	void dma_init();

public:
	struct sig_events::event_b adc_event;
	// We take average of 10 readings to infer the adc values
	dma_adc();
	// takes in source and destination ADDRESS as 32 bit ints and also
	// number of integers needed to transfer the memory.
	void start_transaction(uint32_t src, uint32_t dest, uint32_t len);

	// Raised by the irq, you need to handle it efficiently
	void transfer_complete();
	//~dma_adc();

};

}//adc


#endif __DMA_ADC__
