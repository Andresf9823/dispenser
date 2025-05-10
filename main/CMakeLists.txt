# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS
    source/infrastructure/storage/localStorage/LocalStorage.cpp
    source/infrastructure/network/protocols/protocols/TcpServer.cpp
    source/infrastructure/network/protocols/protocols/TcpClient.cpp
    source/infrastructure/network/protocols/protocols/Http.cpp
    source/infrastructure/network/wifi/Wifi.cpp
    source/infrastructure/network/ethernet/Ethernet.cpp
    source/infrastructure/format/Formatter.cpp
    source/app.cpp
    main.cpp       		 # list the source files of this component
    INCLUDE_DIRS .       # optional, add here public include directories
    includes
    PRIV_INCLUDE_DIRS    # optional, add here private include directories
    REQUIRES             # optional, list the public requirements (component names)
    PRIV_REQUIRES        # optional, list the private requirements
)

target_compile_options(${COMPONENT_LIB} PRIVATE -fexceptions)
target_compile_options(${COMPONENT_LIB} PRIVATE -frtti)