project("Training")

cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(options)
include(utils)

find_package(LIBNX REQUIRED)
if (NOT LIBNX_FOUND)
    cmake_panic("Unable to detect libnx on this system.")
endif ()

include_directories(${PROJECT_BINARY_DIR})

# Replace this with the name of your application
set(HOMEBREW_APP "Training")

# Meta information about the app
set(APP_TITLE ${HOMEBREW_APP})
set(APP_AUTHOR "Topru")
set(APP_ICON "${CMAKE_SOURCE_DIR}/assets/icon.jpg")
set(TARGET ${HOMEBREW_APP}.elf)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_MICRO 1)
set(APP_VERSION
        "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}")

if (NOT DEFINED CMAKE_BUILD_TYPE_INIT)
    set(CMAKE_BUILD_TYPE_INIT Release)
endif ()

set(CMAKE_C_STANDARD 99)
if (USE_CPP_17)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(clang-utils)
endif ()

include(nx-utils)

cmake_info("Building ${APP_TITLE} version ${APP_VERSION}.")

set(SWURL_PATH ${DEVKITPRO}/swurl)

add_executable(${TARGET}
        ${PROJECT_SOURCE_DIR}/src/main.cpp)

include_directories(include)
include_directories(${PORTLIBS}/include)
include_directories(${SWURL_PATH}/include)

file(GLOB SWURL_LIBS ${SWURL_PATH}/lib/*.a)

target_link_libraries(${TARGET} switch::libnx)
target_link_libraries(${TARGET} ${SWURL_LIBS})

target_link_libraries(${TARGET} curl z mbedtls mbedx509 mbedcrypto nx)


set_target_properties(${TARGET} PROPERTIES
        LINKER_LANGUAGE CXX # Replace this with C if you have C source files
        LINK_FLAGS "-specs=${LIBNX}/switch.specs -Wl,-no-as-needed -Wl,-Map,.map"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG target
        RUNTIME_OUTPUT_DIRECTORY_RELEASE target)

build_switch_binaries(${TARGET})
