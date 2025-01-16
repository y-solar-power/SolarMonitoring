#include <pico/stdlib.h>
#include "hardware/i2c.h"
#include "SHT31.h"

SHT31::SHT31(uint8_t i2caddr) {
  _i2caddr = i2caddr;
}

void SHT31::i2c_write(uint16_t data)
{
  uint8_t bytes[] = {(uint8_t)((data & 0xFF00) >> 8), (uint8_t)(data & 0xFF)};
  i2c_write_blocking(i2c0, _i2caddr, bytes, 2, false);
}
void SHT31::i2c_read(uint8_t data[],uint8_t num)
{
  i2c_read_blocking(i2c0, _i2caddr, data, num, false);
}

void SHT31::BussReset(void)
{
  i2c_write(0x0006);
}

void SHT31::SoftReset(void)
{
  i2c_write(0x30A2);
  sleep_ms(500);
  i2c_write(0x3041);
  sleep_ms(500);
}

void SHT31::Heater(uint8_t onoff)
{
  if(onoff==1)i2c_write(0x306D);
  else i2c_write(0x3066);
  sleep_ms(500);
}


uint16_t SHT31::ReadStatus(void)
{
  uint8_t data[3];
  i2c_write(0xF32D);
  i2c_read(data,3);
  return (data[0]<<8|data[1]);
}

void SHT31::GetTempHum(void)
{
  uint8_t data[7];
    
  i2c_write(0x2400);
  sleep_ms(300);
  i2c_read(data,6);
  temperature = -45.0 + (175.0 * ((data[0] * 256.0) + data[1]) / 65535.0);
  humidity = (100.0 * ((data[3] * 256.0) + data[4])) / 65535.0;
}

float SHT31::Temperature(void)
{
 return temperature;
}

float SHT31::Humidity(void)
{
 return humidity;
}
