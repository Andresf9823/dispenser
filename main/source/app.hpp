#ifndef APP_HPP
#define APP_HPP

#include "infrastructure/storage/localStorage/LocalStorage.hpp"
#include "infrastructure/network/protocols/protocols/TcpServer.hpp"

class App {

private:
constexpr static string tag = "APP";
unique_ptr<WifiService> wifiService;
public:
    App();
    ~App() = default;
};

#endif // APP_HPP