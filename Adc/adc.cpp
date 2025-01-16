// Default Include
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "Adc/adc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define LUX_SENSOR_BRIGHT 20
#define LUX_SENSOR_NORMAL 21
#define LUX_SENSOR_DARK 22


void initialize_lux_sensor(){
    adc_gpio_init(26);                //ADC0(GPIO26)の端子設定

    gpio_init(LUX_SENSOR_BRIGHT);
    gpio_set_dir(LUX_SENSOR_BRIGHT, GPIO_IN);
    gpio_init(LUX_SENSOR_NORMAL);
    gpio_set_dir(LUX_SENSOR_NORMAL, GPIO_IN);
    gpio_init(LUX_SENSOR_DARK);
    gpio_set_dir(LUX_SENSOR_DARK, GPIO_IN);
}

char const MaxLuxRange = 2;
char CurrentLuxRange = 0;

void set_lux_range(char range){
    CurrentLuxRange = range;
    gpio_set_dir(LUX_SENSOR_BRIGHT, CurrentLuxRange == 2 ? GPIO_OUT : GPIO_IN);
    gpio_disable_pulls(LUX_SENSOR_BRIGHT);
    gpio_put(LUX_SENSOR_BRIGHT, 0);
    gpio_set_dir(LUX_SENSOR_NORMAL, CurrentLuxRange == 1 ? GPIO_OUT : GPIO_IN);
    gpio_disable_pulls(LUX_SENSOR_NORMAL);
    gpio_put(LUX_SENSOR_NORMAL, 0);
    gpio_set_dir(LUX_SENSOR_DARK, CurrentLuxRange == 0 ? GPIO_OUT : GPIO_IN);
    gpio_disable_pulls(LUX_SENSOR_DARK);
    gpio_put(LUX_SENSOR_DARK, 0);
}

float get_lux(char Range){
    adc_select_input(0);
    set_lux_range(Range);
    float voltage = adc_read() / 4095.0 * 3.3;      
    if(voltage > 3) return -1;
    float r = (CurrentLuxRange == 0 ? 2169.0 : (CurrentLuxRange == 1 ? 219.2 : 22.1));
    float current = voltage / r;
    float lx = 2.17 * current * 1000000.0;
    return lx;
}

float get_lux_auto_range(char InitialRange){
    float Lux = get_lux(InitialRange);
    if(Lux == -1) return get_lux_auto_range(InitialRange + 1);
    else return Lux;
}

void initialize_thermister_pins(){
    adc_gpio_init(28);                //ADC2(GPIO28)の端子設定
}

float get_thermister_temperature(){
    adc_select_input(2);
    float voltage = adc_read() / 4095.0 * 3.3;   
    float temp = 1.0 / ( 1.0 / (25 + 273) + log(voltage / (3.3 - voltage)) / 3380.0) - 273;
    return temp;
}
