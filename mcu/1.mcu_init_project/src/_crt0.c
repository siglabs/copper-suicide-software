/** @file _crt0.c
 *  @brief Initialization of ARM Cortex-M7
 *
 *  Roles:
 *    - Defines the interrupt vector
 *    - Sets the stack pointer
 *    - Copies DATA region from FLASH to RAM
 *    - Initializes BSS region
 *    - Initializes C++ static ctors
 *    - Initializes platform specific hardware
 *    - Initializes platform specific clocks
 *    - Jumps to main()
 *
 *  @author JD Brinton (joel@siglabs.com)
 *  @bug No know bugs.
 */

/* -- Linker Definitions -- */
/* -------------------------*/

extern void (*__preinit_array_start[]) (void) __attribute__((weak));
extern void (*__preinit_array_end[]) (void) __attribute__((weak));
extern void (*__init_array_start[]) (void) __attribute__((weak));
extern void (*__init_array_end[]) (void) __attribute__((weak));

extern unsigned int __data_lma_start;
extern unsigned int __data_vma_start;
extern unsigned int __data_vma_end;

extern unsigned int __bss_vma_start;
extern unsigned int __bss_vma_end;

extern unsigned int _estack;




/* -- Platform Specific Forward Definitions -- */
/* --------------------------------------------*/

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

//extern void Default_Handler_x(void);
extern int main(void);




/* -- ctors/dtors hooks -- */
/* ------------------------*/

void _fini(void) {
}

void _init(void) {
}

void _getpid(void) {
}

void _kill(void) {
}

void *__dso_handle;




/* -- _crt0 -- */
/* ------------*/

inline void __attribute__((always_inline)) __initialize_data (unsigned int* from, unsigned int* region_begin, unsigned int* region_end)
{
  // Iterate and copy word by word.
  // It is assumed that the pointers are word aligned.
  unsigned int *p = region_begin;
  while (p < region_end)
    *p++ = *from++;
}

inline void __attribute__((always_inline)) __initialize_bss (unsigned int* region_begin, unsigned int* region_end)
{
  // Iterate and clear word by word.
  // It is assumed that the pointers are word aligned.
  unsigned int *p = region_begin;
  while (p < region_end)
    *p++ = 0;
}


inline void __attribute__((always_inline)) __run_init_array (void)
{
  // Iterate over all the preinit/init routines (mainly static constructors).
  int count;
  int i;

  count = __preinit_array_end - __preinit_array_start;
  for (i = 0; i < count; i++)
    __preinit_array_start[i] ();

  count = __init_array_end - __init_array_start;
  for (i = 0; i < count; i++)
    __init_array_start[i] ();
}


void _crt0(void)
{
  __asm__("ldr   sp, =_estack");
  __initialize_data(&__data_lma_start, &__data_vma_start, &__data_vma_end);
  __initialize_bss(&__bss_vma_start, &__bss_vma_end);
  SystemInit();
  SystemCoreClockUpdate();
  __run_init_array();

  __asm__("bl main");
}


/* -- functions -- */
/* ----------------*/

void Default_Handler(void) {
  while(1){
//	  Default_Handler_x();
  }
}



/* -- type definitions -- */
/* -----------------------*/

typedef void (* const pHandler)(void);

void __attribute__ ((weak, alias ("Default_Handler"))) NMI_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) HardFault_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) MemManage_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) BusFault_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) UsageFault_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SVC_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DebugMon_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) PendSV_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SysTick_Handler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) WWDG_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) PVD_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TAMP_STAMP_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) RTC_WKUP_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) FLASH_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) RCC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream6_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) ADC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN1_TX_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN1_RX0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN1_RX1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN1_SCE_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI9_5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM1_BRK_TIM9_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM1_UP_TIM10_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM1_TRG_COM_TIM11_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM1_CC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C1_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C1_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C2_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C2_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) USART1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) USART2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) USART3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) EXTI15_10_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) RTC_Alarm_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_FS_WKUP_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM8_BRK_TIM12_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM8_UP_TIM13_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM8_TRG_COM_TIM14_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM8_CC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA1_Stream7_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) FMC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SDMMC1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) UART4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) UART5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM6_DAC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) TIM7_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) ETH_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) ETH_WKUP_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN2_TX_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN2_RX0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN2_RX1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN2_SCE_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_FS_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream6_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2_Stream7_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) USART6_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C3_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C3_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_HS_EP1_OUT_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_HS_EP1_IN_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_HS_WKUP_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) OTG_HS_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DCMI_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) RNG_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) FPU_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) UART7_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) UART8_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI4_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI5_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPI6_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SAI1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) LTDC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) LTDC_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DMA2D_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SAI2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) QUADSPI_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) LPTIM1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CEC_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C4_EV_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) I2C4_ER_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SPDIF_RX_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DSI_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DFSDM1_FLT0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DFSDM1_FLT1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DFSDM1_FLT2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) DFSDM1_FLT3_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) SDMMC2_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN3_TX_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN3_RX0_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN3_RX1_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) CAN3_SCE_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) JPEG_IRQHandler(void);
void __attribute__ ((weak, alias ("Default_Handler"))) MDIOS_IRQHandler(void);





/* -- ISR vector -- */
/* -----------------*/


__attribute__ ((section(".isr_vector"),used))
pHandler gHandlers[] =
{
    (pHandler) &_estack,
    (pHandler) _crt0,
    (pHandler) NMI_Handler,
    (pHandler) HardFault_Handler,
    (pHandler) MemManage_Handler,
    (pHandler) BusFault_Handler,
    (pHandler) UsageFault_Handler,
    (pHandler) 0,
    (pHandler) 0,
    (pHandler) 0,
    (pHandler) 0,
    (pHandler) SVC_Handler,
    (pHandler) DebugMon_Handler,
    (pHandler) 0,
    (pHandler) PendSV_Handler,
    (pHandler) SysTick_Handler,

    /* External Interrupts */

    (pHandler) WWDG_IRQHandler,                  /* Window WatchDog              */
    (pHandler) PVD_IRQHandler,                   /* PVD through EXTI Line detection */
    (pHandler) TAMP_STAMP_IRQHandler,            /* Tamper and TimeStamps through the EXTI line */
    (pHandler) RTC_WKUP_IRQHandler,              /* RTC Wakeup through the EXTI line */
    (pHandler) FLASH_IRQHandler,                 /* FLASH                        */
    (pHandler) RCC_IRQHandler,                   /* RCC                          */
    (pHandler) EXTI0_IRQHandler,                 /* EXTI Line0                   */
    (pHandler) EXTI1_IRQHandler,                 /* EXTI Line1                   */
    (pHandler) EXTI2_IRQHandler,                 /* EXTI Line2                   */
    (pHandler) EXTI3_IRQHandler,                 /* EXTI Line3                   */
    (pHandler) EXTI4_IRQHandler,                 /* EXTI Line4                   */
    (pHandler) DMA1_Stream0_IRQHandler,          /* DMA1 Stream 0                */
    (pHandler) DMA1_Stream1_IRQHandler,          /* DMA1 Stream 1                */
    (pHandler) DMA1_Stream2_IRQHandler,          /* DMA1 Stream 2                */
    (pHandler) DMA1_Stream3_IRQHandler,          /* DMA1 Stream 3                */
    (pHandler) DMA1_Stream4_IRQHandler,          /* DMA1 Stream 4                */
    (pHandler) DMA1_Stream5_IRQHandler,          /* DMA1 Stream 5                */
    (pHandler) DMA1_Stream6_IRQHandler,          /* DMA1 Stream 6                */
    (pHandler) ADC_IRQHandler,                   /* ADC1, ADC2 and ADC3s         */
    (pHandler) CAN1_TX_IRQHandler,               /* CAN1 TX                      */
    (pHandler) CAN1_RX0_IRQHandler,              /* CAN1 RX0                     */
    (pHandler) CAN1_RX1_IRQHandler,              /* CAN1 RX1                     */
    (pHandler) CAN1_SCE_IRQHandler,              /* CAN1 SCE                     */
    (pHandler) EXTI9_5_IRQHandler,               /* External Line[9:5]s          */
    (pHandler) TIM1_BRK_TIM9_IRQHandler,         /* TIM1 Break and TIM9          */
    (pHandler) TIM1_UP_TIM10_IRQHandler,         /* TIM1 Update and TIM10        */
    (pHandler) TIM1_TRG_COM_TIM11_IRQHandler,    /* TIM1 Trigger and Commutation and TIM11 */
    (pHandler) TIM1_CC_IRQHandler,               /* TIM1 Capture Compare         */
    (pHandler) TIM2_IRQHandler,                  /* TIM2                         */
    (pHandler) TIM3_IRQHandler,                  /* TIM3                         */
    (pHandler) TIM4_IRQHandler,                  /* TIM4                         */
    (pHandler) I2C1_EV_IRQHandler,               /* I2C1 Event                   */
    (pHandler) I2C1_ER_IRQHandler,               /* I2C1 Error                   */
    (pHandler) I2C2_EV_IRQHandler,               /* I2C2 Event                   */
    (pHandler) I2C2_ER_IRQHandler,               /* I2C2 Error                   */
    (pHandler) SPI1_IRQHandler,                  /* SPI1                         */
    (pHandler) SPI2_IRQHandler,                  /* SPI2                         */
    (pHandler) USART1_IRQHandler,                /* USART1                       */
    (pHandler) USART2_IRQHandler,                /* USART2                       */
    (pHandler) USART3_IRQHandler,                /* USART3                       */
    (pHandler) EXTI15_10_IRQHandler,             /* External Line[15:10]s        */
    (pHandler) RTC_Alarm_IRQHandler,             /* RTC Alarm (A and B) through EXTI Line */
    (pHandler) OTG_FS_WKUP_IRQHandler,           /* USB OTG FS Wakeup through EXTI line */
    (pHandler) TIM8_BRK_TIM12_IRQHandler,        /* TIM8 Break and TIM12         */
    (pHandler) TIM8_UP_TIM13_IRQHandler,         /* TIM8 Update and TIM13        */
    (pHandler) TIM8_TRG_COM_TIM14_IRQHandler,    /* TIM8 Trigger and Commutation and TIM14 */
    (pHandler) TIM8_CC_IRQHandler,               /* TIM8 Capture Compare         */
    (pHandler) DMA1_Stream7_IRQHandler,          /* DMA1 Stream7                 */
    (pHandler) FMC_IRQHandler,                   /* FMC                          */
    (pHandler) SDMMC1_IRQHandler,                /* SDMMC1                       */
    (pHandler) TIM5_IRQHandler,                  /* TIM5                         */
    (pHandler) SPI3_IRQHandler,                  /* SPI3                         */
    (pHandler) UART4_IRQHandler,                 /* UART4                        */
    (pHandler) UART5_IRQHandler,                 /* UART5                        */
    (pHandler) TIM6_DAC_IRQHandler,              /* TIM6 and DAC1&2 underrun errors */
    (pHandler) TIM7_IRQHandler,                  /* TIM7                         */
    (pHandler) DMA2_Stream0_IRQHandler,          /* DMA2 Stream 0                */
    (pHandler) DMA2_Stream1_IRQHandler,          /* DMA2 Stream 1                */
    (pHandler) DMA2_Stream2_IRQHandler,          /* DMA2 Stream 2                */
    (pHandler) DMA2_Stream3_IRQHandler,          /* DMA2 Stream 3                */
    (pHandler) DMA2_Stream4_IRQHandler,          /* DMA2 Stream 4                */
    (pHandler) ETH_IRQHandler,                   /* Ethernet                     */
    (pHandler) ETH_WKUP_IRQHandler,              /* Ethernet Wakeup through EXTI line */
    (pHandler) CAN2_TX_IRQHandler,               /* CAN2 TX                      */
    (pHandler) CAN2_RX0_IRQHandler,              /* CAN2 RX0                     */
    (pHandler) CAN2_RX1_IRQHandler,              /* CAN2 RX1                     */
    (pHandler) CAN2_SCE_IRQHandler,              /* CAN2 SCE                     */
    (pHandler) OTG_FS_IRQHandler,                /* USB OTG FS                   */
    (pHandler) DMA2_Stream5_IRQHandler,          /* DMA2 Stream 5                */
    (pHandler) DMA2_Stream6_IRQHandler,          /* DMA2 Stream 6                */
    (pHandler) DMA2_Stream7_IRQHandler,          /* DMA2 Stream 7                */
    (pHandler) USART6_IRQHandler,                /* USART6                       */
    (pHandler) I2C3_EV_IRQHandler,               /* I2C3 event                   */
    (pHandler) I2C3_ER_IRQHandler,               /* I2C3 error                   */
    (pHandler) OTG_HS_EP1_OUT_IRQHandler,        /* USB OTG HS End Point 1 Out   */
    (pHandler) OTG_HS_EP1_IN_IRQHandler,         /* USB OTG HS End Point 1 In    */
    (pHandler) OTG_HS_WKUP_IRQHandler,           /* USB OTG HS Wakeup through EXTI */
    (pHandler) OTG_HS_IRQHandler,                /* USB OTG HS                   */
    (pHandler) DCMI_IRQHandler,                  /* DCMI                         */
    (pHandler) 0,                                /* Reserved                     */
    (pHandler) RNG_IRQHandler,                   /* RNG                          */
    (pHandler) FPU_IRQHandler,                   /* FPU                          */
    (pHandler) UART7_IRQHandler,                 /* UART7                        */
    (pHandler) UART8_IRQHandler,                 /* UART8                        */
    (pHandler) SPI4_IRQHandler,                  /* SPI4                         */
    (pHandler) SPI5_IRQHandler,                  /* SPI5                         */
    (pHandler) SPI6_IRQHandler,                  /* SPI6                         */
    (pHandler) SAI1_IRQHandler,                  /* SAI1                         */
    (pHandler) LTDC_IRQHandler,                  /* LTDC                         */
    (pHandler) LTDC_ER_IRQHandler,               /* LTDC error                   */
    (pHandler) DMA2D_IRQHandler,                 /* DMA2D                        */
    (pHandler) SAI2_IRQHandler,                  /* SAI2                         */
    (pHandler) QUADSPI_IRQHandler,               /* QUADSPI                      */
    (pHandler) LPTIM1_IRQHandler,                /* LPTIM1                       */
    (pHandler) CEC_IRQHandler,                   /* HDMI_CEC                     */
    (pHandler) I2C4_EV_IRQHandler,               /* I2C4 Event                   */
    (pHandler) I2C4_ER_IRQHandler,               /* I2C4 Error                   */
    (pHandler) SPDIF_RX_IRQHandler,              /* SPDIF_RX                     */
    (pHandler) DSI_IRQHandler,                   /* DSI                          */
    (pHandler) DFSDM1_FLT0_IRQHandler,           /* DFSDM1 Filter 0 global Interrupt */
    (pHandler) DFSDM1_FLT1_IRQHandler,           /* DFSDM1 Filter 1 global Interrupt */
    (pHandler) DFSDM1_FLT2_IRQHandler,           /* DFSDM1 Filter 2 global Interrupt */
    (pHandler) DFSDM1_FLT3_IRQHandler,           /* DFSDM1 Filter 3 global Interrupt */
    (pHandler) SDMMC2_IRQHandler,                /* SDMMC2                       */
    (pHandler) CAN3_TX_IRQHandler,               /* CAN3 TX                      */
    (pHandler) CAN3_RX0_IRQHandler,              /* CAN3 RX0                     */
    (pHandler) CAN3_RX1_IRQHandler,              /* CAN3 RX1                     */
    (pHandler) CAN3_SCE_IRQHandler,              /* CAN3 SCE                     */
    (pHandler) JPEG_IRQHandler,                  /* JPEG                         */
    (pHandler) MDIOS_IRQHandler,                 /* MDIOS                        */
};




