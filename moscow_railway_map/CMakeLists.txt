cmake_minimum_required(VERSION 3.16)
project(MoscowRailwayMap VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mapwidget.cpp
    src/railwaydata.cpp
    src/stationmarker.cpp
)

set(HEADERS
    include/mainwindow.h
    include/mapwidget.h
    include/railwaydata.h
    include/stationmarker.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
