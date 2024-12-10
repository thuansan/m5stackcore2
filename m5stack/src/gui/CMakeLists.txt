SET(SOURCES ui_Screen1.c
    ui_Screen2.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_layout2_png.c
    ui_img_weathe_png.c
    ui_img_999427845.c
    ui_img_humi_png.c
    ui_img_wifi_png.c
    ui_img_cloud_png.c
    ui_img_co2_png.c)

add_library(ui ${SOURCES})
