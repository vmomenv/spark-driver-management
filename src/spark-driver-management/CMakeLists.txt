cmake_minimum_required(VERSION 3.16)

project(spark-driver-management LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ 开启 Qt 的自动 MOC/UIC/RCC 功能
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 找 Qt 包
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

# 源码、头文件、UI、资源
set(SOURCES
    main.cpp
    mainwindow.cpp
    deviceinfo.cpp
    driverdownloader.cpp
    driverlist.cpp
    downloadwidget.cpp
)

set(HEADERS
    mainwindow.h
    deviceinfo.h
    driverdownloader.h
    driverlist.h
    downloadwidget.h
)

set(FORMS
    mainwindow.ui
    info.ui
)

set(RESOURCES
    res.qrc
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

# 头文件包含路径（ui_xxx.h）
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 可选：安装
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
