#ifndef UI_H
#define UI_H

#ifdef HAS_DISPLAY

#include <TFT_eSPI.h>
#include <lvgl.h>
#include "huawei_inverter.h"

class UI {
public:
    static void begin();
    static void update();

private:
    static TFT_eSPI tft;
    static lv_disp_drv_t disp_drv;
    static lv_disp_t* disp;
    static lv_obj_t* soc_label;
    static lv_obj_t* input_power_label;
    static lv_obj_t* active_power_label;

    static void tft_init();
    static void lvgl_init();
    static void create_ui();
    static void disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
};

#endif // HAS_DISPLAY
#endif // UI_H