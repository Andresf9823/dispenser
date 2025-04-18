#ifndef APP_HPP
#define APP_HPP

#include "infrastructure/storage/localStorage/LocalStorage.hpp"

class App {

private:
constexpr static string tag = "APP";
public:
    App();
    ~App() = default;
};

#endif // APP_HPP