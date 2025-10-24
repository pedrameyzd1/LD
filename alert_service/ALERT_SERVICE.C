#include "alert_service.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

static char last_alert[512];

extern int platform_http_post(const char *host, int port, const char *path, const char *body, char *response, int resp_len);

void alert_service_init(void){
    last_alert[0]=0;
}

void alert_service_post_alert(const char *json_payload){
    strncpy(last_alert, json_payload, sizeof(last_alert)-1);
    last_alert[sizeof(last_alert)-1]=0;
    char response[256];
    int rc = platform_http_post(ALERT_SERVER_HOST, ALERT_SERVER_PORT, ALERT_POST_PATH, json_payload, response, sizeof(response));
    if(rc < 0){
        printf("Alert POST failed: %s\n", json_payload);
    } else {
        printf("Alert sent (%d): %s\n", rc, json_payload);
    }
}

const char* alert_service_get_latest(void){
    return last_alert;
}
