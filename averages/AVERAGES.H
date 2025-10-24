#pragma once
#include <stdint.h>

void averages_init(void);
void averages_store_hourly(int hectare_id, float avg_hour);
float averages_get_hectare_avg_hours(int hectare_id, int hours);
float averages_get_hectare_avg_days(int hectare_id, int days);
