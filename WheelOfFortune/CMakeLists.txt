cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "WheelOfFortune")

if (WIN32)
    project(WheelOfFortune VERSION 1.0.0 LANGUAGES CXX)
elseif(UNIX)
    project(WheelOfFortune)
endif()

# TODO: ustawic zmienna
set(CMAKE_TOOLCHAIN_FILE "M:/Repo/VCPKG/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo;Debug" CACHE STRING "" FORCE)


#======================= INCLUSION OF Qt & OpenGL =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(Qt5 COMPONENTS Core Widgets Xml OpenGL REQUIRED)

find_package(OpenGL REQUIRED)
#find_package(freeglut REQUIRED)

find_package(GLEW REQUIRED)
find_package(FREEGLUT REQUIRED)


#SET(CMAKE_CXX_FLAGS "-O2 -std=c++11")

#=================== INCLUSION OF Project Files ====================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")


include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${RESOURCES_DIR}/*.qrc"
    "${RESOURCES_DIR}/*.xml"
    "${RESOURCES_DIR}/*.gif"
    "${RESOURCES_DIR}/*.png"
    "${INCLUDE_DIR}/*.h"
    "${INCLUDE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.cpp"
)


# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# suppress ZERO_CHECK project
#set(CMAKE_SUPPRESS_REGENERATION true)

# Add resources
list (APPEND RESOURCES resources/resources.qrc)
qt5_add_resources (RCC_SOURCES ${RESOURCES})

# Add the executable
if (WIN32) 
    add_executable(WheelOfFortune WIN32 ${SOURCES} ${RCC_SOURCES})
elseif(UNIX)
    add_executable(WheelOfFortune ${SOURCES} ${RCC_SOURCES})
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)


# Add the target includes for WheelOfFortune 
target_include_directories(WheelOfFortune PRIVATE ${FORMS_DIR})
target_include_directories(WheelOfFortune PRIVATE ${INCLUDE_DIR})
target_include_directories(WheelOfFortune PRIVATE ${SOURCE_DIR})


#===================== LINKING LIBRARIES =======================#
target_link_libraries(WheelOfFortune Qt5::Widgets Qt5::OpenGL Qt5::Xml OpenGL::GL freeglut)

#===================== SOURCE GROUPS (FOR VISUAL STUDIO) =======================#

# Function to recursively create source groups based on file structure
function(create_source_groups base_dir group_name)
    file(GLOB_RECURSE all_files
        RELATIVE ${base_dir}
        "${base_dir}/*"
    )

    foreach(file IN LISTS all_files)
        get_filename_component(dir "${file}" DIRECTORY)
        string(REPLACE "/" "\\" group_path "${dir}")  # Replace slashes for Visual Studio

        # Assign each file to its corresponding group (folder) in the project
        source_group("${group_name}\\${group_path}" FILES "${base_dir}/${file}")
    endforeach()
endfunction()

# Organize source files into groups based on their directory structure
create_source_groups(${INCLUDE_DIR} "Header Files")
create_source_groups(${SOURCE_DIR} "Source Files")
create_source_groups(${FORMS_DIR} "UI Files")
create_source_groups(${RESOURCES_DIR} "Resources Files")