#include "mister_control.h"
#include "config.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

static int hectare_state[NUM_HECTARES];
static time_t hectare_cycle_start[NUM_HECTARES];
static int hectare_cycle_phase[NUM_HECTARES];

extern void platform_set_mister_gpio(int hectare_id, bool on);

void mister_control_init(void){
    for(int i=0;i<NUM_HECTARES;i++){
        hectare_state[i]=0;
        hectare_cycle_phase[i]=0;
        hectare_cycle_start[i]=0;
        platform_set_mister_gpio(i,false);
    }
}

void mister_control_update_hectare(int hectare_id, float current_avg){
    if(hectare_id<0 || hectare_id>=NUM_HECTARES) return;
    time_t now = time(NULL);
    if(isnan(current_avg)){
        platform_set_mister_gpio(hectare_id,false);
        hectare_state[hectare_id]=0;
        hectare_cycle_phase[hectare_id]=0;
        return;
    }
    if(current_avg < TEMP_THRESHOLD_OFF){
        if(hectare_state[hectare_id] != 0){
            platform_set_mister_gpio(hectare_id,false);
            hectare_state[hectare_id]=0;
            hectare_cycle_phase[hectare_id]=0;
        }
        return;
    } else {
        if(hectare_cycle_phase[hectare_id]==0){
            hectare_cycle_phase[hectare_id]=1;
            hectare_cycle_start[hectare_id]=now;
            platform_set_mister_gpio(hectare_id,true);
            hectare_state[hectare_id]=1;
            return;
        } else if(hectare_cycle_phase[hectare_id]==1){
            if(difftime(now, hectare_cycle_start[hectare_id]) >= 180.0){
                hectare_cycle_phase[hectare_id]=2;
                hectare_cycle_start[hectare_id]=now;
                platform_set_mister_gpio(hectare_id,false);
                hectare_state[hectare_id]=0;
            }
            return;
        } else if(hectare_cycle_phase[hectare_id]==2){
            if(difftime(now, hectare_cycle_start[hectare_id]) >= 120.0){
                hectare_cycle_phase[hectare_id]=1;
                hectare_cycle_start[hectare_id]=now;
                platform_set_mister_gpio(hectare_id,true);
                hectare_state[hectare_id]=1;
            }
            return;
        }
    }
}
