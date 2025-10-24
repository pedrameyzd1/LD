#include <stdio.h>
#include <time.h>
#include "config.h"
#include "sensor_manager.h"
#include "averages.h"
#include "mister_control.h"
#include "sensor_index.h"
#include "alert_service.h"

extern void platform_delay_seconds(int s);
extern void platform_init_time_and_wifi(void);
extern float platform_read_sensor_temperature(int sensor_id, bool *ok);
extern void platform_set_mister_gpio(int hectare_id, bool on);

int main(void){
    platform_init_time_and_wifi();
    alert_service_init();
    sensor_manager_init();
    averages_init();
    mister_control_init();

    time_t last_hour_agg = 0;

    while(1){
        sensor_manager_poll_all();

        float hectare_sum[NUM_HECTARES];
        int hectare_count[NUM_HECTARES];
        for(int i=0;i<NUM_HECTARES;i++){ hectare_sum[i]=0.0f; hectare_count[i]=0; }

        for(int s=0;s<NUM_SENSORS;s++){
            const sensor_t *sn = sensor_manager_get_sensor(s);
            int h = sensor_to_hectare_map[s];
            if(sn && sn->status == SENSOR_OK){
                hectare_sum[h] += sn->last_temp;
                hectare_count[h] += 1;
            }
        }

        float hectare_avg[NUM_HECTARES];
        for(int h=0;h<NUM_HECTARES;h++){
            if(hectare_count[h] > 0) hectare_avg[h] = hectare_sum[h] / (float)hectare_count[h];
            else hectare_avg[h] = NAN;
            mister_control_update_hectare(h, hectare_avg[h]);
            printf("Hectare %d avg = %.2f\n", h, hectare_avg[h]);
        }

        time_t now = time(NULL);
        if(last_hour_agg==0 || difftime(now, last_hour_agg) >= 3600.0){
            for(int h=0;h<NUM_HECTARES;h++){
                averages_store_hourly(h, hectare_avg[h]);
            }
            last_hour_agg = now;
        }

        platform_delay_seconds(SENSOR_POLL_INTERVAL_S);
    }
    return 0;
}
