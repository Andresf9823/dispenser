#include "JsonFormatter.hpp"

JsonFormatter::JsonFormatter(/* args */) { // @suppress("Class members should be properly initialized")

	jsonInitializer = "{";
	jsonEnding = "}";
	colon = ";";
	openArrayKey = "[";
	closeArrayKey = "]";
}

void JsonFormatter::createApRecordsData(wifi_ap_record_t *apRecords,
		char *buffer, uint8_t bufferLen) {
	memset(buffer, 0, bufferLen);
	sprintf(buffer, jsonInitializer);

	for (int i = 0; i < sizeof(apRecords); i++) {

		char ssid[33];
		char bssid[6];
		char rssi[8];
		char authmode[2];

		sprintf(ssid, "%s", apRecords[i].ssid);
		sprintf(bssid, "%s", apRecords[i].bssid);
		sprintf(rssi, "%d", apRecords[i].rssi);
		sprintf(authmode, "%d", apRecords[i].authmode);

		strcat(buffer, "[");
		strcat(buffer, "\"ssid\":");
		strcat(buffer, ssid);
		strcat(buffer, ",\"bssid\":");
		strcat(buffer, bssid);
		strcat(buffer, ",\"rssi\":");
		strcat(buffer, rssi);
		strcat(buffer, ",\"authmode\":");
		strcat(buffer, authmode);
		strcat(buffer, "]");

		if (i != sizeof(apRecords) - 1) {
			strcat(buffer, ",");
		}

	}

	this->logString(this->tag, "Access points list: ");
	this->logString(this->tag, buffer);

}

JsonFormatter::~JsonFormatter(/* args */) {
}
