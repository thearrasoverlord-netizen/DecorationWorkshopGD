cmake_minimum_required(VERSION 3.21)

project(DecorationWorkshopGD VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable is not set")
endif()

# Geode SDK 5.4.1 contains a duplicate CCFileUtilsWin32::init() declaration.
# Remove the duplicate before adding the SDK to this project.
set(GEODE_FILEUTILS_WIN32 "$ENV{GEODE_SDK}/loader/include/Geode/cocos/platform/win32/CCFileUtilsWin32.h")

if (EXISTS "${GEODE_FILEUTILS_WIN32}")
    file(READ "${GEODE_FILEUTILS_WIN32}" GEODE_FILEUTILS_CONTENT)
    string(REPLACE
        "    virtual bool isAbsolutePath(const gd::string& strPath);\n\n    virtual bool init();\n"
        "    virtual bool isAbsolutePath(const gd::string& strPath);\n"
        GEODE_FILEUTILS_CONTENT
        "${GEODE_FILEUTILS_CONTENT}"
    )
    file(WRITE "${GEODE_FILEUTILS_WIN32}" "${GEODE_FILEUTILS_CONTENT}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_BINARY_DIR}/geode)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

setup_geode_mod(${PROJECT_NAME})
