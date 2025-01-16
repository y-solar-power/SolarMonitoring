#include <pico/stdlib.h>
#include <string.h>
#include "hardware/i2c.h"
#include "LPS25HB.h"


/// @brief LPS25HB WRITE
/// @param data 
/// @param len 
/// @return 
int LPS25HB::i2c_write(unsigned char *data, unsigned int len){
     return i2c_write_blocking(i2c0, LPS25HB_ADDR, (const uint8_t *)data, len, false);
}

/// @brief データの読み書き対象のアドレスの設定
/// @param address 
/// @return 
int LPS25HB::i2c_write_single(unsigned char value){
     unsigned char data[1] = {value};
     return i2c_write_blocking(i2c0, LPS25HB_ADDR, (const uint8_t *)data, 1, false);
}

/// @brief LPS25HB READ
/// @param buff 
/// @param len 
/// @return 
int LPS25HB::i2c_read(unsigned char *buff, unsigned int len){
     return i2c_read_blocking(i2c0, LPS25HB_ADDR, (uint8_t *)buff, len, false);
}

LPS25HB::LPS25HB(){}

void LPS25HB::SetToNormalMode(){
     unsigned char buff[2] = {LPS25HB_REG_CTRL_REG1, 0x90};
     LPS25HB::i2c_write(buff,2);
}

double LPS25HB::GetTemp(){
     unsigned char data[2];
     for(int i = 0; i < 2; i++){
          LPS25HB::i2c_write_single(LPS25HB_REG_TEMP_OUT_L + i);
          LPS25HB::i2c_read(data + i, 1);
     }
     int16_t raw = (((uint16_t)data[1]) << 8) | ((uint16_t)data[0]);
     double temp = raw / 480.0 + 42.5;
     return temp;
}

double LPS25HB::GetPressure(){
     unsigned char data[3];
     LPS25HB::i2c_write_single(LPS25HB_REG_PRESS_OUT_XL | 0x80);
     LPS25HB::i2c_read(data, 3);
     int32_t raw = ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];
     if((raw & 0x800000) > 0) raw = ~raw - 1;
     double pressure = raw / 4096.0;
     return pressure;
}

char LPS25HB::WhoAmI(){
     unsigned char data[1];
     LPS25HB::i2c_write_single(LPS25HB_REG_WHO_AM_I);
     LPS25HB::i2c_read(data, 1);
     return data[0];
}