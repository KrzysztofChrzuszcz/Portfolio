cmake_minimum_required(VERSION 3.16)

project(QmlHUD LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Quick)

# TODO: src/ & include/
set(SOURCES
    main.cpp
    abcontrol.cpp
    gauge.cpp
    hboard.cpp
    vbar.cpp
	)

set(INCLUDES
    abcontrol.h
    gauge.h
    hboard.h
    vbar.h
    utilities.h
)

set(RESOURCES
    qml.qrc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
