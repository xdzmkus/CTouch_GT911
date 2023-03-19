#include "CTouch_GT911.h"

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

void printTouchPoint(const Touch_Point& tp)
{
	Serial.print("Touch "); Serial.print(tp.id); Serial.print(": ");;
	Serial.print("  x: "); Serial.print(tp.x);
	Serial.print("  y: "); Serial.print(tp.y);
	Serial.print("  size: "); Serial.println(tp.size);
}

void on_1_PointTouched(const Touch_Point& tp1)
{
	printTouchPoint(tp1);
	Serial.println(' ');
}

void on_2_PointTouched(const Touch_Point& tp1, const Touch_Point& tp2)
{
	printTouchPoint(tp1);
	printTouchPoint(tp2);

	Serial.println(' ');
}

void on_3_PointTouched(const Touch_Point& tp1, const Touch_Point& tp2, const Touch_Point& tp3)
{
	printTouchPoint(tp1);
	printTouchPoint(tp2);
	printTouchPoint(tp3);

	Serial.println(' ');
}

void on_4_PointTouched(const Touch_Point& tp1, const Touch_Point& tp2, const Touch_Point& tp3, const Touch_Point& tp4)
{
	printTouchPoint(tp1);
	printTouchPoint(tp2);
	printTouchPoint(tp3);
	printTouchPoint(tp4);

	Serial.println(' ');
}

void on_5_PointTouched(const Touch_Point& tp1, const Touch_Point& tp2, const Touch_Point& tp3, const Touch_Point& tp4, const Touch_Point& tp5)
{
	printTouchPoint(tp1);
	printTouchPoint(tp2);
	printTouchPoint(tp3);
	printTouchPoint(tp4);
	printTouchPoint(tp5);

	Serial.println(' ');
}

void setup()
{
	Serial.begin(115200);
	Serial.println("GT911 Example: Touch screen and enjoy");

	ts.init(isr_On_Touched);
	ts.setDimensions(320, 480);
	ts.setRotation(2);

	ts.setCallback_1_PointTouched(on_1_PointTouched);
	ts.setCallback_2_PointTouched(on_2_PointTouched);
	ts.setCallback_3_PointTouched(on_3_PointTouched);
	ts.setCallback_4_PointTouched(on_4_PointTouched);
	ts.setCallback_5_PointTouched(on_5_PointTouched);
}

void loop()
{
	if (touched)
	{
		touched = false;

		ts.process();
	}
}
