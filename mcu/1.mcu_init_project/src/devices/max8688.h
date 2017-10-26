/** @file max8688.h
 *  @brief Driver for MAX8688, Digital Power-Supply Controller
 *
 *  Maxim P/N 8688
 *  Digital Power-Supply Controller/Monitor with PMBus Interface
 *
 *  driver
 *
 *  @author JD Brinton (joel@siglabs.com)
 *  @bug No know bugs.
 */

#ifndef MAX8688_H
#define MAX8688_H

#include <cstdint>

#include "mbed.h"

// Refer to max8688
// http://datasheets.maximintegrated.com/en/ds/MAX8688.pdf
#define MAX8688_OPERATION                0x01
#define MAX8688_CLEAR_FAULTS             0x03
#define MAX8688_STORE_DEFAULT_ALL        0x11
#define MAX8688_RESTORE_DEFAULT_ALL      0x12
#define MAX8688_VOUT_COMMAND             0x21
#define MAX8688_VOUT_MARGIN_HIGH         0x25
#define MAX8688_VOUT_MARGIN_LOW          0x26
#define MAX8688_VOUT_TRANSITION_RATE     0x27
#define MAX8688_VOUT_SCALE_LOOP          0x29
#define MAX8688_IOUT_SCALE               0x38
#define MAX8688_VOUT_OV_FAULT_LIMIT      0x40
#define MAX8688_VOUT_OV_WARN_LIMIT       0x42
#define MAX8688_VOUT_UV_WARN_LIMIT       0x43
#define MAX8688_VOUT_UV_FAULT_LIMIT      0x44
#define MAX8688_IOUT_OC_FAULT_LIMIT      0x46
#define MAX8688_IOUT_OC_WARN_LIMIT       0x4A
#define MAX8688_OT_FAULT_LIMIT           0x4F
#define MAX8688_OT_WARN_LIMIT            0x51
#define MAX8688_TON_DELAY                0x60
#define MAX8688_TON_RISE                 0x61
#define MAX8688_TOFF_DELAY               0x64
#define MAX8688_TOFF_FALL                0x65
#define MAX8688_STATUS_BYTE              0x78
#define MAX8688_READ_VOUT                0x8B
#define MAX8688_READ_IOUT                0x8C
#define MAX8688_READ_TEMPERATURE_1       0x8D
#define MAX8688_PMBUS_REVISION           0x98
#define MAX8688_MFR_ID                   0x99
#define MAX8688_MFR_MODEL                0x9A
#define MAX8688_MFR_REVISION             0x9B
#define MAX8688_MFR_SMB_LOOPBACK         0xD0
#define MAX8688_MFR_MODE                 0xD1
#define MAX8688_MFR_VOUT_CORRECTION_RATE 0xD2
#define MAX8688_MFR_SAMPLE_RATE          0xD3
#define MAX8688_MFR_VOUT_PEAK            0xD4
#define MAX8688_MFR_IOUT_PEAK            0xD5
#define MAX8688_MFR_TEMPERATURE_PEAK     0xD6
#define MAX8688_MFR_FILTER_MODE          0xD7
#define MAX8688_MFR_FAULT_STATUS         0xD8
#define MAX8688_MFR_FAULT_RESPONSE       0xD9
#define MAX8688_MFR_FAULT_RETRY          0xDA
#define MAX8688_MFR_SET_ADDRESS          0xDB
#define MAX8688_MFR_IOUT_TEMP_COEFF      0xDC

// Copper Suicide Rev. 1 Addresses

#define I2CA_FPGA00_ADDR 0x00
#define I2CA_FPGA01_ADDR 0x01
#define I2CA_FPGA02_ADDR 0x09
#define I2CA_FPGA03_ADDR 0x12
#define I2CA_FPGA10_ADDR 0x13
#define I2CA_FPGA11_ADDR 0x1B
#define I2CA_FPGA12_ADDR 0x02
#define I2CA_FPGA13_ADDR 0x0B

#define I2CB_FPGA20_ADDR 0x00
#define I2CB_FPGA21_ADDR 0x01
#define I2CB_FPGA22_ADDR 0x09
#define I2CB_FPGA23_ADDR 0x12
#define I2CB_FPGA30_ADDR 0x13
#define I2CB_FPGA31_ADDR 0x1B
#define I2CB_FPGA32_ADDR 0x02
#define I2CB_FPGA33_ADDR 0x0B
#define I2CC_FPGA_CFG_ADDR 0x00

#define CS_SENSE_RESISTOR 0x04

// OPERATION command bytes
#define IMMEDIATE_OFF  0x00
#define SOFT_OFF       0x40
#define ON_MARGIN_OFF  0x80
#define ON_MARGIN_LOW  0x98
#define ON_MARGIN_HIGH 0xA8

static int map_fpgas[16] = {
	I2CA_FPGA00_ADDR,
	I2CA_FPGA01_ADDR,
	I2CA_FPGA02_ADDR,
	I2CA_FPGA03_ADDR,
	I2CA_FPGA10_ADDR,
	I2CA_FPGA11_ADDR,
	I2CA_FPGA12_ADDR,
	I2CA_FPGA13_ADDR,
	I2CB_FPGA20_ADDR,
	I2CB_FPGA21_ADDR,
	I2CB_FPGA22_ADDR,
	I2CB_FPGA23_ADDR,
	I2CB_FPGA30_ADDR,
	I2CB_FPGA31_ADDR,
	I2CB_FPGA32_ADDR,
	I2CB_FPGA33_ADDR
	};


/*
 * This function maps the FPGA to appropriate address
 * range: 00 ..... 03
 * 		  10 ..... 13
 * 		  ........
 * 		  ........
 * 		  ........ 33
 */
int MapToFPGA(int in);

uint16_t MAX8688_Read_Reg(I2C* i2c, uint8_t addr, uint8_t cmd);

void MAX8688_Write_Reg(I2C* i2c, uint8_t addr, uint8_t* value, uint8_t len);

double MAX8688_Get_Temperature(I2C* i2c, uint8_t addr);

double MAX8688_Get_Voltage(I2C* i2c, uint8_t addr);

double MAX8688_Get_Current(I2C* i2c, uint8_t addr);

void MAX8688_Init(I2C* i2c, uint8_t addr);

/*
 * This function powers the appropriate board.
 * @param(i2c): pointer to i2c object
 * @param(addr): address of the module to switch
 * @param(operation): SEE OPERATION MACROS ABOVE
 */
void MAX8688_Power_FPGA(I2C* i2c, uint8_t addr, uint8_t operation );

/*
 * This function powers cycles the said FPGAS.
 * @param(i2c): pointer to i2c object
 * @param(addr): address of the module/FPGA to switch
 */
void MAX8688_Power_Cycle(I2C* i2c, uint8_t addr);

/*
 * This funstion set the VOut impedance appropriate board.
 * @param(i2c): pointer to i2c object
 * @param(addr): address of the module to switch
 * @param(value): default is 0(range 0-2.048 & 0-5.5 when input range is 0-5.5)
 */
void MAX8688_Set_VOut(I2C* i2c, uint8_t addr, uint8_t value);


/*
 * This funstion powers the appropriate board.
 * @param(i2c): pointer to i2c object
 * @param(addr): address of the module to switch
 * @param(value): default is csr (for devices with cs_sense_resistor its
 *                              the sense resistor.)
 */
void MAX8688_Set_Current(I2C* i2c, uint8_t addr, uint8_t value);

#endif
