target_sources (
    vollo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/primitives.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/trace.cpp
)

target_include_directories (vollo PRIVATE include)
