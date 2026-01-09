#ifndef UI_H
#define UI_H

#ifdef HAS_DISPLAY

#include <TFT_eSPI.h>
#include <lvgl.h>
#include "huawei_inverter.h"

enum ConnectionStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR
};

class UI {
public:
    static void begin();
    static void update();

    // Enhanced status update methods (now public)
    static void updateModbusStatus(ConnectionStatus status);
    static void updateWiFiStatus(ConnectionStatus status);

private:
    static TFT_eSPI tft;
    static lv_disp_drv_t disp_drv;
    static lv_disp_t* disp;
    
    // Existing data labels
    static lv_obj_t* soc_label;
    static lv_obj_t* input_power_label;
    static lv_obj_t* active_power_label;
    
    // Status indicators
    static lv_obj_t* modbus_status_label;
    static lv_obj_t* wifi_status_label;
    static lv_obj_t* modbus_status_icon;
    static lv_obj_t* wifi_status_icon;

    static void tft_init();
    static void lvgl_init();
    static void create_ui();
    static void disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
    
    // Helper method to set status icon color
    static void setStatusIconColor(lv_obj_t* icon, ConnectionStatus status);
};

#endif // HAS_DISPLAY
#endif // UI_H