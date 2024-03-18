#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <watchface.h>
#include <WiFi.h>
#include "ui.h"
#include "js.h"
#include "jsServer.h"

const uint32_t screenTimeout = 10000;
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const int timezone = +8;
const char *wifiSsid = "(ssid)";
const char *wifiPassword = "(password)";

void setup()
{
	Serial.begin(115200);
	watch.begin();
	beginLvglHelper();

	setupUi();

	configTime(timezone * 3600, 0, ntpServer1, ntpServer2);

	WiFi.begin(wifiSsid, wifiPassword);

	setupJs();
	setupJsServer();
}
void enterLightSleep()
{
	Serial.println("Enter light sleep mode.");

	auto brightness = watch.getBrightness();
	watch.decrementBrightness(0);

	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
	gpio_wakeup_enable((gpio_num_t)BOARD_TOUCH_INT, GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();

	esp_light_sleep_start();

	lv_disp_trig_activity(NULL);

	watch.incrementalBrightness(brightness);
}
void loop()
{
	lv_task_handler();
	if (lv_disp_get_inactive_time(NULL) >= screenTimeout)
		enterLightSleep();

	watchfaceHandler();
	jsServer.handleClient();

	delay(5);
}


