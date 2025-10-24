# LD
small project of PLC with ladder and C
LD/
│
├── main.c
├── config.h
sensor_manager/
├── sensor_index.h
├── sensor_manager.c
├── sensor_manager.h
alret_service/
├── alert_service.c
├── alert_service.h
averages/
├── averages.c
├── averages.h
mister_control/
├── mister_control.c
├── mister_control.h
└── README.md
# 🌿 Greenhouse Auto-Misting System (C Version)

This project is a full C-based controller that keeps your greenhouse temperature steady by automatically switching misting systems on and off.  
It’s made for a large-scale setup (like 8 hectares) with multiple sensors per area.

---

## 💡 How it works
- Each hectare has a few temperature sensors (default: 2 per hectare → 16 total).  
- Every minute, the system reads all sensors, checks if they’re healthy, and calculates average temps per hectare.
- If a sensor fails multiple times, it triggers a POST request with an error message to your server (for your Easy Pro Builder frontend to show).
- If the average temp in a hectare is below **35°C**, the misters stay **off**.  
- If it’s above **35°C**, the system starts a repeating cycle: **3 minutes ON**, **2 minutes OFF**, until things cool down.
- The program stores hourly averages and can later compute daily, weekly, or monthly trends (up to 3 months of data).

---

## ⚙️ Folder Overview
| File | Description |
|------|--------------|
| `main.c` | Main loop, controls everything. |
| `config.h` | Core settings (thresholds, server info, counts). |
| `sensor_manager.*` | Reads sensors, detects failures, triggers alerts. |
| `alert_service.*` | Handles POST requests to your alert server. |
| `mister_control.*` | Manages the ON/OFF logic for misters. |
| `averages.*` | Stores historical hourly averages. |
| `sensor_index.h` | Maps each sensor to its hectare. |

---

## 🔌 Platform requirements
You’ll need to implement a few platform-specific functions:
```c
float platform_read_sensor_temperature(int id, bool *ok);
int platform_http_post(const char *host, int port, const char *path, const char *body, char *resp, int len);
void platform_set_mister_gpio(int hectare_id, bool on);
void platform_delay_seconds(int s);
void platform_init_time_and_wifi(void);
