#ifndef __DMA_QSPI__
#define __DMA_QSPI__

#include <stdlib.h>
#include "mbed.h"
#include "dma_base.h"
#include "../events.h"
//#include "../sig_register.h"

//using namespace sigdma;

// TODO(shashank): Move this inside the class but you'd need to figure out if it
// if it will work in dma_init() in stm_hal.h

namespace sig_qspi{



// TODO(shashank): Add specific methods to read and write.
class DmaQspi : public sigdma::dma_base{
private:
	// gpio_init for qspi nor flash
	void gpio_init();
	// start_
	void dma_init();

public:
	// Event that needs to be pushed to handle
	struct sig_events::event_b qspi_event;
	DmaQspi();
	// takes in source and destination ADDRESS as 32 bit ints and also
	// number of integers needed to transfer the memory.
	void start_transaction(uint32_t src, uint32_t dest, uint32_t len);

	// Raised by the irq, you need to handle it efficiently
	void transfer_complete();
	//~dma_adc();

};

}//sig_qspi


#endif __DMA_QSPI__
