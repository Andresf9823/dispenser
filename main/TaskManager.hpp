#ifndef _TASK_HANDLER_HPP_
#define _TASK_HANDLER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_log.h>

class TaskManager {
	public:
		TaskManager();
		void (*logString)(const char *TAG, const char *message);
		void (*logDword)(const char *TAG, uint32_t logNumber);
		void (*logFloat)(const char *TAG, double logFloating);
		~TaskManager();
	private:
};

#endif
