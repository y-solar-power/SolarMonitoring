#include <pico/stdlib.h>
#include <string.h>
#include "hardware/i2c.h"
#include "RTC8564NB.h"
#include "ff.h"

unsigned char RTC8564NB::ctrl2 = 0x00;

// ＢＣＤをバイナリ(10進数)に変換する処理(BCDは２桁まで対応)
unsigned int RTC8564NB::bcd2bin(unsigned char dt)
{
     return ((dt >> 4) * 10) + (dt & 0xf) ;
}
// バイナリ(10進数)をＢＣＤに変換する処理(0-255までの変換に対応)
unsigned int RTC8564NB::bin2bcd(unsigned int num) 
{
     return ((num/100) << 8) | ( ((num%100) / 10) << 4 ) | (num%10) ;
}

/// @brief "yyyy/mm/dd www hh:mm:ss"で返す　例)"2010/01/15 TUE 15:30:00"
/// @param tm 
/// @param c 
void RTC8564NB::TimeToString(unsigned char *tm, unsigned char *c)
{
     unsigned char dt[7][4] = { "SUN","MON","TUE","WED","THU","FRI","SAT" } ;
     unsigned char buf[24] ;

     buf[23] = 0x00 ;
     RTC8564NB::set_ctime(*tm,':',&buf[20]) ;    // 秒
     tm++ ;
     RTC8564NB::set_ctime(*tm,':',&buf[17]) ;    // 分
     tm++ ;
     RTC8564NB::set_ctime(*tm,' ',&buf[14]) ;    // 時
     tm++ ;
     buf[10] = ' ' ;
     RTC8564NB::set_ctime(*tm,'/',&buf[7]) ;    // 日
     tm++ ;
     memcpy(&buf[11],&dt[*tm & 0x0f][0],3) ; // 曜日
     tm++ ;
     RTC8564NB::set_ctime(*tm,'/',&buf[4]) ;    // 月
     tm++ ;
     RTC8564NB::set_ctime(*tm,'0',&buf[1]) ;    // 年
     buf[0] = '2' ;
     memcpy(c,buf,24) ;
}

void RTC8564NB::set_ctime(unsigned char tm,unsigned char s,unsigned char *a){
     *a = s ;
     a++ ;
     *a = (tm >> 4) + 0x30 ;
     a++ ;
     *a = (tm & 0x0f) + 0x30 ;
}

/// @brief RTCにI2CでWRITE
/// @param data 
/// @param len 
/// @return 
int RTC8564NB::i2c_write(unsigned char *data, unsigned int len){
     return i2c_write_blocking(i2c0, RTC_ADRS, (const uint8_t *)data, len, false);
}

/// @brief データの読み書き対象のアドレスの設定
/// @param address 
/// @return 
int RTC8564NB::i2c_write_single(unsigned char value){
     unsigned char data[1] = {value};
     return i2c_write_blocking(i2c0, RTC_ADRS, (const uint8_t *)data, 1, false);
}

/// @brief RTCにI2CでREAD
/// @param buff 
/// @param len 
/// @return 
int RTC8564NB::i2c_read(unsigned char *buff, unsigned int len){
     return i2c_read_blocking(i2c0, RTC_ADRS, (uint8_t *)buff, len, false);
}

// Constructors
RTC8564NB::RTC8564NB()
{
}

/// @brief RTC停止
/// @return 
int RTC8564NB::StopClock()
{
     unsigned char data_to_write[] = {
         0x00, // レジスターアドレスは00hを指定する
         0x20, // Control1(Reg00)の設定(TEST=0,STOP=1)
     };
     return RTC8564NB::i2c_write(data_to_write, 2);
}

/// @brief RTC動作開始
/// @return 
int RTC8564NB::StartClock(){
     unsigned char data_to_write[] = {
          0x00,                    // レジスターアドレスは00hを指定する
          0x00,                    // Control1(Reg00)の設定(TEST=0,STOP=0)
     };
     return RTC8564NB::i2c_write(data_to_write, 2);  
}

/// @brief 
/// @param Year 0-99 下二けた
/// @param Mon 1-12
/// @param mDay 1-31
/// @param wDay 日[0] 月[1] 火[2] 水[3] 木[4] 金[5] 土[6]
/// @param Hour 0-23
/// @param Min 0-59
/// @param Sec 0-59
/// @return 
void RTC8564NB::SetTime(unsigned char Year,unsigned char Mon,unsigned char mDay,unsigned char wDay,unsigned char Hour,unsigned char Min,unsigned char Sec){
     RTC8564NB::StopClock();
     unsigned char data_to_write[] = {
          0x02,                    // レジスターアドレスは02hを指定する
          (unsigned char)RTC8564NB::bin2bcd(Sec),      // Seconds(Reg02)の設定(時刻の秒0-59,VL=0)
          (unsigned char)RTC8564NB::bin2bcd(Min),      // Minutes(Reg03)の設定(時刻の分0-59)
          (unsigned char)RTC8564NB::bin2bcd(Hour),     // Hours(Reg04)の設定(時刻の時0-23)
          (unsigned char)RTC8564NB::bin2bcd(mDay),     // Days(Reg05)の設定(カレンダの日1-31)
          (unsigned char)RTC8564NB::bin2bcd(wDay),     // WeekDays(Reg06)の設定(カレンダの曜日0-6)
          (unsigned char)RTC8564NB::bin2bcd(Mon),      // Months(Reg07)の設定(カレンダの月1-12)
          (unsigned char)RTC8564NB::bin2bcd(Year),     // Years(Reg08)の設定(カレンダの年00-99)
     };
     RTC8564NB::i2c_write(data_to_write, 8);
     RTC8564NB::StartClock();
}

/// @brief 配列にはＢＣＤデータで秒・分・時・日・曜日・月・年の順で保存する 
/// @param Buff With 7 unsigned chars
void RTC8564NB::GetTime(unsigned char *Buff){
     RTC8564NB::i2c_write_single(0x02);
     RTC8564NB::i2c_read(Buff, 7);

     char filter[7] = {0x7f, 0x7f, 0x3f, 0x3f, 0x07, 0x1f, 0xFF};
     for(char i = 0; i < 7; i++){
          Buff[i] = Buff[i] & filter[i];
     }
}


/// @brief 定周期タイマーの設定をする処理（タイマーの開始）
/// @param sclk ソースクロックの指定(244.14us[0] 15.625ms[1] 1sec[2] 1min[3])
/// @param count カウント値の指定(1-255) sclk=2 count=10 なら10秒周期となる
void RTC8564NB::SetTimer(unsigned char sclk,unsigned char count)
{
     unsigned char count_down_reset[2] = {0x0f, count};
     unsigned char start_timer[2] = {0x0e, (unsigned char)(sclk | 0x80)};
     
     RTC8564NB::i2c_write(count_down_reset, 2);
     RTC8564NB::i2c_write(start_timer, 2);
}

/// @brief 定周期タイマー機能を無効にする処理（タイマーの終了）       
void RTC8564NB::StopTimer(){
     unsigned char stop_timer[2] = {0x0e, 0x00};
     ctrl2 = ctrl2 & 0xfb;
     unsigned char stop_alarm[2] = {0x01, ctrl2};

     RTC8564NB::i2c_write(stop_timer, 2);
     RTC8564NB::i2c_write(stop_alarm, 2);
}

/// @brief アラームの日時を設定する処理（アラームの開始）           
/// @param Hour 時刻の時(0-23)を指定する、0xff指定でHour設定は無効          
/// @param Min 時刻の分(0-59)を指定する、0xff指定でMin設定は無効               
/// @param mDay 日付の日(1-31)を指定する、0xff指定でmDay設定は無効            
/// @param wDay 曜日の指定(日[0] 月[1] 火[2] 水[3] 木[4] 金[5] 土[6])   0xff指定でwDay設定は無効                       
void RTC8564NB::SetAlarm(unsigned char Hour,unsigned char Min,unsigned char mDay,unsigned char wDay)
{
     unsigned char set_alarm[5] = {
          0x09,
          Min == 0xFF ? (unsigned char)0x80 : (unsigned char)bin2bcd(Min),
          Hour == 0xFF ? (unsigned char)0x80 : (unsigned char)bin2bcd(Hour),
          mDay == 0xFF ? (unsigned char)0x80 : (unsigned char)bin2bcd(mDay),
          wDay == 0xFF ? (unsigned char)0x80 : (unsigned char)bin2bcd(wDay)
     };
     RTC8564NB::i2c_write(set_alarm, 5);

     ctrl2 = (ctrl2 | 0x02) & 0xf7 ; 
     unsigned char start_alarm[2] = {0x01, ctrl2};
     RTC8564NB::i2c_write(start_alarm, 5);
}


/// @brief アラーム機能を無効にする処理（アラーム終了）                 
void RTC8564NB::StopAlarm()
{
     ctrl2 = ctrl2 & 0xf5;
     unsigned char stop_alarm[2] = {0x01, ctrl2};
     RTC8564NB::i2c_write(stop_alarm, 2);
}

/// @brief アラームの発生を解除する処理（アラームは引き続き有効）       
void RTC8564NB::OffAlarm()
{
     ctrl2 = ctrl2 & 0xf7;
     unsigned char stop_alarm[2] = {0x01, ctrl2};
     RTC8564NB::i2c_write(stop_alarm, 2);
}

bool RTC8564NB::interrupted = false;

void RTC8564NB::InterruptHandler(uint gpio, uint32_t events){
     interrupted = true; 
}

void RTC8564NB::AttachInterrupt(unsigned char Pin, gpio_irq_callback_t Handler){
     gpio_init(Pin);
     gpio_set_dir(Pin, false);
     gpio_set_pulls(Pin, true, false);

     if(Handler == NULL) Handler = &InterruptHandler;
     gpio_set_irq_enabled_with_callback(Pin, GPIO_IRQ_EDGE_RISE , true, Handler);
}

bool RTC8564NB::IsInitialized(){
     RTC8564NB::i2c_write_single(0x01);
     unsigned char current_stat[2];
     RTC8564NB::i2c_read(current_stat, 2);
     ctrl2 = current_stat[1];
     return current_stat[1] & 0x80 == 0;
}

void RTC8564NB::Initialize(unsigned char Year,unsigned char Mon,unsigned char mDay,unsigned char wDay,unsigned char Hour,unsigned char Min,unsigned char Sec){
     ctrl2 = 0x11;
     unsigned char data_to_write[] = {
          0x00,                    // レジスターアドレスは00hを指定する
          0x20,                    // Control1(Reg00)の設定(TEST=0,STOP=1)
          ctrl2,                   // Control2(Reg01)の設定(割込み禁止)
          (unsigned char)RTC8564NB::bin2bcd(Sec),      // Seconds(Reg02)の設定(時刻の秒0-59,VL=0)
          (unsigned char)RTC8564NB::bin2bcd(Min),      // Minutes(Reg03)の設定(時刻の分0-59)
          (unsigned char)RTC8564NB::bin2bcd(Hour),     // Hours(Reg04)の設定(時刻の時0-23)
          (unsigned char)RTC8564NB::bin2bcd(mDay),     // Days(Reg05)の設定(カレンダの日1-31)
          (unsigned char)RTC8564NB::bin2bcd(wDay),     // WeekDays(Reg06)の設定(カレンダの曜日0-6)
          (unsigned char)RTC8564NB::bin2bcd(Mon),      // Months(Reg07)の設定(カレンダの月1-12)
          (unsigned char)RTC8564NB::bin2bcd(Year),     // Years(Reg08)の設定(カレンダの年00-99)
          0x80,                    // MinuteAlarm(Reg09)の設定(アラームの分無効)
          0x80,                    // HourAlarm(Reg0A)の設定(アラームの時無効)
          0x80,                    // HourAlarm(Reg0B)の設定(アラームの日無効)
          0x80,                    // WeekDayAlarm(Reg0C)の設定(アラームの曜日無効)
          0x83,                    // CLKOUT(Reg0D)の設定(1Hzで出力する)
          0x00,                    // TimerControl(Reg0E)の設定(タイマ機能は無効)
          0x00                     // Timer(Reg0F)の設定(タイマ初期値は０)
     };
     RTC8564NB::i2c_write(data_to_write, 17);
}


uint32_t RTC8564NB::GetFatTime(){
     unsigned char Buff[7] = {0};
     RTC8564NB::GetTime(Buff);
     int32_t Value = (bcd2bin(Buff[0]) / 2 & 0b1111) | (bcd2bin(Buff[1]) & 0b111111) << 5 | (bcd2bin(Buff[2]) & 0b11111) << 11 | \
      (bcd2bin(Buff[3]) & 0b11111) << 16 | (bcd2bin(Buff[5]) & 0b1111) << 21 | ((bcd2bin(Buff[6]) & 0b1111111) + 20) << 25;

      return Value;
}

DWORD get_fattime(void){
     RTC8564NB rtc = RTC8564NB();
     return rtc.GetFatTime();
}