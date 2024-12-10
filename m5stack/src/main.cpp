#include <M5Unified.h>
#include <lvgl.h>
#include <gui/ui.h>
#include <main.h>


/*Change to your screen resolution*/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  M5.Display.startWrite();
  M5.Display.setAddrWindow(area->x1, area->y1, w, h);
  M5.Display.pushPixelsDMA((uint16_t *)&color_p->full, w * h, true);
  M5.Display.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (!M5.Display.touch())
  {
    Serial.println("Touch not found.");
    return;
  }

  uint16_t touchX = 0, touchY = 0;

  if (!M5.Display.getTouch(&touchX, &touchY))
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void lvgl_driver_init()
{
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
}
void update_time_label()
{
  // struct tm time_info;
  char time_buffer[32];

  // Get current time from RTC8563
  // auto time_info = M5.Rtc.getDateTime();
  auto t = time(nullptr);
  auto tm = localtime(&t);
  // Format the time as "HH:MM - DD/MM/YYYY"

  snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d - %02d/%02d/%04d",
           tm->tm_hour, tm->tm_min,
           tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

  // Update the label with the formatted time
  lv_label_set_text(ui_time, time_buffer);
  lv_refr_now(NULL);
}


void ui_start()
{
  lv_init();
  lvgl_driver_init();
  ui_init();

  // Tạo label thời gian
  lv_label_set_text(ui_time, "00:00 - 01/01/2024");
}

void setup()
{
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
  ui_start();

  Serial.begin(115200);

  // Connect to WiFi
  connectWiFi();


  // Connect to Adafruit IO MQTT
  connectMQTT();

  Wire.begin();

  init_bmp();
  init_sht();
  init_scd();

  //M5.Rtc.setDateTime({{2024, 12, 5}, {13, 07, 56}});

  M5.Rtc.setDateTime({{2024, 12, 5}, {13, 07, 56}});
}

void loop()
{
  //vTaskDelete(NULL);
  // vTaskDelete(NULL);
  //  Reconnect if the connection to MQTT is lost
  if (!mqtt.ping())
  {
    if (!mqtt.connected())
    {
      connectMQTT();
    }
  }

  updateValue();
  update_time_label();
  lv_task_handler();
  delay(8000);
}