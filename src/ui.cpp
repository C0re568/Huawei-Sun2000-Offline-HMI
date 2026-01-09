#include "ui.h"
#include "wifi_manager.h"

#ifdef HAS_DISPLAY

TFT_eSPI UI::tft = TFT_eSPI();
lv_disp_drv_t UI::disp_drv;
lv_disp_t* UI::disp = nullptr;
lv_obj_t* UI::soc_label = nullptr;
lv_obj_t* UI::input_power_label = nullptr;
lv_obj_t* UI::active_power_label = nullptr;
lv_obj_t* UI::modbus_status_label = nullptr;
lv_obj_t* UI::wifi_status_label = nullptr;
lv_obj_t* UI::modbus_status_icon = nullptr;
lv_obj_t* UI::wifi_status_icon = nullptr;

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

    // Status Indicators
    // Modbus Status
    modbus_status_label = lv_label_create(screen);
    lv_label_set_text(modbus_status_label, "Modbus");
    lv_obj_set_style_text_color(modbus_status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(modbus_status_label, LV_ALIGN_BOTTOM_LEFT, 10, -20);

    modbus_status_icon = lv_obj_create(screen);
    lv_obj_set_size(modbus_status_icon, 20, 20);
    lv_obj_align_to(modbus_status_icon, modbus_status_label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    setStatusIconColor(modbus_status_icon, DISCONNECTED);

    // WiFi Status
    wifi_status_label = lv_label_create(screen);
    lv_label_set_text(wifi_status_label, "WiFi");
    lv_obj_set_style_text_color(wifi_status_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(wifi_status_label, LV_ALIGN_BOTTOM_RIGHT, -10, -20);

    wifi_status_icon = lv_obj_create(screen);
    lv_obj_set_size(wifi_status_icon, 20, 20);
    lv_obj_align_to(wifi_status_icon, wifi_status_label, LV_ALIGN_OUT_LEFT_MID, -10, 0);
    setStatusIconColor(wifi_status_icon, DISCONNECTED);
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

    // Update Modbus connection status
    ConnectionStatus modbusStatus = inverter.isConnected() ? CONNECTED : DISCONNECTED;
    updateModbusStatus(modbusStatus);

    // Update WiFi connection status
    bool isWiFiConnected = WiFiManager::getConnectionStatus();
    ConnectionStatus wifiStatus = isWiFiConnected ? CONNECTED : DISCONNECTED;
    updateWiFiStatus(wifiStatus);

    // Trigger LVGL rendering
    lv_task_handler();
}

void UI::setStatusIconColor(lv_obj_t* icon, ConnectionStatus status) {
    if (!icon) return;

    lv_color_t color;
    switch (status) {
        case DISCONNECTED:
            color = lv_color_hex(0xFF0000);  // Red
            break;
        case CONNECTING:
            color = lv_color_hex(0xFFFF00);  // Yellow
            break;
        case CONNECTED:
            color = lv_color_hex(0x00FF00);  // Green
            break;
        case ERROR:
            color = lv_color_hex(0xFF6600);  // Orange
            break;
        default:
            color = lv_color_hex(0x808080);  // Gray
    }

    lv_obj_set_style_bg_color(icon, color, 0);
    lv_obj_set_style_border_width(icon, 0, 0);
    lv_obj_set_style_radius(icon, LV_RADIUS_CIRCLE, 0);
}

void UI::updateModbusStatus(ConnectionStatus status) {
    if (!modbus_status_label || !modbus_status_icon) return;

    // Update icon color
    setStatusIconColor(modbus_status_icon, status);

    // Update label text
    switch (status) {
        case DISCONNECTED:
            lv_label_set_text(modbus_status_label, "Modbus: Disconnected");
            break;
        case CONNECTING:
            lv_label_set_text(modbus_status_label, "Modbus: Connecting");
            break;
        case CONNECTED:
            lv_label_set_text(modbus_status_label, "Modbus: Connected");
            break;
        case ERROR:
            lv_label_set_text(modbus_status_label, "Modbus: Error");
            break;
    }
}

void UI::updateWiFiStatus(ConnectionStatus status) {
    if (!wifi_status_label || !wifi_status_icon) return;

    // Update icon color
    setStatusIconColor(wifi_status_icon, status);

    // Update label text
    switch (status) {
        case DISCONNECTED:
            lv_label_set_text(wifi_status_label, "WiFi: Disconnected");
            break;
        case CONNECTING:
            lv_label_set_text(wifi_status_label, "WiFi: Connecting");
            break;
        case CONNECTED:
            lv_label_set_text(wifi_status_label, "WiFi: Connected");
            break;
        case ERROR:
            lv_label_set_text(wifi_status_label, "WiFi: Error");
            break;
    }
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