
#ifndef skRTClib_h
#define skRTClib_h

#define RTC_ADRS 0B1010001              // ＲＴＣのスレーブアドレス

/*******************************************************************************
*	クラスの定義                                                              *
*******************************************************************************/
class RTC8564NB
{
  public:
    // static unsigned char InterFlag ;
    RTC8564NB() ;

    void TimeToString(unsigned char *tm, unsigned char *c);

    int StopClock();
    int StartClock();

    void SetTime(unsigned char Year,unsigned char Mon,unsigned char mDay,unsigned char wDay,unsigned char Hour,unsigned char Min,unsigned char Sec);
    void GetTime(unsigned char *Buff);

    void SetTimer(unsigned char sclk,unsigned char count);
    void StopTimer() ;

    void SetAlarm(unsigned char Hour,unsigned char Min,unsigned char mDay,unsigned char wDay) ;
    void StopAlarm() ;
    void OffAlarm() ;

    static bool interrupted;
    static void InterruptHandler(uint gpio, uint32_t events);
    void AttachInterrupt(unsigned char Pin, gpio_irq_callback_t Handler);

    bool IsInitialized();
    void Initialize(unsigned char Year,unsigned char Mon,unsigned char mDay,unsigned char wDay,unsigned char Hour,unsigned char Min,unsigned char Sec);

    uint32_t GetFatTime();
    static unsigned int bcd2bin(unsigned char dt) ;
    static unsigned int bin2bcd(unsigned int num) ;

  private:
    static unsigned char ctrl2;
    void set_ctime(unsigned char tm,unsigned char s,unsigned char *a) ;
    int i2c_write(unsigned char *data, unsigned int len);
    int i2c_write_single(unsigned char value);
    int i2c_read(unsigned char *buff, unsigned int len);
} ;

extern RTC8564NB RTC ;

#endif
