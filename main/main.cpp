#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_log.h>
#include <Formatter.hpp>
#include <InputsOutputs.hpp>
#include <TcpService.hpp>
#include <UartsFunctions.hpp>

using namespace std;

/*Memory size*/
#define KB (1024UL)
#define MB (KB)(KB)

static const char *tag = "Main";

Uarts *Uart;
Formatter *Format;
InputsOutputs *Gpio;
TcpService *Tcp;

void logString(const char *TAG, const char *message)
{
	ESP_LOGI(TAG, "%s", (unsigned char *)message);
}

void logDword(const char *TAG, uint32_t logNumber)
{
	char charNumber[KB];
	sprintf(charNumber, "%ld", logNumber);
	logString(TAG, charNumber);
}

void logFloat(const char *TAG, double logFloating)
{
	char charFloating[KB];
	sprintf(charFloating, "%f.2", logFloating);
	logString(TAG, (char *)charFloating);
}

void initObjects()
{
	Uart = new Uarts();
	Uart->logString = logString;
	Uart->logDword = logDword;
	Uart->logFloat = logFloat;
	Uart->UartInitializer(2);

	Format = new Formatter();
	Format->logString = logString;
	Format->logDword = logDword;
	Format->logFloat = logFloat;

	Gpio = new InputsOutputs();
	Gpio->logString = logString;
	Gpio->logDword = logDword;
	Gpio->logFloat = logFloat;
	Gpio->InitBlink();

	Tcp = new TcpService();
	Tcp->logString = logString;
	Tcp->logDword = logDword;
	Tcp->logFloat = logFloat;
	Tcp->InitWifiService(WiFiMode::ApStation);
}

extern "C" int app_main(void)
{
	logString(tag, "Go project!");
	initObjects();
	return 0;
}
