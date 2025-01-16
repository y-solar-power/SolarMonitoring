#ifndef _SHT31_H_
#define _SHT31_H_

class SHT31{
  public:
    SHT31(uint8_t i2caddr);
    void BussReset(void);
    void SoftReset(void);
    void Heater(uint8_t onoff);
    uint16_t ReadStatus(void);
    void GetTempHum(void);
    float Temperature(void);
    float Humidity(void);
  private:
  	uint8_t _i2caddr;
    void i2c_write(uint16_t data);
    void i2c_read(uint8_t data[],uint8_t num);
    float humidity, temperature;
};

#endif