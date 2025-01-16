#ifndef _MY_ADC_H_
#define _MY_ADC_H_

void initialize_lux_sensor();
void set_lux_range(char range);
float get_lux(char Range);
float get_lux_auto_range(char InitialRange);
void initialize_thermister_pins();
float get_thermister_temperature();

#endif