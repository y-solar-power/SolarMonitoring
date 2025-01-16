#include <pico/stdlib.h>
#include "hardware/i2c.h"
#include "BNO055.h"


BNO055::BNO055(uint8_t address){
  _i2caddr = address;
}

void BNO055::i2c_write(uint8_t *data,uint8_t num)
{
  i2c_write_blocking(i2c0, _i2caddr, data, num, false);
}
void BNO055::i2c_read(uint8_t data[],uint8_t num)
{
  i2c_read_blocking(i2c0, _i2caddr, data, num, false);
}

uint8_t BNO055::i2c_read_8(uint8_t address){
  uint8_t val = 0x00;
  uint8_t write[] = {address};
  i2c_write(write , 1);
  i2c_read(&val, 1);
  return val;
}

uint16_t BNO055::i2c_read_16(uint8_t address){
  uint8_t LSB = i2c_read_8(address);
  uint8_t MSB = i2c_read_8(address + 1);
  uint16_t val = (uint16_t)(MSB << 8) | (uint16_t)LSB;
  return val;
}


uint8_t BNO055::GetBNO055ChipId(){
  return i2c_read_8(0x00);
}

uint8_t BNO055::GetAccChipId(){
  return i2c_read_8(0x01);
}

uint8_t BNO055::GetMagChipId(){
  return i2c_read_8(0x02);
}

uint8_t BNO055::GetGyroChipId(){
  return i2c_read_8(0x03);
}

uint16_t BNO055::GetSwRevId(){
  return i2c_read_16(0x04);
}

uint8_t BNO055::GetBlRevId(){
  return i2c_read_8(0x06);
}

uint8_t BNO055::GetPageId(){
  return i2c_read_8(0x07);
}

void BNO055::SetPageId(uint8_t val){
  uint8_t data[] = {0x07, val};
  return i2c_write(data, 1);
}

int16_t BNO055::GetAccelerationX(){
  return (int16_t)i2c_read_16(0x08);
}

int16_t BNO055::GetAccelerationY(){
  return (int16_t)i2c_read_16(0x0A);
}

int16_t BNO055::GetAccelerationZ(){
  return (int16_t)i2c_read_16(0x0C);
}

int16_t BNO055::GetMagX(){
  return i2c_read_16(0x0E);
}

int16_t BNO055::GetMagY(){
  return i2c_read_16(0x10);
}

int16_t BNO055::GetMagZ(){
  return i2c_read_16(0x12);
}

int16_t BNO055::GetGyroX(){
  return i2c_read_16(0x14);
}

int16_t BNO055::GetGyroY(){
  return i2c_read_16(0x16);
}

int16_t BNO055::GetGyroZ(){
  return i2c_read_16(0x18);
}

int16_t BNO055::GetEulDataX(){
  return i2c_read_16(0x1A);
}

int16_t BNO055::GetEulDataY(){
  return i2c_read_16(0x1C);
}

int16_t BNO055::GetEulDataZ(){
  return i2c_read_16(0x1E);
}

int16_t BNO055::GetQuaDataW(){
  return i2c_read_16(0x20);
}

int16_t BNO055::GetQuaDataX(){
  return i2c_read_16(0x22);
}

int16_t BNO055::GetQuaDataY(){
  return i2c_read_16(0x24);
}

int16_t BNO055::GetQuaDataZ(){
  return i2c_read_16(0x26);
}

int16_t BNO055::GetLiaDataX(){
  return i2c_read_16(0x28);
}

int16_t BNO055::GetLiaDataY(){
  return i2c_read_16(0x2A);
}

int16_t BNO055::GetLiaDataZ(){
  return i2c_read_16(0x2C);
}

int16_t BNO055::GetGrvDataX(){
  return i2c_read_16(0x2E);
}

int16_t BNO055::GetGrvDataY(){
  return i2c_read_16(0x30);
}

int16_t BNO055::GetGrvDataZ(){
  return i2c_read_16(0x32);
}

uint8_t BNO055::GetTemp(){
  return i2c_read_8(0x34);
}

uint8_t BNO055::GetCalibStat(){
  return i2c_read_8(0x35);
}

uint8_t BNO055::GetStResult(){
  return i2c_read_8(0x36);
}

uint8_t BNO055::GetInaSta(){
  return i2c_read_8(0x37);
}

uint8_t BNO055::GetSysClkStatus(){
  return i2c_read_8(0x38);
}

uint8_t BNO055::GetSysStatus(){
  return i2c_read_8(0x39);
}

uint8_t BNO055::GetSysErr(){
  return i2c_read_8(0x3A);
}

uint8_t BNO055::GetUnitSel(){
  return i2c_read_8(0x3B);
}

void BNO055::SetUnitSel(uint8_t val){
  uint8_t data[] = {0x3B, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetOprMode(){
  return i2c_read_8(0x3D);
}

void BNO055::SetOprMode(uint8_t val){
  uint8_t data[] = {0x3D, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetPwrMode(){
  return i2c_read_8(0x3E);
}

void BNO055::SetPwrMode(uint8_t val){
  uint8_t data[] = {0x3E, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetSysTrigger(){
  return i2c_read_8(0x3F);
}

void BNO055::SetSysTrigger(uint8_t val){
  uint8_t data[] = {0x3F, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetTempSource(){
  return i2c_read_8(0x40); 
} 

void BNO055::SetTempSource(uint8_t val){
  uint8_t data[] = {0x40, val};
  return i2c_write(data, 2); 
}

uint8_t BNO055::GetAxisMapConfig(){
  return i2c_read_8(0x41);
}

void BNO055::SetAxisMapConfig(uint8_t val){
  uint8_t data[] = {0x41, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAxisMapSign(){
  return i2c_read_8(0x42);
}

void BNO055::SetAxisMapSign(uint8_t val){
  uint8_t data[] = {0x42, val};
  return i2c_write(data, 2);
}

uint16_t BNO055::GetSicMatrix0(){
  return i2c_read_16(67);
}

void BNO055::SetSicMatrix0(uint16_t val){
  uint8_t data[] = {67, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix1(){
  return i2c_read_16(69);
}

void BNO055::SetSicMatrix1(uint16_t val){
  uint8_t data[] = {69, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix2(){
  return i2c_read_16(71);
}

void BNO055::SetSicMatrix2(uint16_t val){
  uint8_t data[] = {71, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix3(){
  return i2c_read_16(73);
}

void BNO055::SetSicMatrix3(uint16_t val){
  uint8_t data[] = {73, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix4(){
  return i2c_read_16(75);
}

void BNO055::SetSicMatrix4(uint16_t val){
  uint8_t data[] = {75, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix5(){
  return i2c_read_16(77);
}

void BNO055::SetSicMatrix5(uint16_t val){
  uint8_t data[] = {77, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix6(){
  return i2c_read_16(79);
}

void BNO055::SetSicMatrix6(uint16_t val){
  uint8_t data[] = {79, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix7(){
  return i2c_read_16(81);
}

void BNO055::SetSicMatrix7(uint16_t val){
  uint8_t data[] = {81, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetSicMatrix8(){
  return i2c_read_16(83);
}

void BNO055::SetSicMatrix8(uint16_t val){
  uint8_t data[] = {83, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetAccOffsetX(){
  return i2c_read_16(0x55);
}

void BNO055::SetAccOffsetX(uint16_t val){
  uint8_t data[] = {0x55, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetAccOffsetY(){
  return i2c_read_16(0x57);
}

void BNO055::SetAccOffsetY(uint16_t val){
  uint8_t data[] = {0x57, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetAccOffsetZ(){
  return i2c_read_16(0x59);
}

void BNO055::SetAccOffsetZ(uint16_t val){
  uint8_t data[] = {0x59, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetMagOffsetX(){
  return i2c_read_16(0x5B);
}

void BNO055::SetMagOffsetX(uint16_t val){
  uint8_t data[] = {0x5B, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetMagOffsetY(){
  return i2c_read_16(0x5D);
}

void BNO055::SetMagOffsetY(uint16_t val){
  uint8_t data[] = {0x5D, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetMagOffsetZ(){
  return i2c_read_16(0x5F);
}

void BNO055::SetMagOffsetZ(uint16_t val){
  uint8_t data[] = {0x5F, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetGyrOffsetX(){
  return i2c_read_16(0x61);
}

void BNO055::SetGyrOffsetX(uint16_t val){
  uint8_t data[] = {0x61, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetGyrOffsetY(){
  return i2c_read_16(0x63);
}

void BNO055::SetGyrOffsetY(uint16_t val){
  uint8_t data[] = {0x63, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetGyrOffsetZ(){
  return i2c_read_16(0x65);
}

void BNO055::SetGyrOffsetZ(uint16_t val){
  uint8_t data[] = {0x65, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetAccRadius(){
  return i2c_read_16(0x67);
}

void BNO055::SetAccRadius(uint16_t val){
  uint8_t data[] = {0x67, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint16_t BNO055::GetMagRadius(){
  return i2c_read_16(0x69);
}

void BNO055::SetMagRadius(uint16_t val){
  uint8_t data[] = {0x69, (uint8_t)((val >> 8) & 0xFF), (uint8_t)(val & 0xFF)};
  return i2c_write(data, 3);
}

uint8_t BNO055::GetAccConfig(){
  return i2c_read_8(0x08);
}

void BNO055::SetAccConfig(uint8_t val){
  uint8_t data[] = {0x08, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetMagConfig(){
  return i2c_read_8(0x09);
}

void BNO055::SetMagConfig(uint8_t val){
  uint8_t data[] = {0x09, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrConfig0(){
  return i2c_read_8(0x0A);
}

void BNO055::SetGyrConfig0(uint8_t val){
  uint8_t data[] = {0x0A, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrConfig1(){
  return i2c_read_8(0x0B);
}

void BNO055::SetGyrConfig1(uint8_t val){
  uint8_t data[] = {0x0B, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccSleepConfig(){
  return i2c_read_8(0x0C);
}

void BNO055::SetAccSleepConfig(uint8_t val){
  uint8_t data[] = {0x0C, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrSleepConfig(){
  return i2c_read_8(0x0D);
}

void BNO055::SetGyrSleepConfig(uint8_t val){
  uint8_t data[] = {0x0D, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetIntMsk(){
  return i2c_read_8(0x0F);
}

void BNO055::SetIntMsk(uint8_t val){
  uint8_t data[] = {0x0F, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetIntEn(){
  return i2c_read_8(0x10);
}

void BNO055::SetIntEn(uint8_t val){
  uint8_t data[] = {0x10, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccAmThres(){
  return i2c_read_8(0x11);
}

void BNO055::SetAccAmThres(uint8_t val){
  uint8_t data[] = {0x11, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccIntSettings(){
  return i2c_read_8(0x12);
}

void BNO055::SetAccIntSettings(uint8_t val){
  uint8_t data[] = {0x12, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccHgDuration(){
  return i2c_read_8(0x13);
}

void BNO055::SetAccHgDuration(uint8_t val){
  uint8_t data[] = {0x13, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccHgThres(){
  return i2c_read_8(0x14);
}

void BNO055::SetAccHgThres(uint8_t val){
  uint8_t data[] = {0x14, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccNmThres(){
  return i2c_read_8(0x15);
}

void BNO055::SetAccNmThres(uint8_t val){
  uint8_t data[] = {0x15, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetAccNmSet(){
  return i2c_read_8(0x16);
}

void BNO055::SetAccNmSet(uint8_t val){
  uint8_t data[] = {0x16, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrIntSetting(){
  return i2c_read_8(0x17);
}

void BNO055::SetGyrIntSetting(uint8_t val){
  uint8_t data[] = {0x17, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrHrXSet(){
  return i2c_read_8(0x18);
}

void BNO055::SetGyrHrXSet(uint8_t val){
  uint8_t data[] = {0x18, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrDurX(){
  return i2c_read_8(0x19);
}

void BNO055::SetGyrDurX(uint8_t val){
  uint8_t data[] = {0x19, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrHrYSet(){
  return i2c_read_8(0x1A);
}

void BNO055::SetGyrHrYSet(uint8_t val){
  uint8_t data[] = {0x1A, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrDurY(){
  return i2c_read_8(0x1B);
}

void BNO055::SetGyrDurY(uint8_t val){
  uint8_t data[] = {0x1B, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrHrZSet(){
  return i2c_read_8(0x1c);
}

void BNO055::SetGyrHrZSet(uint8_t val){
  uint8_t data[] = {0x1c, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrDurZ(){
  return i2c_read_8(0x1d);
}

void BNO055::SetGyrDurZ(uint8_t val){
  uint8_t data[] = {0x1d, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrAmThres(){
  return i2c_read_8(0x1E);
}

void BNO055::SetGyrAmThres(uint8_t val){
  uint8_t data[] = {0x1E, val};
  return i2c_write(data, 2);
}

uint8_t BNO055::GetGyrAmSet(){
  return i2c_read_8(0x1F);
}

void BNO055::SetGyrAmSet(uint8_t val){
  uint8_t data[] = {0x1F, val};
  return i2c_write(data, 2);
}