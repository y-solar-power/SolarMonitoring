#ifndef _INA228_H_
#define _INA228_H_


#define REG_CONFIGURATION 0x00
#define REG_ADC_CONF 0x01
#define REG_SHUNT_CAL 0x02
#define REG_SHUNT_TEMPCO 0x03
#define REG_VSHUNT 0x04
#define REG_VBUS 0x05
#define REG_DIETEMP 0x06
#define REG_CURRENT 0x07
#define REG_POWER 0x08
#define REG_ENERGY 0x09
#define REG_CHARGE 0x0A
#define REG_DIAG_ALRT 0x0B
#define REG_SOVL 0x0C
#define REG_SUVL 0x0D
#define REG_BOVL 0x0E
#define REG_BUVL 0x0F
#define REG_TEMP_LIMIT 0x10
#define REG_PWR_LIMIT 0x11
#define REG_MANUFACTURER_ID 0x3E
#define REG_DEVICE_ID 0x3F

class INA228{
  public:
    INA228(uint8_t i2caddr);
    void i2c_write_24(char reg, int32_t data);
    void i2c_write_16(char reg, uint16_t data);
    void i2c_write_8(char reg, uint8_t data);
    void i2c_write_reg(char reg);
    void i2c_read(uint8_t data[],uint8_t num);
    int64_t i2c_read_40();
    int32_t i2c_read_24();
    uint16_t i2c_read_16();

    /// @brief 
    /// @param RST Reset Bit. Setting this bit to '1' generates a system reset that is the
    /// @param RSTACC Resets the contents of accumulation registers ENERGY and CHARGE to 0
    /// @param CONVDLY Sets the Delay for initial ADC conversion in steps of 2 ms.
    /// @param TEMPCOMP Enables temperature compensation of an external shunt
    /// @param ADCRANGE Shunt full scale range selection across IN+ and IN–.
    void SetConfiguration(bool RST, bool RSTACC, char CONVDLY, bool TEMPCOMP, bool ADCRANGE);
    uint16_t GetConfiguration(uint8_t *buff);
    /// @brief 
    /// @param MODE The user can set the MODE bits for continuous or triggered mode on bus voltage, shunt voltage or temperature measurement.
    /// @param VBUSCT Sets the conversion time of the bus voltage measurement:
    /// @param VSHCT Sets the conversion time of the shunt voltage measurement:
    /// @param VTCT Sets the conversion time of the temperature measurement:
    /// @param AVG Selects ADC sample averaging count. The averaging setting applies to all active inputs.
    void SetAdcConfiguration(char MODE, char VBUSCT, char VSHCT, char VTCT, char AVG);
    uint16_t GetAdcConfiguration(uint8_t *buff);
    /// @brief The register provides the device with a conversion constant value that represents shunt resistance used to calculate current value in Amperes.
    /// @param CURRLAB 
    void SetShuntCalibration(uint16_t CURRLAB);
    uint16_t GetShuntCalibration();
    /// @brief Temperature coefficient of the shunt for temperature compensation correction. 
    /// @param TEMPCO 
    void SetShutTemperatureCoefficient(uint16_t TEMPCO);
    uint16_t GetShuntTemperatureCoefficient();
    int32_t GetVShunt();
    int32_t GetVBus();
    uint16_t GetDieTemp();
    int32_t GetCurrentResult();
    int32_t GetPower();
    int64_t GetEnergy();
    int64_t GetCharge();
    void SetDiagnosticFlagsAndAlert(bool ALATCH, bool CNVR, bool SLOWALERT, bool APOL ,bool TMPOL, bool SHNTOL, bool SHNTUL, bool BUSOL, bool BUSUL, bool POL, bool CNVRF, bool MEMSTAT);
    uint16_t GetDiagnosticFlagsAndAlert();
    void SetShuntOvervoltageThreshold(uint16_t SOVL);
    uint16_t GetShuntOvervoltageThreshold();
    void SetShuntUndervoltageThreshold(uint16_t SUVL);
    uint16_t GetShuntUndervoltageThreshold();
    void SetBusOvervoltageThreshold(uint16_t BOVL);
    uint16_t GetBusOvervoltageThreshold();
    void SetBusUndervoltageThreshold(uint16_t BUVL);
    uint16_t GetBusUndervoltageThreshold();
    void SetTemperatureOverLimitThreshold(uint16_t TOL);
    uint16_t GetTemperatureOverLimitThreshold();
    void SetPowerOverLimitThreshold(uint16_t POL);
    uint16_t GetPowerOverLimitThreshold();
    uint16_t GetManufacturerId();
    uint16_t GetDeviceId();
  private:
  	uint8_t _i2caddr;
};

#endif