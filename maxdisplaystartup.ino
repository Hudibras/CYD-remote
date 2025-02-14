#include <Arduino.h>
#include <lvgl.h>
#include "CST820.h"

#define TOUCH

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#define I2C_SDA 21
#define I2C_SCL 22

#ifdef TOUCH 
  CST820 touch(I2C_SDA, I2C_SCL, -1, -1);
#endif

#define GFX
#if defined(GFX)
#define LGFX_USE_V1      // set to use new version of library
#include <LovyanGFX.hpp> // main library

class LGFX : public lgfx::LGFX_Device
{

    lgfx::Panel_ST7789 _panel_instance; // ST7789UI
    lgfx::Bus_Parallel8 _bus_instance;  // MCU8080 8B

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();
            cfg.freq_write = 25000000;
            cfg.pin_wr = 4;
            cfg.pin_rd = 2;
            cfg.pin_rs = 16;

            cfg.pin_d0 = 15;
            cfg.pin_d1 = 13;
            cfg.pin_d2 = 12;
            cfg.pin_d3 = 14;
            cfg.pin_d4 = 27;
            cfg.pin_d5 = 25;
            cfg.pin_d6 = 33;
            cfg.pin_d7 = 32;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = 17;
            cfg.pin_rst = -1;
            cfg.pin_busy = -1;

            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            // cfg.dummy_read_pixel = 8;
            // cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};

static LGFX tft; // declare display variable

#endif

#if defined(GFX)
/*
    lcd interface
    transfer pixel data range to lcd
*/
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    int w = (area->x2 - area->x1 + 1);
    int h = (area->y2 - area->y1 + 1);

    tft.startWrite();                            /* Start new TFT transaction */
    tft.setAddrWindow(area->x1, area->y1, w, h); /* set the working window */
    tft.writePixels(&color_p->full, w * h,false);//true

    tft.endWrite();            /* terminate TFT transaction */
    lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}
#endif

#ifdef TOUCH
/*Læs TOUCHPAD*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

    bool touched;
    uint8_t gesture;
    uint16_t touchX, touchY;

    touched = touch.getTouch(&touchX, &touchY, &gesture);

    if (!touched)
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
#endif

int incomedate = 0;

void setup() {
  // put your setup code here, to run once:
    
    #ifdef TOUCH
      touch.begin(); /*Initialiser touchpad'en*/
    #endif

    lv_init();
    // lv_img_cache_set_size(10);

    tft.init();
    
    #ifdef TOUCH
        touch.begin(); /*Initialiser touchpad'en*/
    #endif

    
   // WiFi.setAutoReconnect(true);
   // status = WiFi.begin("Kirkebakken39", "oz1ldoKim");
   // if(status != WL_CONNECTED){
   //   Serial.println("venter på connect");
   //   delay(1000);
   //   }//end if
   Serial.begin(115200);
    tft.setRotation(0);
    tft.fillScreen(TFT_BLUE);
    tft.setTextSize(3);
    tft.setTextColor(TFT_BLACK);

  tft.drawRect   (   9,   9, 107,  52, TFT_BLACK ); // forsøg dobbel linie
  tft.drawRect   (  10,  10, 105,  50, TFT_BLACK ); tft.fillRect (  11, 11, 103, 48, TFT_RED );  // udfylde et rektangel

  tft.setCursor(17, 25);
  tft.print("CYKEL");
   
  tft.drawRect   ( 125,  10, 105,  50, TFT_BLACK ); tft.fillRect ( 126, 11, 103, 48, TFT_GREEN );
  tft.setCursor(127, 25);
  tft.print(" FLEX");
  
  tft.drawRect   (  10,  70, 105,  50, TFT_BLACK ); tft.fillRect (  11, 71, 103, 48, TFT_GREEN );
  tft.setCursor(19,85);
  tft.print("Varde");
  
  tft.drawRect   ( 125,  70, 105,  50, TFT_BLACK ); tft.fillRect ( 126, 71, 103, 48, TFT_GREEN ); 
  tft.setCursor(127, 85);
  tft.print(" Ribe");

  tft.drawRect   ( 10, 130, 105,  50, TFT_BLACK ); tft.fillRect (  11,  131, 103, 48, TFT_GREEN);
  
  tft.drawRect   ( 125, 130, 105,  50, TFT_BLACK); tft.fillRect ( 126, 131, 103, 48, TFT_GREEN);
  
}

void loop() {
//  If touched {tft.print("Touched);
//  tft.print(touchX);
//  tft.print(touchY);
//  }
  // put your main code here, to run repeatedly:
  
  //  std::int32_t x, y, number = 0;
  //while (display.getTouch(&x, &y, number))
  //{
  //  tft.fillCircle(x, y, 5, (std::uint32_t)(number * 0x333333u));
  //  ++number;
  // }
  if (Serial.available() > 0)
  {
    incomedate = Serial.read();
    if (incomedate == 'H')
    {
      Serial.println("I am OK!");
    }
  }
  delay(1000);

}
/*
  fillScreen    (                color);  // Fylder hele skærmen
  drawPixel     ( x, y         , color);  // punkt
  drawFastVLine ( x, y   , h   , color);  // lodret linje
  drawFastHLine ( x, y, w      , color);  // vandret linje
  drawRect      ( x, y, w, h   , color);  // omkreds af rektangel
  fillRect      ( x, y, w, h   , color);  // udfylde et rektangel
  drawRoundRect ( x, y, w, h, r, color);  // Omkreds af rektangel med afrundede hjørner
  fillRoundRect ( x, y, w, h, r, color);  // Fyldning af et rektangel med afrundede hjørner
  drawCircle    ( x, y      , r, color);  // omkredsen af en cirkel
  fillCircle    ( x, y      , r, color);  // At udfylde cirklen
  drawEllipse   ( x, y, rx, ry , color);  // omkreds af ellipse
  fillEllipse   ( x, y, rx, ry , color);  // Fylder ellipsen
  drawLine      ( x0, y0, x1, y1        , color); // lige linje mellem to punkter
  drawTriangle  ( x0, y0, x1, y1, x2, y2, color); // Omkreds af trekanten mellem tre punkter
  fillTriangle  ( x0, y0, x1, y1, x2, y2, color); // Udfyldning af en trekant mellem tre punkter
  drawBezier    ( x0, y0, x1, y1, x2, y2, color); // Bezier-kurve mellem tre punkter
  drawBezier    ( x0, y0, x1, y1, x2, y2, x3, y3, color); // Bezier-kurve mellem 4 punkter
  drawArc       ( x, y, r0, r1, angle0, angle1, color);   // buens omkreds
  fillArc       ( x, y, r0, r1, angle0, angle1, color);   // Fyldning af buen
*/
