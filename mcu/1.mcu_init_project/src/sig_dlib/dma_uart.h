#ifndef __DMA_UART__
#define __DMA_UART__

#include <stdlib.h>
#include "mbed.h"
#include "dma_base.h"
#include "../events.h"
//#include "../sig_register.h"

    /**UART4 GPIO Configuration
    PI9     ------> UART4_RX
    PB14     ------> UART4_RTS
    PB15     ------> UART4_CTS
    PA12     ------> UART4_TX
    */

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */

//using namespace sigdma;

// TODO(shashank): Move this inside the class but you'd need to figure out if it
// if it will work in dma_init() in stm_hal.h

namespace sig_uart{



// TODO(shashank): Add specific methods to infer current and voltage.
class DmaUart : public sigdma::dma_base{
private:

	// ADC related variable declaration
	void gpio_init();
	void dma_init();


public:
	// Event that needs to be pushed to handle
	static constexpr unsigned char *uart_test = (uint8_t*)"UART test signal!\n\r";

	struct sig_events::event_b uart1_event;
	struct sig_events::event_b uart2_event;
	DmaUart();
	void init();
	// takes in source and destination ADDRESS as 32 bit ints and also
	// number of integers needed to transfer the memory.
	void start_tx(uint32_t device, unsigned char *src, uint16_t len);

	// Raised by the irq, you need to handle it efficiently
	// We don't need to handle this for tx, but we do need to
	void transfer_complete();
	//~dma_adc();

};

}//sig_uart


#endif __DMA_UART__
