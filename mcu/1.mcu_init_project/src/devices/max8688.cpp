/** @file max8688.cpp
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

#include "max8688.h"
#include "math.h"


int MapToFPGA(int in)
{
	int a = in & 0xf;
	int b = (in & 0xf0) >> 4;
	int idx;
	idx = a + b*4;
	if (idx < 16)
		return map_fpgas[idx];
	else
		return -1;
}

uint16_t MAX8688_Read_Reg(I2C* i2c, uint8_t addr, uint8_t cmd)
{
  uint8_t ret[2] = {0x00, 0x00};
  uint16_t ret_r;



  i2c->write(addr<<1, (const char*)&cmd, 1, 1);
  i2c->read(addr<<1, (char*)ret, 2, 0);



  ret_r = (ret[0]) + (ret[1]<<8);

  return ret_r;
}

void MAX8688_Write_Reg(I2C* i2c, uint8_t addr, uint8_t* value, uint8_t len)
{
  i2c->write(addr<<1, (const char*)value, len, 0);
}

double MAX8688_Get_Temperature(I2C* i2c, uint8_t addr)
{
  uint16_t y;
  double x;

  y = MAX8688_Read_Reg(i2c, addr, MAX8688_READ_TEMPERATURE_1);

  x = -(((double)((int16_t)y)) * 1000.0 - 335.0) / 7612.0;

  return x;
}

double MAX8688_Get_Voltage(I2C* i2c, uint8_t addr)
{
  uint16_t y;
  double x;

  y = MAX8688_Read_Reg(i2c, addr, MAX8688_READ_VOUT);

  x = (((double)y) * 10) / 19995.0;

  return x;
}

double MAX8688_Get_Current(I2C* i2c, uint8_t addr)
{
  uint16_t y;
  double x;

  y = MAX8688_Read_Reg(i2c, addr, MAX8688_READ_IOUT);

  x = (((double)y) * 100.0) / 23109.0;

  return x;
}

void MAX8688_Power_FPGA(I2C* i2c, uint8_t addr, uint8_t operation) {
    uint8_t cmd[2] = {MAX8688_OPERATION, operation};

    MAX8688_Write_Reg(i2c, addr, cmd, 2);
}

void MAX8688_Power_Cycle(I2C* i2c, uint8_t addr){

	  MAX8688_Power_FPGA(i2c, addr, ON_MARGIN_OFF);
	  wait(0.01);
	  MAX8688_Power_FPGA(i2c, addr, IMMEDIATE_OFF);
}

void MAX8688_Set_VOut(I2C* i2c, uint8_t addr, float value=0.0){
	uint8_t len = 3;
    uint8_t cmd[3];
	uint16_t v_send = floor((19995*value+0)/10);
    cmd[0] = (uint8_t)MAX8688_VOUT_COMMAND;
    // check for endianess
    cmd[1] = v_send & 0xFF;
    cmd[2] = v_send >> 8;
    // To take effect we need to switch it off and switch it back ON! 
    MAX8688_Power_FPGA(i2c, addr, ON_MARGIN_OFF);
    


    MAX8688_Write_Reg(i2c, addr, cmd, len);
    
    MAX8688_Power_FPGA(i2c, addr, IMMEDIATE_OFF);
}

void MAX8688_Set_Current(I2C* i2c, uint8_t addr, 
                         uint8_t value=CS_SENSE_RESISTOR){
	uint8_t len = 3;
	uint8_t cmd[3];
	uint16_t v_send = floor((1*value+0)*10);
    cmd[0] = (uint8_t)MAX8688_IOUT_SCALE;
    // check for endianess
    cmd[1] = v_send & 0xFF;
    cmd[2] = v_send >> 8;
    // To take effect we need to switch it off and switch it back ON! 
    MAX8688_Power_FPGA(i2c, addr, ON_MARGIN_OFF);
    
    MAX8688_Write_Reg(i2c, addr, cmd, len);
    
    MAX8688_Power_FPGA(i2c, addr, IMMEDIATE_OFF);
}

void MAX8688_Init(I2C* i2c, uint8_t addr)
{
  uint8_t cmd[3] = {0x00, 0x00};

  cmd[0] = (uint8_t)MAX8688_IOUT_SCALE;
  cmd[2] = 0x00;
  cmd[1] = (uint8_t)(CS_SENSE_RESISTOR * 10); // mOhm

  MAX8688_Write_Reg(i2c, addr, (uint8_t*)cmd, 3);
}

