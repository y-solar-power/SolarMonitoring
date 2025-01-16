#ifndef _LPS25HB_H_
#define _LPS25HB_H_

#define LPS25HB_ADDR 0b1011100
#define LPS25HB_REG_WHO_AM_I 0x0F
#define LPS25HB_REG_CTRL_REG1 0x20
#define LPS25HB_REG_PRESS_OUT_XL 0x28
#define LPS25HB_REG_PRESS_OUT_L 0x29
#define LPS25HB_REG_PRESS_OUT_H 0x2A
#define LPS25HB_REG_TEMP_OUT_L 0x2B
#define LPS25HB_REG_TEMP_OUT_H 0x2C

class LPS25HB{
    public:
      LPS25HB();
      void SetToNormalMode();
      double GetTemp();
      double GetPressure();
      char WhoAmI();
    private:
      int i2c_write(unsigned char *data, unsigned int len);
      int i2c_write_single(unsigned char value);
      int i2c_read(unsigned char *buff, unsigned int len);
};

#endif