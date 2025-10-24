#pragma once
void alert_service_init(void);
void alert_service_post_alert(const char *json_payload);
const char* alert_service_get_latest(void);
