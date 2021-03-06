cmake_minimum_required (VERSION 3.8)
project (vollo)

# Options
set (CMAKE_TOOLCHAIN_FILE "CMAKE_TOOLCHAIN_FILE-NOTFOUND" CACHE FILEPATH
    "Path to CMake toolchain file")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string (REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc")
    endif ()
else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif ()

message (STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
message (STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
find_package (SFML COMPONENTS system network window graphics audio REQUIRED)

add_subdirectory (src)

include (InstallRequiredSystemLibraries)
include (CPack)
