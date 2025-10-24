#include "averages.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HOURLY_SLOTS (24*90)

typedef struct {
    float slots[HOURLY_SLOTS];
    int write_idx;
    int filled;
} hectare_history_t;

static hectare_history_t history[NUM_HECTARES];

void averages_init(void){
    memset(history, 0, sizeof(history));
    for(int i=0;i<NUM_HECTARES;i++){
        for(int j=0;j<HOURLY_SLOTS;j++) history[i].slots[j]=NAN;
        history[i].write_idx = 0;
        history[i].filled = 0;
    }
}

void averages_store_hourly(int hectare_id, float avg_hour){
    if(hectare_id<0 || hectare_id>=NUM_HECTARES) return;
    hectare_history_t *h = &history[hectare_id];
    h->slots[h->write_idx] = avg_hour;
    h->write_idx = (h->write_idx+1) % HOURLY_SLOTS;
    if(h->filled < HOURLY_SLOTS) h->filled++;
}

float averages_get_hectare_avg_hours(int hectare_id, int hours){
    if(hectare_id<0 || hectare_id>=NUM_HECTARES || hours<=0) return NAN;
    hectare_history_t *h = &history[hectare_id];
    if(h->filled==0) return NAN;
    if(hours > h->filled) hours = h->filled;
    float sum=0.0f; int cnt=0;
    int idx = (h->write_idx - 1 + HOURLY_SLOTS) % HOURLY_SLOTS;
    for(int i=0;i<hours;i++){
        float v = h->slots[idx];
        if(!isnan(v)){ sum += v; cnt++; }
        idx = (idx -1 + HOURLY_SLOTS)%HOURLY_SLOTS;
    }
    if(cnt==0) return NAN;
    return sum / cnt;
}

float averages_get_hectare_avg_days(int hectare_id, int days){
    if(days<=0) return NAN;
    return averages_get_hectare_avg_hours(hectare_id, days*24);
}
