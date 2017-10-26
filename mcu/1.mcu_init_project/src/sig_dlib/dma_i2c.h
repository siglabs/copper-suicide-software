#ifndef __DMA_I2C__
#define __DMA_I2C__

#include <stdlib.h>
#include "mbed.h"
#include "dma_base.h"
#include "../events.h"
#include "../devices/max8688.h"
//#include "../sig_register.h"

//using namespace sigdma;

// TODO(shashank): Move this inside the class but you'd need to figure out if it
// if it will work in dma_init() in stm_hal.h

namespace sig_i2c{



// TODO(shashank): Add specific methods to infer current and voltage.
class DmaI2c : public sigdma::dma_base{
private:
	// ADC related variable decalaration
	void gpio_init();
	void dma_init();

public:
	// Event that needs to be pushed to handle
	struct sig_events::event_b i2c_event;
	DmaI2c();
	void init();
	// takes in source and destination ADDRESS as 32 bit ints and also
	// number of integers needed to transfer the memory.
	void reads(uint16_t slaveAddr, uint32_t readAddr, uint32_t sensor);

	// Raised by the irq, you need to handle it efficiently
	void transfer_complete();
	//~DmaI2c();

};

}//sig_i2c


#endif __DMA_I2C__
