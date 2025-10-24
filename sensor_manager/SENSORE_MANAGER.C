#include "sensor_manager.h"
#include "config.h"
#include "alert_service.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

static sensor_t sensors[NUM_SENSORS];

extern float platform_read_sensor_temperature(int sensor_id, bool *ok); 

void sensor_manager_init(void){
    for(int i=0;i<NUM_SENSORS;i++){
        sensors[i].id = i;
        sensors[i].last_temp = NAN;
        sensors[i].status = SENSOR_NO_RESPONSE;
        sensors[i].consecutive_failures = 0;
        sensors[i].last_valid_ts = 0;
    }
}

void sensor_manager_handle_failure(int id, const char *reason){
    sensors[id].consecutive_failures++;
    if(sensors[id].consecutive_failures >= SENSOR_MAX_CONSECUTIVE_FAILS){
        sensors[id].status = SENSOR_FAIL;
        char msg[256];
        snprintf(msg, sizeof(msg), "{\"sensor_id\":%d, \"error\":\"%s\"}", id, reason);
        alert_service_post_alert(msg);
    }
}

void sensor_manager_handle_success(int id, float temp){
    sensors[id].last_temp = temp;
    sensors[id].status = SENSOR_OK;
    sensors[id].consecutive_failures = 0;
    sensors[id].last_valid_ts = (unsigned long)time(NULL);
}

void sensor_manager_poll_all(void){
    for(int i=0;i<NUM_SENSORS;i++){
        bool ok=false;
        float t = platform_read_sensor_temperature(i, &ok);
        if(!ok || isnan(t) || t < SENSOR_MIN_VALID || t > SENSOR_MAX_VALID){
            sensor_manager_handle_failure(i, "read_invalid_or_no_response");
        } else {
            sensor_manager_handle_success(i, t);
        }
    }
}

const sensor_t* sensor_manager_get_sensor(int id){
    if(id<0 || id>=NUM_SENSORS) return NULL;
    return &sensors[id];
}

int sensor_manager_get_num_sensors(void){ return NUM_SENSORS; }
