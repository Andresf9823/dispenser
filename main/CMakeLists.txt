# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    UartsFunctions.cpp
    TcpService.cpp
    WebApiConsumer.cpp
    InputsOutputs.cpp  
    

    #Network .cpp files
    source/network/wifi/Wifi.cpp
    source/network/wifi/modes/WifiModeAp.cpp
    source/network/wifi/modes/WifiModeSta.cpp
    source/network/wifi/modes/WifiModeApSta.cpp

    #Network protocols files
    source/network/protocols/TcpProtocol.cpp
    source/network/protocols/UdpProtocol.cpp

    #Local Storage files
    source/database/Formatter.cpp
    source/database/localstorage/LocalStorage.cpp

    main.cpp       		 # list the source files of this component
    INCLUDE_DIRS .       # optional, add here public include directories
    includes
    PRIV_INCLUDE_DIRS    # optional, add here private include directories
    REQUIRES             # optional, list the public requirements (component names)
    PRIV_REQUIRES        # optional, list the private requirements
)
