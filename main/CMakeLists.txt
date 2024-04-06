# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    source/wired/Uarts.cpp
    source/fileSystem/LocalStorage.cpp
    source/network/protocols/Tcp.cpp
    source/network/protocols/Http.cpp
    source/network/wifi/Wifi.cpp
    source/network/ethernet/Ethernet.cpp
    source/format/Formatter.cpp
    source/gpio/Gpio.cpp
    main.cpp       		 # list the source files of this component
    INCLUDE_DIRS .       # optional, add here public include directories
    includes
    PRIV_INCLUDE_DIRS    # optional, add here private include directories
    REQUIRES             # optional, list the public requirements (component names)
    PRIV_REQUIRES        # optional, list the private requirements
)
