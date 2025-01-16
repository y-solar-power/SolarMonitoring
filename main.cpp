// Default Include
#include "pico.h"
#include "pico/sleep.h"
#include <pico/stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Self Include
#include "main.h"

// Arducam Include
#include "Arducam/Arducam_Mega.h"
#include "Arducam/Core/cam_spi_master.h"
#include "Arducam/Core/ArducamCamera.h"

// I2C Include
#include "SHT31/SHT31.h"
#include "RTC8564NB/RTC8564NB.h"
#include "INA228/INA228.h"
#include "BNO055/BNO055.h"
#include "LPS25HB/LPS25HB.h"

// SdCard(FatFs) Include
#include "FatFs/ff.h"
#include "FatFs/diskio.h"

// My Sensor Adc Include
#include "Adc/adc.h"

// Raspberry Pi Pico Hardware Include
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// Port Define
#define PIN_SW 15
#define PIN_LED 14
#define PIN_RTC_INT 0
#define PIN_WIFI_EN 23
#define PIN_CAM_CS 17

// I2C Port Define
#define PICO_DEFAULT_I2C_SDA_PIN 4
#define PICO_DEFAULT_I2C_SCL_PIN 5

RTC8564NB rtc = RTC8564NB();
SHT31 sht = SHT31(0x45);
INA228 ina = INA228(0x40);
BNO055 bno = BNO055(0x28);
LPS25HB lps = LPS25HB();

#define READ_IMAGE_LENGTH 255
Arducam_Mega myCAM = Arducam_Mega(PIN_CAM_CS);
uint32_t readImageLength = 0;
uint8_t jpegHeadFlag = 0;

char volatile intr_rtc_tmr = 0;
char intr_rtc_tmr_cnt = 0;
void rtc_tmr_handler(uint gpio, uint32_t events){
    if(++intr_rtc_tmr_cnt == 1){
        intr_rtc_tmr_cnt = 0;
        intr_rtc_tmr = 1;
    }
}

//
// LED BLINK
//
unsigned char _blink = 0;
void toggle_led(uint gpio, uint32_t events)
{
    _blink = 1 - _blink;
    gpio_put(PIN_LED, _blink);
    rtc.interrupted = true;
}

void infinite_blink()
{
    while (true)
    {
        toggle_led(0, 0);
        sleep_ms(50);
    }
}

//
// INITIALIZE
//
void init_gpio()
{
    gpio_init(PIN_SW);
    gpio_set_dir(PIN_SW, GPIO_IN);
    gpio_pull_up(PIN_SW);

    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    gpio_init(PIN_RTC_INT);
    gpio_set_dir(PIN_RTC_INT, false);
    gpio_set_pulls(PIN_RTC_INT, true, false);

    // Wifi turn off
    gpio_init(PIN_WIFI_EN);
    gpio_set_dir(PIN_WIFI_EN, GPIO_OUT);
    gpio_put(PIN_WIFI_EN, 0);
}

void init_i2c()
{
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    sht.ReadStatus();
    sht.SoftReset();

    // if (rtc.IsInitialized() == 0)
    //     rtc.Initialize(getchar(), getchar(), getchar(), getchar(), getchar(), getchar(), getchar());

    rtc.StartClock();
    // rtc.AttachInterrupt(0, &rtc_tmr_handler);
    rtc.SetTimer(2, 15);

    ina.SetConfiguration(false, false, 0x00, false, true);
    ina.SetAdcConfiguration(0x0F, 0x00, 0x00, 0x00, 0x02);

    bno.SetOprMode(0b1100);
    bno.SetPwrMode(0b00);
    bno.SetAccOffsetX(0x0100);
    bno.SetAccOffsetY(0x0100);
    bno.SetAccOffsetZ(0x0100);

    lps.SetToNormalMode();
}

int init_sd(void)
{
    DSTATUS ret;
    int result = 0;
    ret = disk_initialize(0);
    if (ret & STA_NOINIT)
    {
        result = -1;
    }
    return result;
}

void init_camera(){
    myCAM.begin();  
    setAutoExposure(myCAM.getCameraInstance(), 1);
    setAutoWhiteBalance(myCAM.getCameraInstance(), 1);
}

void init_camera_mode(){
    setAutoWhiteBalance(myCAM.getCameraInstance(), 0);
    setAutoWhiteBalanceMode(myCAM.getCameraInstance(), CAM_WHITE_BALANCE_MODE_DEFAULT);
    setAutoISOSensitive(myCAM.getCameraInstance(), 0);
    setISOSensitivity(myCAM.getCameraInstance(), 1);
    setAutoExposure(myCAM.getCameraInstance(), 0);
    setAbsoluteExposure(myCAM.getCameraInstance(), 100);
}

void init_monitoring_system()
{
    stdio_init_all();
    init_gpio();
    init_i2c();
    init_sd();
    init_camera();
    adc_init();
    initialize_lux_sensor();
    initialize_thermister_pins();
}

void take_image(unsigned char *Time){
    FIL  fil;
    FRESULT  ret;
    UINT  wsize ;

    char file_name[32] = {0};
    sprintf(file_name, "%04d%02d%02d%02d%02d%02d.JPG", 
    RTC8564NB::bcd2bin(Time[6]) + 2000, RTC8564NB::bcd2bin(Time[5]), RTC8564NB::bcd2bin(Time[3]), 
    RTC8564NB::bcd2bin(Time[2]), RTC8564NB::bcd2bin(Time[1]), RTC8564NB::bcd2bin(Time[0]));
    ret = f_open( &fil, file_name, FA_WRITE|FA_CREATE_ALWAYS );
    if( ret != FR_OK ) {
        infinite_blink();
    }
    // Take a picture
    init_camera_mode();
    myCAM.takePicture((CAM_IMAGE_MODE)CAM_IMAGE_MODE_96X96, (CAM_IMAGE_PIX_FMT)CAM_IMAGE_PIX_FMT_JPG);

    // Prepare Image Info
    uint8_t buff[READ_IMAGE_LENGTH] = {0};
    uint8_t rtLength = 0;
    ArducamCamera* cameraInstance = myCAM.getCameraInstance();
    uint32_t len = myCAM.getReceivedLength();
    
    // Read the image and save a file.
    while (myCAM.getReceivedLength()) {
        rtLength = readBuff(cameraInstance, buff, READ_IMAGE_LENGTH);
        ret = f_write( &fil, buff, (UINT)rtLength, &wsize );
        if( ret != FR_OK ) {
            infinite_blink();
        }
    }

    // Close the file.
    f_close( &fil );
}

bool take_sensor(unsigned char *rtc_time){
    FRESULT  ret;
    FIL  fil;
    UINT  wsize ;
    ret = f_open( &fil, "records.csv", FA_WRITE | FA_OPEN_ALWAYS );
    ret = f_lseek(&fil, f_size(&fil));
    if( ret != FR_OK ) {
        infinite_blink();
    }

    sht.GetTempHum();

    float temp_sht = sht.Temperature();
    float temp_ina = ina.GetDieTemp() * 7.8125 * 0.001;
    float temp_thermister = get_thermister_temperature();
    float temp_lps = lps.GetTemp();

    float humid_sht = sht.Humidity();

    float pressure_lps = lps.GetPressure();

    float elec_vbus = ina.GetVBus() * 195.3125 * 0.000001;
    float elec_current = ina.GetCurrentResult() * 0.00390625;
    float elec_power = ina.GetPower() * 3.2  *  0.00390625 * 0.001;

    float dir_magnetic = 180 / M_PI * atan2((int16_t)bno.GetMagY(), (int16_t)bno.GetMagX());
    dir_magnetic -= 7.5;
    dir_magnetic += dir_magnetic < 0 ? 360 : 0;

    float dir_angle = 180 / M_PI * atan2((int16_t)bno.GetAccelerationZ(), (int16_t)(sqrt(bno.GetAccelerationX()*bno.GetAccelerationX() + bno.GetAccelerationY()*bno.GetAccelerationY())));

    // Prepare save data
    char write[300] = {0};
    
    sprintf(write, "%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n",
        rtc.bcd2bin(rtc_time[0]),rtc.bcd2bin(rtc_time[1]),
        rtc.bcd2bin(rtc_time[2]),rtc.bcd2bin(rtc_time[3]),
        rtc.bcd2bin(rtc_time[4]),rtc.bcd2bin(rtc_time[5]),
        rtc.bcd2bin(rtc_time[6]),
        temp_sht, temp_ina, temp_thermister,temp_lps,
        humid_sht,
        pressure_lps,
        elec_vbus, elec_current, elec_power,
        dir_magnetic, dir_angle
    );

    printf(write);
    
    // Save a file
    ret = f_write( &fil, write, 300, &wsize );
    if( ret != FR_OK ) {
        infinite_blink();
    }

    f_close(&fil);

    return true;
}

int main()
{
    FATFS  fs;
    FRESULT  ret;

    init_monitoring_system();

    ret = f_mount( &fs, "", 1 );
    if( ret != FR_OK ) {
        infinite_blink();
    }

    while(true){
        sleep_run_from_xosc();
        sleep_goto_dormant_until_pin(0, true, true);
        sleep_power_up();
        if(gpio_get(PIN_SW) == 0) break;

        toggle_led(0,0);

        unsigned char rtc_time[7];
        rtc.GetTime(rtc_time);

        // Take Sensor Value to CSV
        take_sensor(rtc_time);

        myCAM.getCameraInstance()->arducamCameraOp->writeReg(myCAM.getCameraInstance(), 0X02, 0b101);
        take_image(rtc_time);
        myCAM.getCameraInstance()->arducamCameraOp->writeReg(myCAM.getCameraInstance(), 0X02, 0b011);

        toggle_led(0,0);
    }
    
    infinite_blink();

    return 0;
}