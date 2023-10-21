#ifndef JSON_FORMATTER_HPP_
#define JSON_FORMATTER_HPP_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <esp_wifi.h>

class JsonFormatter {
private:

	const char *tag = "JSON";
	const char *jsonInitializer;
	const char *jsonEnding;
	const char *colon;
	const char *openArrayKey;
	const char *closeArrayKey;
public:
	JsonFormatter();
	void createApRecordsData(wifi_ap_record_t *apRecords, char *buffer,
			uint8_t bufferLen);
	void (*logString)(const char *TAG, const char *message);
	void (*logDword)(const char *TAG, uint32_t logNumber);
	void (*logFloat)(const char *TAG, double logFloating);
	~JsonFormatter();
};
#endif
