#ifdef HAS_DISPLAY

#include <lvgl.h>
#include <TFT_eSPI.h>  // Display driver
#include "huawei_inverter.h"

// Forward declarations
namespace UI {
    void init();
    void update(HuaweiInverter& inverter);
}

// Global display and touch objects
static TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t draw_buf;
static lv_color_t* buf1 = nullptr;
static lv_color_t* buf2 = nullptr;

// LVGL display driver functions
void my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

// UI Elements
static lv_obj_t* soc_label = nullptr;
static lv_obj_t* input_power_label = nullptr;
static lv_obj_t* active_power_label = nullptr;
static lv_obj_t* status_label = nullptr;

namespace UI {
    void init() {
        // Initialize LVGL
        lv_init();

        // Initialize display
        tft.init();
        tft.setRotation(1);  // Landscape mode
        tft.fillScreen(TFT_BLACK);

        // Allocate draw buffers
        buf1 = (lv_color_t*)malloc(TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t));
        buf2 = (lv_color_t*)malloc(TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t));
        lv_disp_draw_buf_init(&draw_buf, buf1, buf2, TFT_WIDTH * TFT_HEIGHT);

        // Initialize display driver
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = TFT_WIDTH;
        disp_drv.ver_res = TFT_HEIGHT;
        disp_drv.flush_cb = my_disp_flush;
        disp_drv.draw_buf = &draw_buf;
        lv_disp_drv_register(&disp_drv);

        // Create UI layout
        lv_obj_t* screen = lv_scr_act();
        lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

        // Title
        lv_obj_t* title = lv_label_create(screen);
        lv_label_set_text(title, "Huawei Sun2000 Inverter");
        lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
        lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

        // SOC Label
        soc_label = lv_label_create(screen);
        lv_label_set_text(soc_label, "SOC: ---%");
        lv_obj_set_style_text_color(soc_label, lv_color_hex(0x00FF00), 0);
        lv_obj_set_style_text_font(soc_label, &lv_font_montserrat_24, 0);
        lv_obj_align(soc_label, LV_ALIGN_TOP_LEFT, 10, 50);

        // Input Power Label
        input_power_label = lv_label_create(screen);
        lv_label_set_text(input_power_label, "Input Power: -- W");
        lv_obj_set_style_text_color(input_power_label, lv_color_hex(0x00FFFF), 0);
        lv_obj_set_style_text_font(input_power_label, &lv_font_montserrat_20, 0);
        lv_obj_align(input_power_label, LV_ALIGN_TOP_LEFT, 10, 100);

        // Active Power Label
        active_power_label = lv_label_create(screen);
        lv_label_set_text(active_power_label, "Active Power: -- W");
        lv_obj_set_style_text_color(active_power_label, lv_color_hex(0xFFFF00), 0);
        lv_obj_set_style_text_font(active_power_label, &lv_font_montserrat_20, 0);
        lv_obj_align(active_power_label, LV_ALIGN_TOP_LEFT, 10, 150);

        // Status Label
        status_label = lv_label_create(screen);
        lv_label_set_text(status_label, "Status: Initializing");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0xAAAAAA), 0);
        lv_obj_set_style_text_font(status_label, &lv_font_montserrat_16, 0);
        lv_obj_align(status_label, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    }

    void update(HuaweiInverter& inverter) {
        // Update labels with current inverter data
        if (inverter.isDataValid()) {
            // Update SOC
            char soc_text[32];
            snprintf(soc_text, sizeof(soc_text), "SOC: %.1f%%", inverter.getSoc());
            lv_label_set_text(soc_label, soc_text);

            // Update Input Power
            char input_power_text[32];
            snprintf(input_power_text, sizeof(input_power_text), "Input Power: %.1f W", inverter.getInputPower());
            lv_label_set_text(input_power_label, input_power_text);

            // Update Active Power
            char active_power_text[32];
            snprintf(active_power_text, sizeof(active_power_text), "Active Power: %.1f W", inverter.getActivePower());
            lv_label_set_text(active_power_label, active_power_text);

            // Update Status
            lv_label_set_text(status_label, "Status: Connected");
            lv_obj_set_style_text_color(status_label, lv_color_hex(0x00FF00), 0);
        } else {
            // Update Status if no valid data
            lv_label_set_text(status_label, "Status: No Data");
            lv_obj_set_style_text_color(status_label, lv_color_hex(0xFF0000), 0);
        }

        // Trigger LVGL to render updates
        lv_task_handler();
    }
}

#endif // HAS_DISPLAY