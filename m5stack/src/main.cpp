#include <M5Unified.h>
#include <lvgl.h>
#include "gui/ui.h"
#include <test.h>


/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

struct rtc_time_t {
    std::int8_t hours;
    std::int8_t minutes;
    std::int8_t seconds;

    rtc_time_t(std::int8_t hours_ = -1, std::int8_t minutes_ = -1, std::int8_t seconds_ = -1)
        : hours(hours_), minutes(minutes_), seconds(seconds_) {}
};

struct rtc_date_t {
    std::int16_t year;
    std::int8_t month;
    std::int8_t date;
    std::int8_t weekDay;

    rtc_date_t(std::int16_t year_ = 2000, std::int8_t month_ = 1, std::int8_t date_ = -1, std::int8_t weekDay_ = -1)
        : year(year_), month(month_), date(date_), weekDay(weekDay_) {}
};

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  M5.Display.startWrite();
  M5.Display.setAddrWindow(area->x1, area->y1, w, h);
  M5.Display.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
  M5.Display.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  if (!M5.Display.touch()) {
    Serial.println("Touch not found.");
    return;
  }

  uint16_t touchX = 0, touchY = 0;

  if (!M5.Display.getTouch(&touchX, &touchY)) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

// void my_log(const char *buf)
// {
//   Serial.println(buf);
// }

void lvgl_driver_init() {
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /*Print to serial for debug purpose*/
  // lv_log_register_print_cb(my_log);
}

void update_time_label() {
    // Lấy thời gian và ngày tháng từ RTC
    rtc_time_t rtc_time;
    rtc_date_t rtc_date;

    // Format dữ liệu thời gian và ngày tháng thành chuỗi
    char buffer[64];
    snprintf(buffer, sizeof(buffer), 
             "%04d/%02d/%02d %02d:%02d:%02d", 
             rtc_date.year, rtc_date.month, rtc_date.date,
             rtc_time.hours, rtc_time.minutes, rtc_time.seconds);

    // Cập nhật nhãn giao diện với chuỗi định dạng
    lv_label_set_text(ui_time, buffer);
}

void ui_start(void *parameter){
  lv_init();
  lvgl_driver_init();
  ui_init();

  // Tạo label thời gian

  lv_label_set_text(ui_time, "00:00 - 01/01/2024");

  for (;;) {
    // if (xSemaphoreTake(lvgl_mutex, 0) == pdTRUE) {
    //   // Critical section (access shared resource here)
    //   lv_timer_handler();

    //   // Release the mutex after critical section
    //   xSemaphoreGive(lvgl_mutex);
    // }
    // delay(10);
    update_time_label();
    lv_timer_handler();
    delay(5);
  }
}


void setup()
{
  // connectWiFi();
  // connectMQTT();

  m5::M5Unified::config_t cfg = M5.config();

  cfg.serial_baudrate = 115200; // default=115200. if "Serial" is not needed, set it to 0.
  cfg.clear_display = true;     // default=true. clear the screen when begin.
  cfg.output_power = true;      // default=true. use external port 5V output.
  cfg.internal_imu = true;      // default=true. use internal IMU.
  cfg.internal_rtc = true;      // default=true. use internal RTC.
  cfg.internal_spk = false;     // default=true. use internal speaker.
  cfg.internal_mic = false;     // default=true. use internal microphone.
  cfg.external_imu = true;      // default=false. use Unit Accel & Gyro.
  cfg.external_rtc = false;     // default=false. use Unit RTC.
  cfg.led_brightness = 64;      // default= 0. system LED brightness (0=off / 255=max) (※ not NeoPixel)
  M5.begin(cfg);
  M5.Display.setRotation(1);
  xTaskCreatePinnedToCore(ui_start, "ui_start", 4096, NULL, 5, NULL, tskNO_AFFINITY);


}

void loop()
{
      // Reconnect if the connection to MQTT is lost
    // if (!mqtt.ping())
    // {
    //     if (!mqtt.connected())
    //     {
    //         connectMQTT();
    //     }
    // }

    // // Publish dummy temperature data
    // float temperature = random(20, 30); // Example temperature data
    // if (!temperatureFeed.publish(temperature))
    // {
    //     Serial.println("Failed to publish temperature");
    // }
    // else
    // {
    //     Serial.println("Temperature published!");
    // }

    // delay(5000);
  // vTaskDelete(NULL);
}