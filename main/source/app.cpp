#include "app.hpp"

App::App() {

    try {
        ESP_LOGW(this->tag.c_str(), "%s", ">>>>>>>>>>>>> APP STARTED <<<<<<<<<<<<<<");
        
        LocalStorage *localStorage = new LocalStorage();
        WifiService *wifiService = new WifiService();
        
    } catch (const std::exception &e) {
        ESP_LOGE(this->tag.c_str(), "%s", e.what());
    }
}