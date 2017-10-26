/** @file main.cpp
 *  @brief Entry point for Copper Suicide MCU
 *
 *  Entry point for Copper Suicide MCU. Uses the MBED OS
 *  device library in addition to several component specific
 *  libraries.
 *
 *
 *  @author JD Brinton (joel@siglabs.com)
 *  @bug No know bugs.
 */


#include <iostream>

#include "mbed.h"

#include "max8688.h"

//#include "events.h"
//#include "sig_dlib/dma_base.h"
//#include "sig_dlib/dma_adc.h"
//#include "sig_dlib/dma_uart.h"
#include "stm32f7xx_hal.h"

//#include "sig_register.h"

//using namespace std;
//using namespace mbed;
//using namespace sig_events;
//using namespace sigdma;
//using namespace adc;
//using namespace sig_uart;


using namespace std;

__IO ITStatus UartReady = RESET;

//EventManager EVENT_HANDLER(100);

/* Private variables ---------------------------------------------------------*/
//ADC_HandleTypeDef hadc1;
//DMA_HandleTypeDef hdma_adc1;
//
//I2C_HandleTypeDef hi2c1;
//I2C_HandleTypeDef hi2c2;
//I2C_HandleTypeDef hi2c3;
//DMA_HandleTypeDef hdma_i2c1_rx;
//DMA_HandleTypeDef hdma_i2c1_tx;
//DMA_HandleTypeDef hdma_i2c2_rx;
//DMA_HandleTypeDef hdma_i2c2_tx;
//DMA_HandleTypeDef hdma_i2c3_rx;
//DMA_HandleTypeDef hdma_i2c3_tx;
//
//QSPI_HandleTypeDef hqspi;
//DMA_HandleTypeDef hdma_quadspi;
//
//UART_HandleTypeDef huart4;
//UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_uart4_rx;
//DMA_HandleTypeDef hdma_uart4_tx;
//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma_usart1_tx;
//
//NOR_HandleTypeDef hnor1;


Serial stdcon(PA_9, PA_10); // tx, rx
Serial errcon(PH_13, PH_14); // tx, rx

DigitalOut CS_LED1(PB_5);
DigitalOut CS_LED0(PC_8);
DigitalOut CS_LED2(PC_9); //red


DigitalOut FAN(PA_8);

//DmaUart UART1_OBJ;
//dma_adc DMA_ADC_OBJ;


I2C i2ca(PB_9, PB_8);
I2C i2cb(PH_5, PH_4);
I2C i2cc(PH_8, PH_7);

//DigitalIn i2cax(PB_9);


AnalogIn telem0(PC_0);
AnalogIn telem1(PC_1);
AnalogIn telem2(PC_3);
AnalogIn telem3(PC_4);
AnalogIn telem4(PC_5);
AnalogIn telem5(PB_0);
AnalogIn telem6(PB_1);
AnalogIn telem7(PA_0);
AnalogIn telem8(PA_1);
AnalogIn telem9(PA_2);
AnalogIn telem10(PA_3);
AnalogIn telem11(PA_4);
AnalogIn telem12(PA_5);
AnalogIn telem13(PA_6);
AnalogIn telem14(PA_7);
AnalogIn telem15(PC_2);

extern "C" void _Error_Handler(char * file, int line);

void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
	  	CS_LED0 = CS_LED0?0:1;
	  	wait(0.2);
  }
  /* USER CODE END Error_Handler_Debug */
}


void print_board_status(){
    stdcon.printf("\033[2J");
    stdcon.printf("\033[1;37m"); //white
    stdcon.printf("\r\n\r\nCopper Suicide (V1.0) Telemetry System\r\n");
    stdcon.printf(        "--------------------------------------\r\n");
    stdcon.printf("\033[0;31m"); //red

    stdcon.printf("5V Supply                                Current: %5.3f A\n\r", telem14.read() * 0.5f); // I5_SNS
    stdcon.printf("3.3V Supply                              Current: %5.3f A\n\r", (1.0f-telem0.read()) * 2.357f); // I3V3_SNS
    stdcon.printf("2.5V Supply Voltage:         %5.3f V     Current: %5.3f A\n\r", telem2.read() * 10.3125f, (1.0f-telem1.read()) * 6.6f); // V2V5_SNS, I2V5_SNS
    stdcon.printf("1.35VA Supply Voltage:       %5.3f V     Current: %5.3f A\n\r", telem4.read() * 3.3f, (1.0f-telem3.read()) * 16.5f); // V1V35, I1V35_SNSA
    stdcon.printf("1.35VB Supply Voltage:       %5.3f V     Current: %5.3f A\n\r", telem4.read() * 3.3f, (1.0f-telem5.read()) * 16.5f); // V1V35, I1V35_SNSB
    stdcon.printf("J3.VTT Voltage:              %5.3f V\n\r", telem7.read() * 3.3f); // J3.VTT
    stdcon.printf("J3.VIO Voltage:              %5.3f V\n\r", telem8.read() * 3.3f); // J3.VIO
    stdcon.printf("J4.VTT Voltage:              %5.3f V\n\r", telem9.read() * 3.3f); // J4.VTT
    stdcon.printf("J4.VIO Voltage:              %5.3f V\n\r", telem10.read() * 3.3f); // J4.VIO
    stdcon.printf("J2.VTT Voltage:              %5.3f V\n\r", telem11.read() * 3.3f); // J2.VTT
    stdcon.printf("J2.VIO Voltage:              %5.3f V\n\r", telem12.read() * 3.3f); // J2.VIO
    stdcon.printf("J1.VTT Voltage:              %5.3f V\n\r", telem13.read() * 3.3f); // J1.VTT
    stdcon.printf("J1.VIO Voltage:              %5.3f V\n\r", telem14.read() * 3.3f); // J1.VIO

    stdcon.printf("FPGA00 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA00_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA00_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA00_ADDR));
    stdcon.printf("FPGA01 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA01_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA01_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA01_ADDR));
    stdcon.printf("FPGA02 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA02_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA02_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA02_ADDR));
    stdcon.printf("FPGA03 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA03_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA03_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA03_ADDR));
    stdcon.printf("FPGA10 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA10_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA10_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA10_ADDR));
    stdcon.printf("FPGA11 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA11_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA11_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA11_ADDR));
    stdcon.printf("FPGA12 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA12_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA12_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA12_ADDR));
    stdcon.printf("FPGA13 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2ca, I2CA_FPGA13_ADDR), MAX8688_Get_Current(&i2ca, I2CA_FPGA13_ADDR), MAX8688_Get_Temperature(&i2ca, I2CA_FPGA13_ADDR));
    stdcon.printf("FPGA20 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA20_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA20_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA20_ADDR));
    stdcon.printf("FPGA21 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA21_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA21_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA21_ADDR));
    stdcon.printf("FPGA22 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA22_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA22_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA22_ADDR));
    stdcon.printf("FPGA23 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA23_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA23_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA23_ADDR));
    stdcon.printf("FPGA30 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA30_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA30_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA30_ADDR));
    stdcon.printf("FPGA31 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA31_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA31_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA31_ADDR));
    stdcon.printf("FPGA32 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA32_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA32_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA32_ADDR));
    stdcon.printf("FPGA33 Voltage:              %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cb, I2CB_FPGA33_ADDR), MAX8688_Get_Current(&i2cb, I2CB_FPGA33_ADDR), MAX8688_Get_Temperature(&i2cb, I2CB_FPGA33_ADDR));
    stdcon.printf("FPGA CFG Voltage:            %5.3f V     Current: %5.3f A     Temperature: %4.1f degC\n\r", MAX8688_Get_Voltage(&i2cc, I2CC_FPGA_CFG_ADDR), MAX8688_Get_Current(&i2cc, I2CC_FPGA_CFG_ADDR), MAX8688_Get_Temperature(&i2cc, I2CC_FPGA_CFG_ADDR));
//    i2ca.unlock();



}

void cycle_fpgas(){
	  for (int i = 0; i < 16; i++ ){
		  if (i < 8)
			  // Turn off row 1 and 2
			  MAX8688_Power_FPGA(&i2ca, map_fpgas[i], ON_MARGIN_OFF);
		  else
			  // turn off row 2 and 3
			  MAX8688_Power_FPGA(&i2cb, map_fpgas[i], ON_MARGIN_OFF);
	  }
	//  CS_LED1 = CS_LED1?0:1;
	  // we wait a total of 10 second before we bring up remaining consortium of
	  // FPGAs
	  wait(1);
	  for (int i = 0; i < 16; i++ ){
	  	  if (i < 8)
	  		  // Turn ON row 1 and 2
	  		  MAX8688_Power_FPGA(&i2ca, map_fpgas[i], IMMEDIATE_OFF);
	  	  else
	  		  // turn ON row 2 and 3
	  		  MAX8688_Power_FPGA(&i2cb, map_fpgas[i], IMMEDIATE_OFF);
	    }
}


int main(void)
{
  unsigned idx;
  unsigned addy;

  i2ca.frequency(100000);
//  i2cax.mode(PullUp);
  i2cb.frequency(100000);
  i2cc.frequency(100000);

  FAN = 1;

  MAX8688_Init(&i2ca, I2CA_FPGA00_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA01_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA02_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA03_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA10_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA11_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA12_ADDR);
  MAX8688_Init(&i2ca, I2CA_FPGA13_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA20_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA21_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA22_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA23_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA30_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA31_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA32_ADDR);
  MAX8688_Init(&i2cb, I2CB_FPGA33_ADDR);
  MAX8688_Init(&i2cb, I2CC_FPGA_CFG_ADDR);
  // Prints status of the board
  print_board_status();

  uint8_t *address;
  uint8_t *command;
  uint8_t count = 1;
//
  CS_LED0 = 1;
  // Turn Off all FPGAs
  // Turn Off Config-FPGA
  MAX8688_Power_FPGA(&i2cc, I2CC_FPGA_CFG_ADDR, ON_MARGIN_OFF);
  // we wait for 1.5 second to turn cFPGA back on
  wait(1);
  MAX8688_Power_FPGA(&i2cc, I2CC_FPGA_CFG_ADDR, IMMEDIATE_OFF);
  for (int i = 0; i < 16; i++ ){
	  if (i < 8)
		  // Turn off row 1 and 2
		  MAX8688_Power_FPGA(&i2ca, map_fpgas[i], ON_MARGIN_OFF);
	  else
		  // turn off row 2 and 3
		  MAX8688_Power_FPGA(&i2cb, map_fpgas[i], ON_MARGIN_OFF);
  }
//  CS_LED1 = CS_LED1?0:1;
  // we wait a total of 10 second before we bring up remaining consortium of
  // FPGAs
  wait(10);
  for (int i = 0; i < 16; i++ ){
  	  if (i < 8)
  		  // Turn ON row 1 and 2
  		  MAX8688_Power_FPGA(&i2ca, map_fpgas[i], IMMEDIATE_OFF);
  	  else
  		  // turn ON row 2 and 3
  		  MAX8688_Power_FPGA(&i2cb, map_fpgas[i], IMMEDIATE_OFF);
    }
  CS_LED2 = CS_LED2?0:1;


//  while(1){
//	  print_board_status();
//	  wait(0.5);
//
//  }
  while(1){

	  //COMMENTED FOR FUTURE USE lol!
//	  stdcon.printf("How many FPGAs would you like to deal with?\n\r");
//	  stdcon.printf("\n\rExample:\n\r1\n\r00 01\n\r takes in 1 command and switches off FPGA00\n\r");
//	  stdcon.printf("NOTE: 0 -> Switches Off, 1 -> Switches On, 3 -> resets\n\r");
	  stdcon.printf("Enter the FPGA you wish to reset\r\n");
	  // 	  stdcon.scanf("%d", &count);
 	  stdcon.printf("We will reset %d FPGA\r\n also, if you care there is bit file in data/ to test", count);
	  if (count > 0) {
		  command = (uint8_t *)malloc(count);
		  address = (uint8_t *)malloc(count);

		  for (int i =0; i < count; i++){
//			  stdcon.scanf("%d %X", &command[i], &address[i]);
			  stdcon.scanf("%X", &address[i]);
		  }
		  for (int i = 0; i < count; i++){
			  uint8_t scase = command[i];

			  stdcon.printf("mapped FPAGA%x to Address%X\r\n", address[i], MapToFPGA(address[i]));
			  if (address[i] < 0x12){
				  MAX8688_Power_Cycle(&i2ca, MapToFPGA(address[i]));
			  }
			  else if (address[i] < 0x34){
				  MAX8688_Power_Cycle(&i2cb, MapToFPGA(address[i]));
			  }
			  else if (address[i] == 0x99){
				  stdcon.printf("refreshing all fpgas except cfg\r\n");
				  cycle_fpgas();
			  }
			  wait(1);
//			  print_board_status();

//			  switch(scase){
//			  case 0: MAX8688_Power_FPGA(&i2ca, address[i], ON_MARGIN_OFF);
//			  break;
//			  case 1: MAX8688_Power_FPGA(&i2ca, address[i], IMMEDIATE_OFF);
//			  break;
//			  case 3: MAX8688_Power_FPGA(&i2ca, address[i], ON_MARGIN_OFF);
//			  	  	  wait(0.7);
//			  	  	  MAX8688_Power_FPGA(&i2ca, address[i], IMMEDIATE_OFF);
//			  break;
//			  default: stdcon.printf("Captured Nothing here"); break;
//			  }
		  }
		  free(command);
		  free(address);
	  }
	  print_board_status();

      CS_LED0 = CS_LED0?0:1;
//      CS_LED1 = CS_LED1?0:1;
      CS_LED2 = CS_LED2?0:1;


      wait(0.1);
  }
}










