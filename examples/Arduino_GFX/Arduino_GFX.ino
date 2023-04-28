#include <Arduino_GFX_Library.h>

#define TFT_MISO 19  // (leave TFT SDO disconnected if other SPI devices share MISO)
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   2   // Data Command control pin
#define TFT_RST  GFX_NOT_DEFINED // 4 - Reset pin (could connect to RST pin)

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus* bus = new Arduino_ESP32SPI(TFT_DC /* DC */, TFT_CS /* CS */, TFT_SCLK /* SCK */, TFT_MOSI /* MOSI */, TFT_MISO /* MISO */, VSPI /* spi_num */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX* gfx = new Arduino_ILI9488_18bit(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

#include <CTouch_GT911.h>

#define C_SDA  21
#define C_SCL  22
#define C_INT  16
#define C_RST  17

GT911 ts = GT911(C_SDA, C_SCL, C_INT, C_RST);

volatile bool touched = false; // The initial state of the panel is "not touched".

#if defined(ESP8266) || defined(ESP32)
IRAM_ATTR
#endif
void isr_On_Touched()
{
    touched = true; // The panel was touched, the interrupt was performed.
}

void on_1_PointTouched(const Touch_Point& tp)
{
    gfx->drawCircle(tp.x, tp.y, tp.size >> 1, WHITE);

    Serial.print("Touch "); Serial.print(tp.id); Serial.print(": ");;
    Serial.print("  x: "); Serial.print(tp.x);
    Serial.print("  y: "); Serial.print(tp.y);
    Serial.print("  size: "); Serial.println(tp.size);
}

// the setup function runs once when you press reset or power the board
void setup()
{
    Serial.begin(115200);
    Serial.println("GT911 Example: Touch screen and enjoy");

    gfx->begin();
    gfx->setRotation(3);
    gfx->fillScreen(BLACK);
    gfx->setTextSize(5);
    gfx->setTextColor(RED);
    gfx->setCursor(5, 5);
    gfx->printf("Touch me");

    ts.init(isr_On_Touched);
    ts.setDimensions(320, 480);
    ts.setRotation(3);
    ts.setCallback_1_PointTouched(on_1_PointTouched);
}

// the loop function runs over and over again until power down or reset
void loop()
{
    if (touched)
    {
        touched = false;

        ts.process();
    }
}
