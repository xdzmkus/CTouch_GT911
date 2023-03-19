// TFT_MISO 19  // (leave TFT SDO disconnected if other SPI devices share MISO)
// TFT_MOSI 23
// TFT_SCLK 18
// TFT_CS   15  // Chip select control pin
// TFT_DC   2   // Data Command control pin
// TFT_RST  4   // Reset pin (could connect to RST pin)

#include <TFT_eSPI.h>

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

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
    tft.setCursor(5, 5, 2);
    tft.printf(" x: %i     ", tp.x);
    tft.setCursor(5, 20, 2);
    tft.printf(" y: %i    ", tp.y);

    tft.drawCircle(tp.x, tp.y, tp.size >> 1, TFT_WHITE);

    Serial.print("Touch "); Serial.print(tp.id); Serial.print(": ");;
    Serial.print("  x: "); Serial.print(tp.x);
    Serial.print("  y: "); Serial.print(tp.y);
    Serial.print("  size: "); Serial.println(tp.size);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("GT911 Example: Touch screen and enjoy");

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    ts.init(isr_On_Touched);
    ts.setDimensions(screenWidth, screenHeight);
    ts.setRotation(1);
    ts.setCallback_1_PointTouched(on_1_PointTouched);
}

void loop()
{
    if (touched)
    {
        touched = false;

        ts.process();
    }
}

