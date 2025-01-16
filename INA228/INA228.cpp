#include <pico/stdlib.h>
#include "hardware/i2c.h"
#include "INA228.h"

INA228::INA228(uint8_t i2caddr) {
  _i2caddr = i2caddr;
}

void INA228::i2c_write_24(char reg, int32_t data)
{
  uint8_t bytes[] = {reg, (uint8_t)((data & 0xFF0000) >> 16), (uint8_t)((data & 0xFF00) >> 8), (uint8_t)(data & 0xFF)};
  i2c_write_blocking(i2c0, _i2caddr, bytes, 4, false);
}
void INA228::i2c_write_16(char reg, uint16_t data)
{
  uint8_t bytes[] = {reg, (uint8_t)((data & 0xFF00) >> 8), (uint8_t)(data & 0xFF)};
  i2c_write_blocking(i2c0, _i2caddr, bytes, 3, false);
}
void INA228::i2c_write_8(char reg, uint8_t data)
{
  uint8_t bytes[] = {reg, data};
  i2c_write_blocking(i2c0, _i2caddr, bytes, 2, false);
}
void INA228::i2c_write_reg(char reg)
{
  uint8_t bytes[] = {reg};
  i2c_write_blocking(i2c0, _i2caddr, bytes, 1, false);
}

void INA228::i2c_read(uint8_t data[],uint8_t num)
{
  i2c_read_blocking(i2c0, _i2caddr, data, num, false);
}

int64_t INA228::i2c_read_40(){
    uint8_t buff[5];
    i2c_read(buff, 5);
    int64_t val = ((int64_t)buff[0] << 32) | ((int64_t)buff[1] << 24) | ((int64_t)buff[2] << 16) | ((int64_t)buff[3] << 8) | ((int64_t)buff[4] << 0);
    return val;
}

int32_t INA228::i2c_read_24(){
    uint8_t buff[3];
    i2c_read(buff, 3);
    int32_t val = ((int32_t)buff[0] << 16) | ((int32_t)buff[1] << 8) | (int32_t)buff[2];
    return val;
}

uint16_t INA228::i2c_read_16(){
    uint8_t buff[2];
    i2c_read(buff, 2);
    uint16_t val = ((uint16_t)buff[0] << 8) | (uint16_t)buff[1];
    return val;
}

/// @brief 
/// @param RST Reset Bit. Setting this bit to '1' generates a system reset that is the
/// @param RSTACC Resets the contents of accumulation registers ENERGY and CHARGE to 0
/// @param CONVDLY Sets the Delay for initial ADC conversion in steps of 2 ms.
/// @param TEMPCOMP Enables temperature compensation of an external shunt
/// @param ADCRANGE Shunt full scale range selection across IN+ and IN–.
void INA228::SetConfiguration(bool RST, bool RSTACC, char CONVDLY, bool TEMPCOMP, bool ADCRANGE){
    uint16_t configuration = RST << 15 | RSTACC << 14 | CONVDLY << 6 | TEMPCOMP << 5 | ADCRANGE << 4;
    i2c_write_16(REG_CONFIGURATION, configuration);
}

uint16_t INA228::GetConfiguration(uint8_t *buff){
    i2c_write_reg(REG_CONFIGURATION);
    return i2c_read_16();
}

/// @brief 
/// @param MODE The user can set the MODE bits for continuous or triggered mode on bus voltage, shunt voltage or temperature measurement.
/// @param VBUSCT Sets the conversion time of the bus voltage measurement:
/// @param VSHCT Sets the conversion time of the shunt voltage measurement:
/// @param VTCT Sets the conversion time of the temperature measurement:
/// @param AVG Selects ADC sample averaging count. The averaging setting applies to all active inputs.
void INA228::SetAdcConfiguration(char MODE, char VBUSCT, char VSHCT, char VTCT, char AVG){
    uint16_t configuration = 0x00;
    configuration |= (MODE & 0b1111) << 12;
    configuration |= (VBUSCT & 0b111) << 9;
    configuration |= (VSHCT & 0b111) << 6;
    configuration |= (VTCT & 0b111) << 3;
    configuration |= (AVG & 0b111) << 0;
    i2c_write_16(REG_ADC_CONF, configuration);
}

uint16_t INA228::GetAdcConfiguration(uint8_t *buff){
    i2c_write_reg(REG_ADC_CONF);
    return i2c_read_16();
}

/// @brief The register provides the device with a conversion constant value that represents shunt resistance used to calculate current value in Amperes.
/// @param CURRLAB 
void INA228::SetShuntCalibration(uint16_t CURRLAB){
    uint16_t configuration = 0x00;
    configuration |= (CURRLAB & 0x7FFF) << 0;
    i2c_write_16(REG_SHUNT_CAL, configuration);
}

uint16_t INA228::GetShuntCalibration(){
    i2c_write_reg(REG_SHUNT_CAL);
    return i2c_read_16();
}

/// @brief Temperature coefficient of the shunt for temperature compensation correction. 
/// @param TEMPCO 
void INA228::SetShutTemperatureCoefficient(uint16_t TEMPCO){
    uint16_t configuration = 0x00;
    configuration |= (TEMPCO & 0x3FFF) << 0;
    i2c_write_16(REG_SHUNT_TEMPCO, configuration);
}

uint16_t INA228::GetShuntTemperatureCoefficient(){
    i2c_write_reg(REG_SHUNT_TEMPCO);
    return i2c_read_16();
}

int32_t INA228::GetVShunt(){
    i2c_write_reg(REG_VSHUNT);
    return i2c_read_24() >> 4;
}

int32_t INA228::GetVBus(){
    i2c_write_reg(REG_VBUS);
    return i2c_read_24() >> 4;
}

uint16_t INA228::GetDieTemp(){
    i2c_write_reg(REG_DIETEMP);
    return i2c_read_16();
}

int32_t INA228::GetCurrentResult(){
    i2c_write_reg(REG_CURRENT);
    int32_t res = i2c_read_24() >> 4;
    if(((uint32_t)res & (uint32_t)0x80000) > 0) {
        res = (~res) & 0xFFFFF;
        res += 1;
        res = -res;
    }
    return res;
}

int32_t INA228::GetPower(){
    i2c_write_reg(REG_POWER);
    return i2c_read_24() >> 0;
}

int64_t INA228::GetEnergy(){
    i2c_write_reg(REG_ENERGY);
    return i2c_read_40() >> 0;
}

int64_t INA228::GetCharge(){
    i2c_write_reg(REG_CHARGE);
    return i2c_read_40() >> 0;
}

void INA228::SetDiagnosticFlagsAndAlert(bool ALATCH, bool CNVR, bool SLOWALERT, bool APOL
,bool TMPOL, bool SHNTOL, bool SHNTUL, bool BUSOL, bool BUSUL, bool POL, bool CNVRF, bool MEMSTAT){
     uint16_t configuration = 0x00;
    configuration |= ALATCH << 15;
    configuration |= CNVR << 14;
    configuration |= SLOWALERT << 13;
    configuration |= APOL << 12;
    configuration |= TMPOL << 7;
    configuration |= SHNTOL << 6;
    configuration |= SHNTUL << 5;
    configuration |= BUSOL << 4;
    configuration |= BUSUL << 3;
    configuration |= POL << 2;
    configuration |= CNVRF << 1;
    configuration |= MEMSTAT << 0;
    i2c_write_16(REG_DIAG_ALRT, configuration);
}

uint16_t INA228::GetDiagnosticFlagsAndAlert(){
    i2c_write_reg(REG_DIAG_ALRT);
    return i2c_read_16();
}

void INA228::SetShuntOvervoltageThreshold(uint16_t SOVL){
    uint16_t configuration = 0x00;
    configuration |= SOVL;
    i2c_write_16(REG_SOVL, configuration);
}

uint16_t INA228::GetShuntOvervoltageThreshold(){
    i2c_write_reg(REG_SOVL);
    return i2c_read_16();
}

void INA228::SetShuntUndervoltageThreshold(uint16_t SUVL){
    uint16_t configuration = 0x00;
    configuration |= SUVL;
    i2c_write_16(REG_SUVL, configuration);
}

uint16_t INA228::GetShuntUndervoltageThreshold(){
    i2c_write_reg(REG_SUVL);
    return i2c_read_16();
}

void INA228::SetBusOvervoltageThreshold(uint16_t BOVL){
    uint16_t configuration = 0x00;
    configuration |= BOVL;
    i2c_write_16(REG_BOVL, configuration);
}

uint16_t INA228::GetBusOvervoltageThreshold(){
    i2c_write_reg(REG_BOVL);
    return i2c_read_16();
}

void INA228::SetBusUndervoltageThreshold(uint16_t BUVL){
    uint16_t configuration = 0x00;
    configuration |= BUVL;
    i2c_write_16(REG_BUVL, configuration);
}

uint16_t INA228::GetBusUndervoltageThreshold(){
    i2c_write_reg(REG_BUVL);
    return i2c_read_16();
}

void INA228::SetTemperatureOverLimitThreshold(uint16_t TOL){
    uint16_t configuration = 0x00;
    configuration |= TOL;
    i2c_write_16(REG_TEMP_LIMIT, configuration);
}

uint16_t INA228::GetTemperatureOverLimitThreshold(){
    i2c_write_reg(REG_TEMP_LIMIT);
    return i2c_read_16();
}

void INA228::SetPowerOverLimitThreshold(uint16_t POL){
    uint16_t configuration = 0x00;
    configuration |= POL;
    i2c_write_16(REG_PWR_LIMIT, configuration);
}

uint16_t INA228::GetPowerOverLimitThreshold(){
    i2c_write_reg(REG_PWR_LIMIT);
    return i2c_read_16();
}

uint16_t INA228::GetManufacturerId(){
    i2c_write_reg(REG_MANUFACTURER_ID);
    return i2c_read_16();
}

uint16_t INA228::GetDeviceId(){
    i2c_write_reg(REG_DEVICE_ID);
    return i2c_read_16();
}