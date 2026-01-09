#include "ui.h"

#ifdef HAS_DISPLAY

TFT_eSPI UI::tft = TFT_eSPI();
lv_disp_drv_t UI::disp_drv;
lv_disp_t* UI::disp = nullptr;
lv_obj_t* UI::soc_label = nullptr;
lv_obj_t* UI::input_power_label = nullptr;
lv_obj_t* UI::active_power_label = nullptr;

void UI::begin() {
    tft_init();
    lvgl_init();
    create_ui();
}

void UI::tft_init() {
    tft.init();
    tft.setRotation(1);  // Landscape
    tft.fillScreen(TFT_BLACK);
}

void UI::lvgl_init() {
    lv_init();
    
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[TFT_WIDTH * 10];
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_WIDTH;
    disp_drv.ver_res = TFT_HEIGHT;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    disp = lv_disp_drv_register(&disp_drv);
}

void UI::create_ui() {
    lv_obj_t* screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);

    // Title
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Huawei Sun2000 Inverter");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // SOC Label
    soc_label = lv_label_create(screen);
    lv_label_set_text(soc_label, "SOC: ---%");
    lv_obj_set_style_text_color(soc_label, lv_color_hex(0x00FF00), 0);
    lv_obj_align(soc_label, LV_ALIGN_CENTER, -50, -30);

    // Input Power Label
    input_power_label = lv_label_create(screen);
    lv_label_set_text(input_power_label, "Input Power: -- W");
    lv_obj_set_style_text_color(input_power_label, lv_color_hex(0x00FFFF), 0);
    lv_obj_align(input_power_label, LV_ALIGN_CENTER, -50, 0);

    // Active Power Label
    active_power_label = lv_label_create(screen);
    lv_label_set_text(active_power_label, "Active Power: -- W");
    lv_obj_set_style_text_color(active_power_label, lv_color_hex(0xFFFF00), 0);
    lv_obj_align(active_power_label, LV_ALIGN_CENTER, -50, 30);
}

void UI::update() {
    HuaweiInverter& inverter = HuaweiInverter::getInstance();
    
    // Update labels with latest data
    char buffer[32];
    
    snprintf(buffer, sizeof(buffer), "SOC: %d%%", inverter.getSoc());
    lv_label_set_text(soc_label, buffer);
    
    snprintf(buffer, sizeof(buffer), "Input Power: %d W", inverter.getInputPower());
    lv_label_set_text(input_power_label, buffer);
    
    snprintf(buffer, sizeof(buffer), "Active Power: %d W", inverter.getActivePower());
    lv_label_set_text(active_power_label, buffer);

    // Trigger LVGL rendering
    lv_task_handler();
}

void UI::disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)color_p, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

#endif // HAS_DISPLAY