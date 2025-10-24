#pragma once
#include <stdbool.h>

typedef enum {
    SENSOR_OK = 0,
    SENSOR_FAIL,
    SENSOR_NO_RESPONSE
} sensor_status_t;

typedef struct {
    int id;
    float last_temp;
    sensor_status_t status;
    int consecutive_failures;
    unsigned long last_valid_ts;
} sensor_t;

void sensor_manager_init(void);
void sensor_manager_poll_all(void);
const sensor_t* sensor_manager_get_sensor(int id);
int sensor_manager_get_num_sensors(void);
